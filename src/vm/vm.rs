use crate::vm::opcode::OpCode;
use crate::vm::object::{ObjFunction, ObjType};
use crate::vm::memory::Memory;
use crate::eval::value::{Value};
use crate::eval::native::get_native_functions;
use std::collections::HashMap;
use std::rc::Rc;

#[derive(Debug, PartialEq)]
pub enum InterpretResult {
    Ok,
    CompileError,
    RuntimeError,
}

struct CallFrame {
    function: Rc<ObjFunction>,
    ip: usize,
    slots_offset: usize,
}

/// The Lavina Virtual Machine.
pub struct VM {
    frames: Vec<CallFrame>,
    stack: Vec<Value>,
    globals: HashMap<String, Value>,
    memory: Memory,
}

impl VM {
    pub fn new() -> Self {
        let mut vm = Self {
            frames: Vec::with_capacity(64),
            stack: Vec::with_capacity(256),
            globals: HashMap::new(),
            memory: Memory::new(),
        };
        
        for (name, func) in get_native_functions() {
            vm.globals.insert(name.clone(), Value::NativeFunction(name, func));
        }
        
        vm
    }

    // --- Core Methods ---

    pub fn interpret(&mut self, function: Rc<ObjFunction>) -> InterpretResult {
        self.stack.clear();
        self.push(Value::ObjFunction(function.clone()));
        let frame = CallFrame {
            function,
            ip: 0,
            slots_offset: 0,
        };
        self.frames.push(frame);
        self.run()
    }

    fn push(&mut self, value: Value) {
        self.stack.push(value);
    }

    fn pop(&mut self) -> Value {
        self.stack.pop().expect("Stack underflow!")
    }

    fn peek(&self, distance: usize) -> &Value {
        &self.stack[self.stack.len() - 1 - distance]
    }

    fn current_frame(&mut self) -> &mut CallFrame {
        self.frames.last_mut().expect("No call frame")
    }

    // --- Main Execution Loop ---

