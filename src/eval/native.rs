use crate::eval::value::{Value, NativeFn, ObjType, Obj};

pub fn get_native_functions() -> Vec<(String, NativeFn)> {
    vec![
        ("print".to_string(), native_print),
        ("len".to_string(), native_len),
        ("clock".to_string(), native_clock),
        ("type".to_string(), native_type),
    ]
}

fn value_to_string(heap: &[Option<Obj>], value: &Value) -> String {
    match value {
        Value::Object(idx) => {
            if let Some(Some(obj)) = heap.get(*idx) {
                match &obj.obj_type {
                    ObjType::String(s) => s.clone(),
                    ObjType::Vector(v) => {
                        let mut s = "[".to_string();
                        for (i, val) in v.iter().enumerate() {
                            if i > 0 { s += ", "; }
                            s += &value_to_string(heap, val);
                        }
                        s += "]";
                        s
                    }
                    ObjType::HashMap(m) => {
                        let mut s = "{".to_string();
                        for (i, (k, v)) in m.iter().enumerate() {
                            if i > 0 { s += ", "; }
                            s += &format!("\"{}\": {}", k, value_to_string(heap, v));
                        }
                        s += "}";
                        s
                    }
                    ObjType::Namespace(name, _) => format!("<namespace {}>", name),
                    ObjType::Function(f) => format!("<fn {}>", f.name),
                }
            } else {
                "null".to_string()
            }
        }
        Value::String(s) => s.clone(),
        _ => value.to_string(),
    }
}

fn native_print(heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    for (i, arg) in args.iter().enumerate() {
        if i > 0 { print!(" "); }
        print!("{}", value_to_string(heap, arg));
    }
    println!();
    Ok(Value::Null)
}

fn native_len(heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 {
        return Err("len() expects exactly 1 argument.".to_string());
    }
    match &args[0] {
        Value::String(s) => Ok(Value::Int(s.len() as i64)),
        Value::Object(idx) => {
            if let Some(Some(obj)) = heap.get(*idx) {
                match &obj.obj_type {
                    ObjType::String(s) => Ok(Value::Int(s.len() as i64)),
                    ObjType::Vector(v) => Ok(Value::Int(v.len() as i64)),
                    ObjType::HashMap(m) => Ok(Value::Int(m.len() as i64)),
                    _ => Err("Object has no length.".to_string()),
                }
            } else {
                Err("Object not found in heap.".to_string())
            }
        }
        _ => Err("len() expects string, vector or hashmap.".to_string()),
    }
}

fn native_clock(_heap: &[Option<Obj>], _args: Vec<Value>) -> Result<Value, String> {
    use std::time::{SystemTime, UNIX_EPOCH};
    let start = SystemTime::now();
    let since_the_epoch = start.duration_since(UNIX_EPOCH).expect("Time went backwards");
    Ok(Value::Float(since_the_epoch.as_secs_f64()))
}

fn native_type(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
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
        Value::Object(_) => "object",
        Value::ObjFunction(_) => "vm function",
        Value::Function(_) => "function",
    };
    Ok(Value::String(type_name.to_string()))
}
