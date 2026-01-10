use crate::vm::opcode::OpCode;
use crate::vm::object::{ObjFunction, ObjType};
use crate::vm::memory::Memory;
use crate::vm::frame::CallFrame;
use crate::eval::value::{Value};
use crate::eval::native::get_native_functions;
use std::collections::HashMap;

#[derive(Debug, PartialEq)]
pub enum InterpretResult {
    Ok,
    CompileError,
    RuntimeError,
}

pub struct VM {
    frames: Vec<CallFrame>,
    stack: Vec<Value>,
    globals: HashMap<String, Value>,
    memory: Memory,
    open_upvalues: Vec<usize>, // Indices of ObjUpvalue in the heap
}

impl VM {
    pub fn new() -> Self {
        let mut vm = Self {
            frames: Vec::with_capacity(64),
            stack: Vec::with_capacity(256),
            globals: HashMap::new(),
            memory: Memory::new(),
            open_upvalues: Vec::new(),
        };
        
        for (name, func) in get_native_functions() {
            vm.globals.insert(name.clone(), Value::NativeFunction(name, func));
        }
        
        vm
    }

    pub fn interpret(&mut self, function: ObjFunction) -> InterpretResult {
        self.stack.clear();
        self.open_upvalues.clear();
        
        let mut function = function;
        // Transform string constants into heap objects once
        for i in 0..function.chunk.constants.len() {
            if let Value::String(s) = &function.chunk.constants[i] {
                let idx = self.memory.alloc(ObjType::String(s.clone()));
                function.chunk.constants[i] = Value::Object(idx);
            }
        }

        let function_idx = self.memory.alloc(ObjType::Function(function));
        let closure = crate::vm::object::ObjClosure {
            function_idx,
            upvalues: Vec::new(),
        };
        let closure_idx = self.memory.alloc(ObjType::Closure(closure));
        
        self.push(Value::Object(closure_idx));
        let frame = CallFrame {
            closure_idx,
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
            if self.memory.bytes_allocated > self.memory.next_gc {
                self.memory.collect_garbage(&self.stack, &self.globals, &self.open_upvalues);
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
                            ObjType::Namespace(_, members) => {
                                match members.get(&key) {
                                    Some(val) => self.push(val.clone()),
                                    None => self.push(Value::Null),
                                }
                            }
                            _ => return self.runtime_error("Can only index vectors, hashmaps and namespaces."),
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
                            ObjType::Namespace(_, members) => {
                                members.insert(key, value.clone());
                                self.push(value);
                            }
                            _ => return self.runtime_error("Can only index vectors, hashmaps and namespaces."),
                        }
                    } else {
                        return self.runtime_error("Can only index objects.");
                    }
                }

                OpCode::Namespace => {
                    let name = self.read_string_constant();
                    // In a real implementation, we would collect defined members.
                    // For now, let's create a Namespace from a HashMap.
                    let val = self.pop();
                    if let Value::Object(idx) = val {
                        // This is a hack: we assume the namespace "init" returned a HashMap
                        // of its members. We convert it to a Namespace object.
                        let obj_type = self.memory.heap[idx].as_ref().unwrap().obj_type.clone();
                        if let ObjType::HashMap(members) = obj_type {
                            let ns_idx = self.memory.alloc(ObjType::Namespace(name, members));
                            self.push(Value::Object(ns_idx));
                        } else {
                            // If it's not a hashmap, maybe it's just null? 
                            // Create empty namespace.
                            let ns_idx = self.memory.alloc(ObjType::Namespace(name, HashMap::new()));
                            self.push(Value::Object(ns_idx));
                        }
                    } else {
                        let ns_idx = self.memory.alloc(ObjType::Namespace(name, HashMap::new()));
                        self.push(Value::Object(ns_idx));
                    }
                }

