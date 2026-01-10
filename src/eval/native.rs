use crate::eval::value::{Value, NativeFn, ObjType, Obj};

pub fn get_native_functions() -> Vec<(String, NativeFn)> {
    vec![
        ("__native_print".to_string(), native_print),
        ("__native_len".to_string(), native_len),
        ("__native_clock".to_string(), native_clock),
        ("__native_type".to_string(), native_type),
        ("__native_typeof".to_string(), native_type),
        ("__native_cast".to_string(), native_cast),
        ("__native_sqrt".to_string(), native_sqrt),
        ("__native_abs".to_string(), native_abs),
        ("__native_sin".to_string(), native_sin),
        ("__native_cos".to_string(), native_cos),
        ("__native_tan".to_string(), native_tan),
        ("__native_input".to_string(), native_input),
        ("__native_string_lower".to_string(), native_string_lower),
        ("__native_string_upper".to_string(), native_string_upper),
        ("__native_string_len".to_string(), native_string_len),
        ("print".to_string(), native_print),
        ("len".to_string(), native_len),
        ("clock".to_string(), native_clock),
        ("typeof".to_string(), native_type),
        ("cast".to_string(), native_cast),
        ("input".to_string(), native_input),
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
    if args.is_empty() {
        println!();
        return Ok(Value::Null);
    }

    let first_arg_str = value_to_string(heap, &args[0]);
    
    // Check for format placeholders {}
    if args.len() > 1 && first_arg_str.contains("{}") {
        let mut result = String::new();
        let mut arg_idx = 1;
        let parts: Vec<&str> = first_arg_str.split("{}").collect();
        
        for (i, part) in parts.iter().enumerate() {
            result.push_str(part);
            if i < parts.len() - 1 && arg_idx < args.len() {
                result.push_str(&value_to_string(heap, &args[arg_idx]));
                arg_idx += 1;
            }
        }
        println!("{}", result);
    } else {
        // Normal print: space-separated
        for (i, arg) in args.iter().enumerate() {
            if i > 0 { print!(" "); }
            print!("{}", value_to_string(heap, arg));
        }
        println!();
    }
    
    Ok(Value::Null)
}

fn native_cast(heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 {
        return Err("cast() expects 2 arguments: value and target type string.".to_string());
    }
    
    let target_type = match &args[1] {
        Value::String(s) => s.as_str(),
        Value::Object(idx) => {
            if let Some(Some(obj)) = heap.get(*idx) {
                if let ObjType::String(s) = &obj.obj_type {
                    s.as_str()
                } else { return Err("Target type must be a string.".to_string()); }
            } else { return Err("Target type must be a string.".to_string()); }
        }
        _ => return Err("Target type must be a string.".to_string()),
    };

    match target_type {
        "int" => match &args[0] {
            Value::Int(i) => Ok(Value::Int(*i)),
            Value::Float(f) => Ok(Value::Int(*f as i64)),
            Value::String(s) => s.parse::<i64>().map(Value::Int).map_err(|e| e.to_string()),
            Value::Object(idx) => {
                if let Some(Some(obj)) = heap.get(*idx) {
                    if let ObjType::String(s) = &obj.obj_type {
                        s.parse::<i64>().map(Value::Int).map_err(|e| e.to_string())
                    } else { Err("Cannot cast object to int.".to_string()) }
                } else { Err("Cannot cast object to int.".to_string()) }
            }
            _ => Err(format!("Cannot cast {} to int.", args[0])),
        },
        "float" => match &args[0] {
            Value::Int(i) => Ok(Value::Float(*i as f64)),
            Value::Float(f) => Ok(Value::Float(*f)),
            Value::String(s) => s.parse::<f64>().map(Value::Float).map_err(|e| e.to_string()),
            _ => Err(format!("Cannot cast {} to float.", args[0])),
        },
        "string" => Ok(Value::String(value_to_string(heap, &args[0]))),
        "bool" => Ok(Value::Bool(args[0].is_truthy())),
        _ => Err(format!("Unknown target type for cast: {}", target_type)),
    }
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
                    ObjType::String(_) => "string",
                    ObjType::Vector(_) => "vector",
                    ObjType::HashMap(_) => "hashmap",
                    ObjType::Class(_) => "class",
                    ObjType::Instance(_) => "instance",
                    ObjType::BoundMethod(_) => "method",
                    ObjType::Function(_) => "function",
                    ObjType::Closure(_) => "function",
                    ObjType::Namespace(_, _) => "namespace",
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

fn native_input(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    use std::io::{self, Write};
    if !args.is_empty() {
        print!("{}", value_to_string(_heap, &args[0]));
        io::stdout().flush().map_err(|e| e.to_string())?;
    }
    let mut input = String::new();
    io::stdin().read_line(&mut input).map_err(|e| e.to_string())?;
    Ok(Value::String(input.trim_end().to_string()))
}

fn native_string_lower(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("lower() expects 0 arguments (plus 'this').".to_string()); }
    let s = value_to_string(_heap, &args[0]);
    Ok(Value::String(s.to_lowercase()))
}

fn native_string_upper(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("upper() expects 0 arguments (plus 'this').".to_string()); }
    let s = value_to_string(_heap, &args[0]);
    Ok(Value::String(s.to_uppercase()))
}

fn native_string_len(_heap: &[Option<Obj>], args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("len() expects 0 arguments (plus 'this').".to_string()); }
    match &args[0] {
        Value::String(s) => Ok(Value::Int(s.len() as i64)),
        Value::Object(idx) => {
            if let Some(Some(obj)) = _heap.get(*idx) {
                if let ObjType::String(s) = &obj.obj_type {
                    return Ok(Value::Int(s.len() as i64));
                }
            }
            Err("Not a string".to_string())
        }
        _ => Err("Not a string".to_string()),
    }
}
