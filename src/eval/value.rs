use std::fmt;
use std::rc::Rc;
use std::collections::HashMap;
use crate::vm::chunk::Chunk;
use crate::eval::function::LavinaFunction;

// Change: NativeFn now receives the heap
pub type NativeFn = fn(&[Option<Obj>], Vec<Value>) -> Result<Value, String>;

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

#[derive(Debug, PartialEq, Clone)]
pub enum ObjType {
    String(String),
    Vector(Vec<Value>),
    HashMap(HashMap<String, Value>),
    Function(ObjFunction),
}

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

#[derive(Clone, PartialEq, PartialOrd)]
pub enum Value {
    Int(i64),
    Float(f64),
    Bool(bool),
    Null,
    NativeFunction(String, NativeFn),
    String(String), 
    Object(usize), 
    ObjFunction(Rc<ObjFunction>),
    Function(Rc<LavinaFunction>),
}

impl Value {
    pub fn is_truthy(&self) -> bool {
        match self {
            Value::Null => false,
            Value::Bool(b) => *b,
            _ => true,
        }
    }

    pub fn as_int(&self) -> Option<i64> {
        match self {
            Value::Int(i) => Some(*i),
            _ => None,
        }
    }

    pub fn is_string(&self) -> bool {
        matches!(self, Value::String(_)) || matches!(self, Value::Object(_))
    }
}

impl fmt::Display for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Value::Int(i) => write!(f, "{}", i),
            Value::Float(fl) => write!(f, "{:?}", fl),
            Value::Bool(b) => write!(f, "{}", b),
            Value::Null => write!(f, "null"),
            Value::NativeFunction(name, _) => write!(f, "<native fn {}>", name),
            Value::String(s) => write!(f, "\"{}\"", s),
            Value::Object(idx) => write!(f, "<obj {}>", idx),
            Value::ObjFunction(func) => write!(f, "<fn {}>", func.name),
            Value::Function(func) => write!(f, "<fn {}>", func.declaration.name.lexeme),
        }
    }
}

impl fmt::Debug for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self)
    }
}