                OpCode::Closure => {
                    let function_val = self.read_constant();
                    if let Value::TemplateFunction(f_template) = function_val {
                        let upvalue_count = f_template.chunk.upvalues.len();
                        
                        // Allocate function on the heap
                        let f_idx = self.memory.alloc(ObjType::Function(*f_template));
                        
                        let mut upvalues = Vec::with_capacity(upvalue_count);
                        let function_chunk = {
                            let obj = self.memory.heap[f_idx].as_ref().unwrap();
                            if let ObjType::Function(f) = &obj.obj_type {
                                f.chunk.clone()
                            } else { panic!("Not a function") }
                        };

                        for i in 0..upvalue_count {
                            let loc = function_chunk.upvalues[i];
                            if loc.is_local {
                                let offset = self.frames.last().unwrap().slots_offset;
                                upvalues.push(self.capture_upvalue(offset + loc.index as usize));
                            } else {
                                let parent_closure_idx = self.frames.last().unwrap().closure_idx;
                                let parent_uv_idx = {
                                    let obj = self.memory.heap[parent_closure_idx].as_ref().unwrap();
                                    if let ObjType::Closure(c) = &obj.obj_type {
                                        c.upvalues[loc.index as usize]
                                    } else { panic!("Not a closure") }
                                };
                                upvalues.push(parent_uv_idx);
                            }
                        }

                        let closure = crate::vm::object::ObjClosure {
                            function_idx: f_idx,
                            upvalues,
                        };
                        let closure_idx = self.memory.alloc(ObjType::Closure(closure));
                        self.push(Value::Object(closure_idx));
                    } else {
                        panic!("VM: OP_CLOSURE expects function template, got {}", function_val);
                    }
                }
                OpCode::GetUpvalue => {
                    let slot = self.read_byte() as usize;
                    let closure_idx = self.frames.last().unwrap().closure_idx;
                    let uv_idx = {
                        let obj = self.memory.heap[closure_idx].as_ref().unwrap();
                        if let ObjType::Closure(closure) = &obj.obj_type {
                            closure.upvalues[slot]
                        } else { panic!("Not a closure") }
                    };

                    let val = {
                        let uv_obj = self.memory.heap[uv_idx].as_ref().unwrap();
                        if let ObjType::Upvalue(uv) = &uv_obj.obj_type {
                            match uv {
                                crate::vm::object::Upvalue::Open(pos) => self.stack[*pos].clone(),
                                crate::vm::object::Upvalue::Closed(val) => val.clone(),
                            }
                        } else { panic!("Not an upvalue") }
                    };
                    self.push(val);
                }
                OpCode::SetUpvalue => {
                    let slot = self.read_byte() as usize;
                    let val = self.peek(0).clone();
                    let closure_idx = self.frames.last().unwrap().closure_idx;
                    let uv_idx = {
                        let obj = self.memory.heap[closure_idx].as_ref().unwrap();
                        if let ObjType::Closure(closure) = &obj.obj_type {
                            closure.upvalues[slot]
                        } else { panic!("Not a closure") }
                    };

                    let uv_obj = self.memory.heap[uv_idx].as_mut().unwrap();
                    if let ObjType::Upvalue(uv) = &mut uv_obj.obj_type {
                        match uv {
                            crate::vm::object::Upvalue::Open(pos) => self.stack[*pos] = val,
                            crate::vm::object::Upvalue::Closed(v) => *v = val,
                        }
                    } else { panic!("Not an upvalue") }
                }
                OpCode::CloseUpvalue => {
                    self.close_upvalues(self.stack.len() - 1);
                    self.pop();
                }

