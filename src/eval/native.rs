use crate::eval::value::Value;
use std::time::{SystemTime, UNIX_EPOCH};

pub type NativeFn = fn(Vec<Value>) -> Result<Value, String>;

pub fn get_native_functions() -> Vec<(String, NativeFn)> {
    vec![
        ("print".to_string(), native_print),
        ("len".to_string(), native_len),
        ("clock".to_string(), native_clock),
    ]
}

fn native_print(args: Vec<Value>) -> Result<Value, String> {
    let strs: Vec<String> = args.iter().map(|a| a.to_string()).collect();
    println!("{}", strs.join(" "));
    Ok(Value::Null)
}

fn native_len(args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 {
        return Err("len() takes exactly 1 argument".to_string());
    }
    match &args[0] {
        Value::String(s) => Ok(Value::Int(s.len() as i64)),
        Value::Vector(v) => Ok(Value::Int(v.borrow().len() as i64)),
        Value::HashMap(m) => Ok(Value::Int(m.borrow().len() as i64)),
        _ => Err("len() argument must be string, vector or hashmap".to_string()),
    }
}

fn native_clock(_args: Vec<Value>) -> Result<Value, String> {
    let start = SystemTime::now();
    let since_the_epoch = start.duration_since(UNIX_EPOCH)
        .map_err(|e| e.to_string())?;
    Ok(Value::Float(since_the_epoch.as_secs_f64()))
}
