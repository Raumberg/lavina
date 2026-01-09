use crate::eval::value::Value;
use std::time::{SystemTime, UNIX_EPOCH};

pub type NativeFn = fn(Vec<Value>) -> Result<Value, String>;

pub fn get_native_functions() -> Vec<(String, NativeFn)> {
    vec![
        ("print".to_string(), native_print),
        ("len".to_string(), native_len),
        ("clock".to_string(), native_clock),
        ("type".to_string(), native_type),
    ]
}

fn native_print(args: Vec<Value>) -> Result<Value, String> {
    for val in args {
        print!("{} ", val);
    }
    println!();
    Ok(Value::Null)
}

fn native_len(args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 {
        return Err("len() expects exactly 1 argument.".to_string());
    }
    match &args[0] {
        Value::String(s) => Ok(Value::Int(s.len() as i64)),
        Value::Vector(v) => Ok(Value::Int(v.borrow().len() as i64)),
        Value::HashMap(m) => Ok(Value::Int(m.borrow().len() as i64)),
        _ => Err("len() argument must be a string, vector, or hashmap.".to_string()),
    }
}

fn native_clock(_args: Vec<Value>) -> Result<Value, String> {
    let start = SystemTime::now();
    let since_the_epoch = start.duration_since(UNIX_EPOCH)
        .expect("Time went backwards");
    Ok(Value::Float(since_the_epoch.as_secs_f64()))
}

fn native_type(args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 {
        return Err("type() expects exactly 1 argument.".to_string());
    }
    let type_name = match &args[0] {
        Value::Int(_) => "int",
        Value::Float(_) => "float",
        Value::String(_) => "string",
        Value::Bool(_) => "bool",
        Value::Null => "null",
        Value::NativeFunction(_, _) => "native function",
        Value::Function(_) => "function",
        Value::ObjFunction(_) => "vm function",
        Value::Vector(_) => "vector",
        Value::HashMap(_) => "hashmap",
    };
    Ok(Value::String(type_name.to_string()))
}
