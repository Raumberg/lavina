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

/// Represents a captured variable in a closure.
#[derive(Debug, Clone, PartialEq)]
pub enum Upvalue {
    Open(usize),  // Index into the VM stack
    Closed(Value), // Value moved to the heap
}

/// A closure wraps a function with its captured environment.
#[derive(Debug, Clone, PartialEq)]
pub struct ObjClosure {
    pub function_idx: usize, // Index of ObjFunction in the heap
    pub upvalues: Vec<usize>, // Indices of ObjUpvalue in the heap
}

impl PartialOrd for ObjClosure {
    fn partial_cmp(&self, _other: &Self) -> Option<std::cmp::Ordering> {
        None // Not really comparable
    }
}

/// Represents a class definition.
#[derive(Debug, Clone, PartialEq)]
pub struct ObjClass {
    pub name: String,
    pub methods: HashMap<String, Value>, // Value should be a Closure
}

/// Represents an instance of a class.
#[derive(Debug, Clone, PartialEq)]
pub struct ObjInstance {
    pub class_idx: usize,
    pub fields: HashMap<String, Value>,
}

/// Represents a method bound to an instance or primitive.
#[derive(Debug, Clone, PartialEq)]
pub struct ObjBoundMethod {
    pub receiver: Value, // Can be an instance (Value::Object(idx)) or a primitive
    pub method: usize,   // Index of the closure
}

/// The types of objects that can live on the VM's heap.
#[derive(Debug, PartialEq, Clone)]
pub enum ObjType {
    String(String),
    Vector(Vec<Value>),
    HashMap(HashMap<String, Value>),
    Function(ObjFunction),
    Closure(ObjClosure),
    Upvalue(Upvalue),
    Namespace(String, HashMap<String, Value>),
    Class(ObjClass),
    Instance(ObjInstance),
    BoundMethod(ObjBoundMethod),
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