    fn run(&mut self) -> InterpretResult {
        loop {
            let instruction = self.read_byte();
            match OpCode::from(instruction) {
                OpCode::Constant => {
                    let mut constant = self.read_constant();
                    if let Value::String(s) = constant {
                        let idx = self.memory.alloc(ObjType::String(s));
                        constant = Value::Object(idx);
                    }
                    self.push(constant);
                }
                OpCode::True => self.push(Value::Bool(true)),
                OpCode::False => self.push(Value::Bool(false)),
                OpCode::Null => self.push(Value::Null),
                OpCode::Pop => { self.pop(); }
                
                OpCode::GetLocal => {
                    let slot = self.read_byte() as usize;
                    let offset = self.frames.last().unwrap().slots_offset;
                    let val = self.stack[offset + slot].clone();
                    self.push(val);
                }
                OpCode::SetLocal => {
                    let slot = self.read_byte() as usize;
                    let offset = self.frames.last().unwrap().slots_offset;
                    self.stack[offset + slot] = self.peek(0).clone();
                }
                OpCode::DefineGlobal => {
                    let name = self.read_string_constant();
                    let val = self.pop();
                    self.globals.insert(name, val);
                }
                OpCode::GetGlobal => {
                    let name = self.read_string_constant();
                    match self.globals.get(&name) {
                        Some(val) => self.push(val.clone()),
                        None => {
                            let msg = format!("Undefined variable '{}'.", name);
                            return self.runtime_error(&msg);
                        }
                    }
                }
                OpCode::SetGlobal => {
                    let name = self.read_string_constant();
                    let val = self.peek(0).clone();
                    if self.globals.contains_key(&name) {
                        self.globals.insert(name, val);
                    } else {
                        let msg = format!("Undefined variable '{}'.", name);
                        return self.runtime_error(&msg);
                    }
                }

                OpCode::JumpIfFalse => {
                    let offset = self.read_short();
                    if !self.peek(0).is_truthy() {
                        self.current_frame().ip += offset as usize;
                    }
                }
                OpCode::Jump => {
                    let offset = self.read_short();
                    self.current_frame().ip += offset as usize;
                }
                OpCode::Loop => {
                    let offset = self.read_short();
                    self.current_frame().ip -= offset as usize;
                }

                OpCode::Equal => {
                    let b = self.pop();
                    let a = self.pop();
                    self.push(Value::Bool(self.values_equal(a, b)));
                }
                OpCode::Greater => {
                    let b = self.pop();
                    let a = self.pop();
                    self.push(Value::Bool(a > b));
                }
                OpCode::Less => {
                    let b = self.pop();
                    let a = self.pop();
                    self.push(Value::Bool(a < b));
                }

                OpCode::Add => {
                    let b = self.pop();
                    let a = self.pop();
                    match (a, b) {
                        (Value::Int(x), Value::Int(y)) => self.push(Value::Int(x + y)),
                        (Value::Float(x), Value::Float(y)) => self.push(Value::Float(x + y)),
                        (l, r) => {
                            let ls = self.value_to_string(&l);
                            let rs = self.value_to_string(&r);
                            if l.is_string() || r.is_string() {
                                let new_str = ls + &rs;
                                let idx = self.memory.alloc(ObjType::String(new_str));
                                self.push(Value::Object(idx));
                            } else {
                                return self.runtime_error("Operands must be two numbers or two strings.");
                            }
                        }
                    }
                }
                OpCode::Subtract => { self.binary_op(|a, b| a - b); }
                OpCode::Multiply => { self.binary_op(|a, b| a * b); }
                OpCode::Divide => { self.binary_op(|a, b| a / b); }
                OpCode::Negate => {
                    let val = self.pop();
                    match val {
                        Value::Int(i) => self.push(Value::Int(-i)),
                        Value::Float(f) => self.push(Value::Float(-f)),
                        _ => return self.runtime_error("Operand must be a number."),
                    }
                }
                OpCode::Not => {
                    let val = self.pop();
                    self.push(Value::Bool(!val.is_truthy()));
                }

                OpCode::Call => {
                    let arg_count = self.read_byte() as usize;
                    if let Err(e) = self.call_value(arg_count) {
                        return self.runtime_error(&e);
                    }
                }

                OpCode::Vector => {
                    let count = self.read_byte() as usize;
                    let mut elements = Vec::with_capacity(count);
                    for _ in 0..count {
                        elements.push(self.pop());
                    }
                    elements.reverse();
                    let idx = self.memory.alloc(ObjType::Vector(elements));
                    self.push(Value::Object(idx));
                }
                OpCode::HashMap => {
                    let count = self.read_byte() as usize;
                    let mut map = std::collections::HashMap::with_capacity(count);
                    for _ in 0..count {
                        let value = self.pop();
                        let key = self.pop();
                        map.insert(self.value_to_string(&key), value);
                    }
                    let idx = self.memory.alloc(ObjType::HashMap(map));
                    self.push(Value::Object(idx));
                }

                OpCode::GetIndex => {
                    let index = self.pop();
                    let collection = self.pop();
                    if let Value::Object(idx) = collection {
                        let key = self.value_to_string(&index);
                        let obj = self.memory.heap[idx].as_ref().unwrap();
                        match &obj.obj_type {
                            ObjType::Vector(v) => {
                                if let Some(i) = index.as_int() {
                                    if i < 0 || i >= v.len() as i64 {
                                        return self.runtime_error("Index out of bounds.");
                                    }
                                    self.push(v[i as usize].clone());
                                } else {
                                    return self.runtime_error("Index must be an integer.");
                                }
                            }
                            ObjType::HashMap(m) => {
                                match m.get(&key) {
                                    Some(val) => self.push(val.clone()),
                                    None => self.push(Value::Null),
                                }
                            }
                            _ => return self.runtime_error("Can only index vectors and hashmaps."),
                        }
                    } else {
                        return self.runtime_error("Can only index objects.");
                    }
                }
                OpCode::SetIndex => {
                    let value = self.pop();
                    let index = self.pop();
                    let collection = self.pop();
                    
                    let key = self.value_to_string(&index);
                    let idx_int = index.as_int();

                    if let Value::Object(idx) = collection {
                        let obj = self.memory.heap[idx].as_mut().unwrap();
                        match &mut obj.obj_type {
                            ObjType::Vector(v) => {
                                if let Some(i) = idx_int {
                                    if i < 0 || i >= v.len() as i64 {
                                        return self.runtime_error("Index out of bounds.");
                                    }
                                    v[i as usize] = value.clone();
                                    self.push(value);
                                } else {
                                    return self.runtime_error("Index must be an integer.");
                                }
                            }
                            ObjType::HashMap(m) => {
                                m.insert(key, value.clone());
                                self.push(value);
                            }
                            _ => return self.runtime_error("Can only index vectors and hashmaps."),
                        }
                    } else {
                        return self.runtime_error("Can only index objects.");
                    }
                }

                OpCode::Return => {
                    let result = self.pop();
                    let frame = self.frames.pop().unwrap();
                    if self.frames.is_empty() {
                        return InterpretResult::Ok;
                    }
                    
                    self.stack.truncate(frame.slots_offset);
                    self.push(result);
                }
            }
        }
    }

    // --- Internal Helpers ---

