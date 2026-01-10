use crate::eval::value::{Value, NativeFn, ObjType, Obj};

pub fn get_native_functions() -> Vec<(String, NativeFn)> {
    vec![
        ("__native_print".to_string(), native_print),
        ("__native_len".to_string(), native_len),
        ("__native_clock".to_string(), native_clock),
        ("__native_type".to_string(), native_type),
        ("__native_sqrt".to_string(), native_sqrt),
        ("__native_abs".to_string(), native_abs),
        ("__native_sin".to_string(), native_sin),
        ("__native_cos".to_string(), native_cos),
        ("__native_tan".to_string(), native_tan),
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
                    ObjType::Class(c) => format!("<class {}>", c.name),
                    ObjType::Instance(i) => {
                        let class_obj = heap[i.class_idx].as_ref().unwrap();
                        if let ObjType::Class(c) = &class_obj.obj_type {
                            format!("<instance of {}>", c.name)
                        } else {
                            "<instance of unknown>".to_string()
                        }
                    },
                    ObjType::BoundMethod(b) => {
                        let method_obj = heap[b.method].as_ref().unwrap();
                        if let ObjType::Closure(c) = &method_obj.obj_type {
                            let f_obj = heap[c.function_idx].as_ref().unwrap();
                            if let ObjType::Function(f) = &f_obj.obj_type {
                                return format!("<bound method {}>", f.name);
                            }
                        }
                        "<bound method unknown>".to_string()
                    },
                    ObjType::Closure(c) => {
                        if let Some(Some(f_obj)) = heap.get(c.function_idx) {
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
        Value::Object(idx) => {
            if let Some(Some(obj)) = _heap.get(*idx) {
                match &obj.obj_type {
                    ObjType::Class(_) => "class",
                    ObjType::Instance(_) => "instance",
                    ObjType::BoundMethod(_) => "method",
                    _ => "object",
                }
            } else {
                "object"
            }
        },
        Value::TemplateFunction(_) => "template function",
        Value::Function(_) => "function",
    };
    Ok(Value::String(type_name.to_string()))
}

fn native_sqrt(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("sqrt() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).sqrt())),
        Value::Float(f) => Ok(Value::Float(f.sqrt())),
        _ => Err("sqrt() expects number.".to_string()),
    }
}

fn native_abs(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("abs() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Int(i.abs())),
        Value::Float(f) => Ok(Value::Float(f.abs())),
        _ => Err("abs() expects number.".to_string()),
    }
}

fn native_sin(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("sin() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).sin())),
        Value::Float(f) => Ok(Value::Float(f.sin())),
        _ => Err("sin() expects number.".to_string()),
    }
}

fn native_cos(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("cos() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).cos())),
        Value::Float(f) => Ok(Value::Float(f.cos())),
        _ => Err("cos() expects number.".to_string()),
    }
}

fn native_tan(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("tan() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).tan())),
        Value::Float(f) => Ok(Value::Float(f.tan())),
        _ => Err("tan() expects number.".to_string()),
    }
}
