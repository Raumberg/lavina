use std::fmt;
use std::collections::HashMap;
use crate::interpreter::vm::chunk::Chunk;
use crate::interpreter::value::Value;

/// Represents the types of objects that can live on the Lavina Heap.
#[derive(Debug, PartialEq, Clone)]
pub enum ObjType {
    /// A basic UTF-8 string.
    String(String),
    /// A dynamic array of Lavina values.
    Vector(Vec<Value>),
    /// A key-value store (keys are currently strings).
    HashMap(HashMap<String, Value>),
    /// A compiled user-defined function.
    Function(ObjFunction),
}

/// A container for a heap-allocated object with GC metadata.
#[derive(Debug)]
pub struct Obj {
    pub obj_type: ObjType,
    /// Metadata for the Mark-and-Sweep garbage collector.
    pub is_marked: bool,
}

impl Obj {
    pub fn new(obj_type: ObjType) -> Self {
        Self {
            obj_type,
            is_marked: false,
        }
    }
}

/// A compiled function object containing its bytecode and metadata.
#[derive(Debug, Clone, PartialEq)]
pub struct ObjFunction {
    pub arity: usize,
    pub chunk: Chunk,
    pub name: String,
}

impl PartialOrd for ObjFunction {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.name.partial_cmp(&other.name)
    }
}