    fn value_to_string(&self, value: &Value) -> String {
        match value {
            Value::Object(idx) => {
                if let Some(Some(obj)) = self.memory.heap.get(*idx) {
                    match &obj.obj_type {
                        ObjType::String(s) => s.clone(),
                        ObjType::Vector(v) => {
                            let mut s = "[".to_string();
                            for (i, val) in v.iter().enumerate() {
                                if i > 0 { s += ", "; }
                                s += &self.value_to_string(val);
                            }
                            s += "]";
                            s
                        }
                        ObjType::HashMap(m) => {
                            let mut s = "{".to_string();
                            for (i, (k, v)) in m.iter().enumerate() {
                                if i > 0 { s += ", "; }
                                s += &format!("\"{}\": {}", k, self.value_to_string(v));
                            }
                            s += "}";
                            s
                        }
                        ObjType::Function(f) => format!("<fn {}>", f.name),
                    }
                } else {
                    "null".to_string()
                }
            }
            Value::String(s) => s.clone(),
            _ => value.to_string(),
        }
    }

    fn values_equal(&self, a: Value, b: Value) -> bool {
        match (a, b) {
            (Value::Int(x), Value::Int(y)) => x == y,
            (Value::Float(x), Value::Float(y)) => x == y,
            (Value::Bool(x), Value::Bool(y)) => x == y,
            (Value::Null, Value::Null) => true,
            (Value::Object(idx1), Value::Object(idx2)) => {
                if idx1 == idx2 { return true; }
                let obj1 = self.memory.heap[idx1].as_ref().unwrap();
                let obj2 = self.memory.heap[idx2].as_ref().unwrap();
                match (&obj1.obj_type, &obj2.obj_type) {
                    (ObjType::String(s1), ObjType::String(s2)) => s1 == s2,
                    _ => false,
                }
            }
            _ => false,
        }
    }

    fn call_value(&mut self, arg_count: usize) -> Result<(), String> {
        let callee = self.peek(arg_count).clone();
        match callee {
            Value::NativeFunction(_, func) => {
                let mut args = Vec::new();
                for _ in 0..arg_count {
                    args.push(self.pop());
                }
                args.reverse();
                self.pop();
                
                match func(&self.memory.heap, args) {
                    Ok(result) => { self.push(result); Ok(()) }
                    Err(e) => Err(e),
                }
            }
            Value::ObjFunction(func) => {
                if arg_count != func.arity {
                    return Err(format!("Expected {} arguments but got {}.", func.arity, arg_count));
                }
                if self.frames.len() >= 64 {
                    return Err("Stack overflow".to_string());
                }
                let frame = CallFrame {
                    function: func,
                    ip: 0,
                    slots_offset: self.stack.len() - arg_count - 1,
                };
                self.frames.push(frame);
                Ok(())
            }
            _ => Err(format!("Can only call functions, got {}", callee)),
        }
    }

    fn read_byte(&mut self) -> u8 {
        let frame = self.frames.last_mut().unwrap();
        let byte = frame.function.chunk.code[frame.ip];
        frame.ip += 1;
        byte
    }

    fn read_short(&mut self) -> u16 {
        let frame = self.frames.last_mut().unwrap();
        let code = &frame.function.chunk.code;
        let res = ((code[frame.ip] as u16) << 8) | (code[frame.ip + 1] as u16);
        frame.ip += 2;
        res
    }

    fn read_constant(&mut self) -> Value {
        let index = self.read_byte() as usize;
        self.frames.last().unwrap().function.chunk.constants[index].clone()
    }

    fn read_string_constant(&mut self) -> String {
        let mut constant = self.read_constant();
        if let Value::String(s) = constant {
            let idx = self.memory.alloc(ObjType::String(s));
            constant = Value::Object(idx);
        }
        self.value_to_string(&constant)
    }

    fn binary_op<F>(&mut self, op: F) 
    where F: Fn(f64, f64) -> f64 {
        let b = self.pop();
        let a = self.pop();
        match (a, b) {
            (Value::Int(x), Value::Int(y)) => self.push(Value::Int(op(x as f64, y as f64) as i64)),
            (Value::Float(x), Value::Float(y)) => self.push(Value::Float(op(x, y))),
            _ => { self.runtime_error("Operands must be numbers."); }
        }
    }

    fn runtime_error(&self, message: &str) -> InterpretResult {
        let frame = self.frames.last().unwrap();
        let line = frame.function.chunk.lines[frame.ip - 1];
        eprintln!("\x1b[1;31mruntime error\x1b[0m: {} [line {} in {}]", message, line, frame.function.name);
        InterpretResult::RuntimeError
    }
}
