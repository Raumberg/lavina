use crate::vm::opcode::OpCode;
use crate::eval::value::{Value, ObjFunction};
use crate::eval::native::get_native_functions;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

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

pub struct VM {
    frames: Vec<CallFrame>,
    stack: Vec<Value>,
    globals: HashMap<String, Value>,
}

impl VM {
    pub fn new() -> Self {
        let mut vm = Self {
            frames: Vec::with_capacity(64),
            stack: Vec::with_capacity(256),
            globals: HashMap::new(),
        };
        
        for (name, func) in get_native_functions() {
            vm.globals.insert(name.clone(), Value::NativeFunction(name, func));
        }
        
        vm
    }

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

    fn run(&mut self) -> InterpretResult {
        loop {
            #[cfg(debug_assertions)]
            {
                print!("          ");
                for value in &self.stack {
                    print!("[ {} ]", value);
                }
                println!();
                let frame = self.frames.last().unwrap();
                frame.function.chunk.disassemble_instruction(frame.ip);
            }

            let instruction = self.read_byte();
            match OpCode::from(instruction) {
                OpCode::Constant => {
                    let constant = self.read_constant();
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
                    self.push(Value::Bool(a == b));
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
                        (Value::String(x), Value::String(y)) => self.push(Value::String(x + &y)),
                        _ => return self.runtime_error("Operands must be two numbers or two strings."),
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
                    self.push(Value::Vector(Rc::new(RefCell::new(elements))));
                }
                OpCode::HashMap => {
                    let count = self.read_byte() as usize;
                    let mut map = std::collections::HashMap::with_capacity(count);
                    for _ in 0..count {
                        let value = self.pop();
                        let key = self.pop();
                        map.insert(key.to_string(), value);
                    }
                    self.push(Value::HashMap(Rc::new(RefCell::new(map))));
                }
                OpCode::GetIndex => {
                    let index = self.pop();
                    let collection = self.pop();
                    match collection {
                        Value::Vector(v) => {
                            if let Some(idx) = index.as_int() {
                                let v = v.borrow();
                                if idx < 0 || idx >= v.len() as i64 {
                                    return self.runtime_error("Vector index out of bounds.");
                                }
                                self.push(v[idx as usize].clone());
                            } else {
                                return self.runtime_error("Vector index must be an integer.");
                            }
                        }
                        Value::HashMap(m) => {
                            let m = m.borrow();
                            let key = index.to_string();
                            // In Lavina maps, keys are converted to strings for representation
                            // but we can make it better later.
                            match m.get(&key) {
                                Some(val) => self.push(val.clone()),
                                None => self.push(Value::Null),
                            }
                        }
                        _ => return self.runtime_error("Can only index vectors and hashmaps."),
                    }
                }
                OpCode::SetIndex => {
                    let value = self.pop();
                    let index = self.pop();
                    let collection = self.pop();
                    match collection {
                        Value::Vector(v) => {
                            if let Some(idx) = index.as_int() {
                                let mut v = v.borrow_mut();
                                if idx < 0 || idx >= v.len() as i64 {
                                    return self.runtime_error("Vector index out of bounds.");
                                }
                                v[idx as usize] = value.clone();
                                self.push(value);
                            } else {
                                return self.runtime_error("Vector index must be an integer.");
                            }
                        }
                        Value::HashMap(m) => {
                            let mut m = m.borrow_mut();
                            let key = index.to_string();
                            m.insert(key, value.clone());
                            self.push(value);
                        }
                        _ => return self.runtime_error("Can only index vectors and hashmaps."),
                    }
                }
                OpCode::Return => {
                    let result = self.pop();
                    let frame = self.frames.pop().unwrap();
                    if self.frames.is_empty() {
                        self.pop();
                        return InterpretResult::Ok;
                    }
                    
                    self.stack.truncate(frame.slots_offset);
                    self.push(result);
                }
            }
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
                
                match func(args) {
                    Ok(result) => {
                        self.push(result);
                        Ok(())
                    }
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
            _ => Err(format!("Can only call functions and classes, got {}", callee)),
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
        match self.read_constant() {
            Value::String(s) => s,
            _ => panic!("Expected string constant"),
        }
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
