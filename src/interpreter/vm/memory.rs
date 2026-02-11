use crate::interpreter::vm::object::{Obj, ObjType};
use crate::interpreter::value::Value;
use std::collections::HashMap;

/// Manages heap memory and garbage collection.
pub struct Memory {
    pub heap: Vec<Option<Obj>>,
    pub bytes_allocated: usize,
    pub next_gc: usize,
}

impl Memory {
    pub fn new() -> Self {
        Self {
            heap: Vec::with_capacity(1024),
            bytes_allocated: 0,
            next_gc: 1024 * 1024, // 1MB threshold for example
        }
    }

    /// Allocates a new object on the heap and returns its index.
    pub fn alloc(&mut self, obj_type: ObjType) -> usize {
        // Simple size estimation
        let size = std::mem::size_of::<Obj>(); 
        self.bytes_allocated += size;

        let obj = Obj::new(obj_type);
        
        for (i, slot) in self.heap.iter_mut().enumerate() {
            if slot.is_none() {
                *slot = Some(obj);
                return i;
            }
        }

        self.heap.push(Some(obj));
        self.heap.len() - 1
    }

    /// Mark-and-Sweep entry point.
    pub fn collect_garbage(&mut self, roots: &[Value], globals: &HashMap<String, Value>, open_upvalues: &[usize]) {
        #[cfg(debug_assertions)]
        println!("-- GC: mark roots --");

        // 1. Mark roots (stack, globals, upvalues, etc.)
        let mut worklist = Vec::new();
        for root in roots {
            self.mark_value(root, &mut worklist);
        }
        for val in globals.values() {
            self.mark_value(val, &mut worklist);
        }
        for uv_idx in open_upvalues {
            self.mark_object(*uv_idx, &mut worklist);
        }

        // 2. Trace references
        while let Some(idx) = worklist.pop() {
            self.blacken_object(idx, &mut worklist);
        }

        // 3. Sweep
        #[cfg(debug_assertions)]
        println!("-- GC: sweep --");
        self.sweep();
    }

    fn mark_value(&mut self, value: &Value, worklist: &mut Vec<usize>) {
        match value {
            Value::Object(idx) => self.mark_object(*idx, worklist),
            Value::TemplateFunction(func) => {
                for val in &func.chunk.constants {
                    self.mark_value(val, worklist);
                }
            }
            _ => {}
        }
    }

    fn mark_object(&mut self, idx: usize, worklist: &mut Vec<usize>) {
        if let Some(Some(obj)) = self.heap.get_mut(idx) {
            if obj.is_marked { return; }
            obj.is_marked = true;
            worklist.push(idx);
        }
    }

    fn blacken_object(&mut self, idx: usize, worklist: &mut Vec<usize>) {
        // We use a trick here: we take the object out, process it, and put it back
        // to satisfy the borrow checker without cloning large data structures.
        let mut obj = self.heap[idx].take().unwrap();
        
        match &mut obj.obj_type {
            ObjType::Vector(elements) => {
                for val in elements {
                    self.mark_value(val, worklist);
                }
            }
            ObjType::HashMap(map) => {
                for val in map.values() {
                    self.mark_value(val, worklist);
                }
            }
            ObjType::Namespace(_, members) => {
                for val in members.values() {
                    self.mark_value(val, worklist);
                }
            }
            ObjType::Function(func) => {
                for val in &func.chunk.constants {
                    self.mark_value(val, worklist);
                }
            }
            ObjType::Closure(closure) => {
                self.mark_object(closure.function_idx, worklist);
                for uv_idx in &closure.upvalues {
                    self.mark_object(*uv_idx, worklist);
                }
            }
            ObjType::Upvalue(uv) => {
                if let crate::interpreter::vm::object::Upvalue::Closed(val) = uv {
                    self.mark_value(val, worklist);
                }
            }
            ObjType::Class(class) => {
                for val in class.methods.values() {
                    self.mark_value(val, worklist);
                }
            }
            ObjType::Instance(instance) => {
                self.mark_object(instance.class_idx, worklist);
                for val in instance.fields.values() {
                    self.mark_value(val, worklist);
                }
            }
            ObjType::BoundMethod(bound) => {
                self.mark_value(&bound.receiver, worklist);
                self.mark_object(bound.method, worklist);
            }
            _ => {}
        }

        self.heap[idx] = Some(obj);
    }

    fn sweep(&mut self) {
        for i in 0..self.heap.len() {
            if let Some(obj) = &mut self.heap[i] {
                if obj.is_marked {
                    obj.is_marked = false; // Reset for next cycle
                } else {
                    self.heap[i] = None;
                }
            }
        }
    }
}
