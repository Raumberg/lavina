use std::fmt;
use std::rc::Rc;
use crate::eval::function::LavinaFunction;
pub use crate::vm::object::{ObjFunction, ObjType, Obj};

/// A callback for native functions.
/// It takes a reference to the heap and the arguments.
pub type NativeFn = fn(&[Option<Obj>], Vec<Value>) -> Result<Value, String>;

#[derive(Clone, PartialOrd)]
pub enum Value {
    Int(i64),
    Float(f64),
    Bool(bool),
    Null,
    #[allow(unpredictable_function_pointer_comparisons)]
    NativeFunction(String, NativeFn),
    String(String), // Used for constants
    Object(usize),  // Index into the GC heap
    TemplateFunction(Box<ObjFunction>), // Template for creating closures
    Function(Rc<LavinaFunction>), // Tree-walker compatibility
}

impl PartialEq for Value {
    fn eq(&self, other: &Self) -> bool {
        match (self, other) {
            (Value::Int(a), Value::Int(b)) => a == b,
            (Value::Float(a), Value::Float(b)) => a == b,
            (Value::Bool(a), Value::Bool(b)) => a == b,
            (Value::Null, Value::Null) => true,
            (Value::String(a), Value::String(b)) => a == b,
            (Value::Object(a), Value::Object(b)) => a == b,
            (Value::NativeFunction(a, _), Value::NativeFunction(b, _)) => a == b,
            _ => false,
        }
    }
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
            Value::TemplateFunction(func) => write!(f, "<fn template {}>", func.name),
            Value::Function(func) => write!(f, "<fn {}>", func.declaration.name.lexeme),
        }
    }
}

impl fmt::Debug for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self)
    }
}
