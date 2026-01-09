use std::fmt;
use std::collections::HashMap;
use crate::vm::chunk::Chunk;
use crate::eval::value::Value;

/// Represents a compiled function in the Lavina VM.
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

/// The types of objects that can live on the VM's heap.
#[derive(Debug, PartialEq, Clone)]
pub enum ObjType {
    String(String),
    Vector(Vec<Value>),
    HashMap(HashMap<String, Value>),
    Function(ObjFunction),
}

/// A header for any heap-allocated object.
/// Includes GC metadata.
#[derive(Debug)]
pub struct Obj {
    pub obj_type: ObjType,
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