                OpCode::Return => {
                    let frame = self.frames.pop().unwrap();
                    
                    // Close upvalues owned by this frame before we mess with the stack
                    self.close_upvalues(frame.slots_offset);

                    if self.frames.is_empty() {
                        return InterpretResult::Ok;
                    }
                    
                    let result = self.pop();
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
                        ObjType::Namespace(name, _) => format!("<namespace {}>", name),
                        ObjType::Function(f) => format!("<fn {}>", f.name),
                        ObjType::Closure(c) => {
                            if let Some(Some(f_obj)) = self.memory.heap.get(c.function_idx) {
                                if let ObjType::Function(f) = &f_obj.obj_type {
                                    return format!("<fn {}>", f.name);
                                }
                            }
                            "<fn unknown>".to_string()
                        }
                        ObjType::Upvalue(_) => "<upvalue>".to_string(),
                    }
                } else {
                    "null".to_string()
                }
            }
            Value::String(s) => s.clone(),
            _ => value.to_string(),
        }
    }

    fn capture_upvalue(&mut self, local_idx: usize) -> usize {
        for uv_idx in &self.open_upvalues {
            if let Some(Some(obj)) = self.memory.heap.get(*uv_idx) {
                if let ObjType::Upvalue(crate::vm::object::Upvalue::Open(pos)) = &obj.obj_type {
                    if *pos == local_idx {
                        return *uv_idx;
                    }
                }
            }
        }

        let uv_idx = self.memory.alloc(ObjType::Upvalue(crate::vm::object::Upvalue::Open(local_idx)));
        self.open_upvalues.push(uv_idx);
        uv_idx
    }

    fn close_upvalues(&mut self, last_slot: usize) {
        let mut i = 0;
        while i < self.open_upvalues.len() {
            let uv_idx = self.open_upvalues[i];
            let mut close = false;
            let mut val_to_store = None;

            if let Some(Some(obj)) = self.memory.heap.get(uv_idx) {
                if let ObjType::Upvalue(crate::vm::object::Upvalue::Open(pos)) = &obj.obj_type {
                    if *pos >= last_slot {
                        close = true;
                        val_to_store = Some(self.stack[*pos].clone());
                    }
                }
            }

            if close {
                self.open_upvalues.remove(i);
                let obj = self.memory.heap[uv_idx].as_mut().unwrap();
                obj.obj_type = ObjType::Upvalue(crate::vm::object::Upvalue::Closed(val_to_store.unwrap()));
            } else {
                i += 1;
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
                self.pop(); // Callee
                
                match func(&self.memory.heap, args) {
                    Ok(result) => { self.push(result); Ok(()) }
                    Err(e) => Err(e),
                }
            }
            Value::Object(idx) => {
                let obj = self.memory.heap[idx].as_ref().ok_or("Invalid object")?;
                if let ObjType::Closure(closure) = &obj.obj_type {
                    let f_obj = self.memory.heap[closure.function_idx].as_ref().ok_or("Invalid function index")?;
                    if let ObjType::Function(func) = &f_obj.obj_type {
                        if arg_count != func.arity {
                            return Err(format!("Expected {} arguments but got {}.", func.arity, arg_count));
                        }
                        if self.frames.len() >= 64 {
                            return Err("Stack overflow".to_string());
                        }
                        let frame = CallFrame {
                            closure_idx: idx,
                            ip: 0,
                            slots_offset: self.stack.len() - arg_count - 1,
                        };
                        self.frames.push(frame);
                        return Ok(());
                    }
                }
                Err(format!("Can only call closures, got {}", callee))
            }
            _ => Err(format!("Can only call functions, got {}", callee)),
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

    fn read_byte(&mut self) -> u8 {
        let frame = self.frames.last_mut().unwrap();
        let closure_idx = frame.closure_idx;
        let ip = frame.ip;
        
        // Safety: we trust the closure index is valid
        if let Some(Some(obj)) = self.memory.heap.get(closure_idx) {
            if let ObjType::Closure(closure) = &obj.obj_type {
                if let Some(Some(f_obj)) = self.memory.heap.get(closure.function_idx) {
                    if let ObjType::Function(func) = &f_obj.obj_type {
                        let byte = func.chunk.code[ip];
                        self.frames.last_mut().unwrap().ip += 1;
                        return byte;
                    }
                }
            }
        }
        panic!("VM: could not read byte from function");
    }

    fn read_short(&mut self) -> u16 {
        let b1 = self.read_byte() as u16;
        let b2 = self.read_byte() as u16;
        (b1 << 8) | b2
    }

    fn read_constant(&mut self) -> Value {
        let index = self.read_byte() as usize;
        let closure_idx = self.frames.last().unwrap().closure_idx;
        
        if let Some(Some(obj)) = self.memory.heap.get(closure_idx) {
            if let ObjType::Closure(closure) = &obj.obj_type {
                if let Some(Some(f_obj)) = self.memory.heap.get(closure.function_idx) {
                    if let ObjType::Function(func) = &f_obj.obj_type {
                        return func.chunk.constants[index].clone();
                    }
                }
            }
        }
        panic!("VM: could not read constant");
    }

    fn read_string_constant(&mut self) -> String {
        let constant = self.read_constant();
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
        let closure_idx = frame.closure_idx;
        let mut line = 0;
        let mut name = "unknown".to_string();

        if let Some(Some(obj)) = self.memory.heap.get(closure_idx) {
            if let ObjType::Closure(closure) = &obj.obj_type {
                if let Some(Some(f_obj)) = self.memory.heap.get(closure.function_idx) {
                    if let ObjType::Function(func) = &f_obj.obj_type {
                        line = func.chunk.lines[frame.ip - 1];
                        name = func.name.clone();
                    }
                }
            }
        }

        eprintln!("\x1b[1;31mruntime error\x1b[0m: {} [line {} in {}]", message, line, name);
        InterpretResult::RuntimeError
    }
}
