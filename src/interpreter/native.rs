use crate::interpreter::value::{Value, NativeFn, ObjType, Obj};
use crate::interpreter::vm::memory::Memory;

pub fn get_native_functions() -> Vec<(String, NativeFn)> {
    vec![
        ("__native_print".to_string(), native_print),
        ("__native_len".to_string(), native_len),
        ("__native_clock".to_string(), native_clock),
        ("__native_typeof".to_string(), native_type),
        ("__native_sqrt".to_string(), native_sqrt),
        ("__native_abs".to_string(), native_abs),
        ("__native_sin".to_string(), native_sin),
        ("__native_cos".to_string(), native_cos),
        ("__native_tan".to_string(), native_tan),
        ("__native_min".to_string(), native_min),
        ("__native_max".to_string(), native_max),
        ("__native_floor".to_string(), native_floor),
        ("__native_ceil".to_string(), native_ceil),
        ("__native_round".to_string(), native_round),
        ("__native_input".to_string(), native_input),
        ("__native_string_lower".to_string(), native_string_lower),
        ("__native_string_upper".to_string(), native_string_upper),
        ("__native_string_len".to_string(), native_string_len),
        ("__native_string_trim".to_string(), native_string_trim),
        ("__native_string_replace".to_string(), native_string_replace),
        ("__native_string_contains".to_string(), native_string_contains),
        ("__native_string_starts_with".to_string(), native_string_starts_with),
        ("__native_string_ends_with".to_string(), native_string_ends_with),
        ("__native_string_split".to_string(), native_string_split),
        ("__native_string_idxof".to_string(), native_string_index_of),
        ("__native_string_is_empty".to_string(), native_string_is_empty),
        ("__native_vector_push".to_string(), native_vector_push),
        ("__native_vector_pop".to_string(), native_vector_pop),
        ("__native_vector_clear".to_string(), native_vector_clear),
        ("__native_vector_remove".to_string(), native_vector_remove),
        ("__native_vector_join".to_string(), native_vector_join),
        ("__native_vector_contains".to_string(), native_vector_contains),
        ("__native_vector_reverse".to_string(), native_vector_reverse),
        ("__native_vector_is_empty".to_string(), native_vector_is_empty),
        ("__native_hashmap_keys".to_string(), native_hashmap_keys),
        ("__native_hashmap_values".to_string(), native_hashmap_values),
        ("__native_hashmap_has".to_string(), native_hashmap_has),
        ("__native_hashmap_clear".to_string(), native_hashmap_clear),
        ("__native_hashmap_remove".to_string(), native_hashmap_remove),
        ("__native_hashmap_is_empty".to_string(), native_hashmap_is_empty),
        ("__native_range".to_string(), native_range),
        ("__native_memory_addr".to_string(), native_memory_addr),
        ("__native_memory_deref".to_string(), native_memory_deref),
        ("__native_memory_delete".to_string(), native_memory_delete),
        ("__native_memory_allocated".to_string(), native_memory_allocated),
        ("__native_memory_sizeof".to_string(), native_memory_sizeof),
        ("__native_fs_read".to_string(), native_fs_read),
        ("__native_fs_write".to_string(), native_fs_write),
        ("__native_fs_exists".to_string(), native_fs_exists),
        ("__native_os_args".to_string(), native_os_args),
        ("__native_os_env".to_string(), native_os_env),
        ("__native_json_stringify".to_string(), native_json_stringify),
        ("__native_json_parse".to_string(), native_json_parse),
        ("print".to_string(), native_print),
        ("len".to_string(), native_len),
        ("clock".to_string(), native_clock),
        ("typeof".to_string(), native_type),
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

fn values_equal(heap: &[Option<Obj>], a: &Value, b: &Value) -> bool {
    match (a, b) {
        (Value::Int(x), Value::Int(y)) => x == y,
        (Value::Float(x), Value::Float(y)) => x == y,
        (Value::Bool(x), Value::Bool(y)) => x == y,
        (Value::Null, Value::Null) => true,
        (Value::String(s1), Value::String(s2)) => s1 == s2,
        (Value::Object(idx1), Value::Object(idx2)) => {
            if idx1 == idx2 { return true; }
            let obj1 = heap[*idx1].as_ref().unwrap();
            let obj2 = heap[*idx2].as_ref().unwrap();
            match (&obj1.obj_type, &obj2.obj_type) {
                (ObjType::String(s1), ObjType::String(s2)) => s1 == s2,
                (ObjType::String(s1), _) => {
                    if let Value::String(s2) = b { s1 == s2 } else { false }
                }
                (_, ObjType::String(s2)) => {
                    if let Value::String(s1) = a { s1 == s2 } else { false }
                }
                _ => false,
            }
        }
        (Value::Object(idx), Value::String(s2)) => {
            let obj = heap[*idx].as_ref().unwrap();
            if let ObjType::String(s1) = &obj.obj_type { s1 == s2 } else { false }
        }
        (Value::String(s1), Value::Object(idx)) => {
            let obj = heap[*idx].as_ref().unwrap();
            if let ObjType::String(s2) = &obj.obj_type { s1 == s2 } else { false }
        }
        _ => false,
    }
}

fn native_print(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.is_empty() {
        println!();
        return Ok(Value::Null);
    }

    let first_arg_str = value_to_string(&memory.heap, &args[0]);
    
    if args.len() > 1 && first_arg_str.contains("{}") {
        let mut result = String::new();
        let mut arg_idx = 1;
        let parts: Vec<&str> = first_arg_str.split("{}").collect();
        
        for (i, part) in parts.iter().enumerate() {
            result.push_str(part);
            if i < parts.len() - 1 && arg_idx < args.len() {
                result.push_str(&value_to_string(&memory.heap, &args[arg_idx]));
                arg_idx += 1;
            }
        }
        println!("{}", result);
    } else {
        for (i, arg) in args.iter().enumerate() {
            if i > 0 { print!(" "); }
            print!("{}", value_to_string(&memory.heap, arg));
        }
        println!();
    }
    
    Ok(Value::Null)
}

fn native_len(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 {
        return Err("len() expects exactly 1 argument.".to_string());
    }
    match &args[0] {
        Value::String(s) => Ok(Value::Int(s.len() as i64)),
        Value::Object(idx) => {
            if let Some(Some(obj)) = memory.heap.get(*idx) {
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

fn native_clock(_memory: &mut Memory, _args: Vec<Value>) -> Result<Value, String> {
    use std::time::{SystemTime, UNIX_EPOCH};
    let start = SystemTime::now();
    let since_the_epoch = start.duration_since(UNIX_EPOCH).expect("Time went backwards");
    Ok(Value::Float(since_the_epoch.as_secs_f64()))
}

fn native_type(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 {
        return Err("typeof() expects exactly 1 argument.".to_string());
    }
    let type_name = match &args[0] {
        Value::Int(_) => "int",
        Value::Float(_) => "float",
        Value::String(_) => "string",
        Value::Bool(_) => "bool",
        Value::Null => "null",
        Value::NativeFunction(_, _) => "native function",
        Value::Object(idx) => {
            if let Some(Some(obj)) = memory.heap.get(*idx) {
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
        Value::TemplateFunction(_) => "function",
    };
    Ok(Value::String(type_name.to_string()))
}

fn native_sqrt(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("sqrt() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).sqrt())),
        Value::Float(f) => Ok(Value::Float(f.sqrt())),
        _ => Err("sqrt() expects number.".to_string()),
    }
}

fn native_abs(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("abs() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Int(i.abs())),
        Value::Float(f) => Ok(Value::Float(f.abs())),
        _ => Err("abs() expects number.".to_string()),
    }
}

fn native_sin(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("sin() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).sin())),
        Value::Float(f) => Ok(Value::Float(f.sin())),
        _ => Err("sin() expects number.".to_string()),
    }
}

fn native_cos(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("cos() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).cos())),
        Value::Float(f) => Ok(Value::Float(f.cos())),
        _ => Err("cos() expects number.".to_string()),
    }
}

fn native_tan(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("tan() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Float((i as f64).tan())),
        Value::Float(f) => Ok(Value::Float(f.tan())),
        _ => Err("tan() expects number.".to_string()),
    }
}

fn native_min(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("min() expects 2 arguments.".to_string()); }
    match (&args[0], &args[1]) {
        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(*a.min(b))),
        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a.min(*b))),
        (Value::Int(a), Value::Float(b)) => Ok(Value::Float((*a as f64).min(*b))),
        (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a.min(*b as f64))),
        _ => Err("min() expects numbers.".to_string()),
    }
}

fn native_max(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("max() expects 2 arguments.".to_string()); }
    match (&args[0], &args[1]) {
        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(*a.max(b))),
        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a.max(*b))),
        (Value::Int(a), Value::Float(b)) => Ok(Value::Float((*a as f64).max(*b))),
        (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a.max(*b as f64))),
        _ => Err("max() expects numbers.".to_string()),
    }
}

fn native_floor(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("floor() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Int(i)),
        Value::Float(f) => Ok(Value::Float(f.floor())),
        _ => Err("floor() expects number.".to_string()),
    }
}

fn native_ceil(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("ceil() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Int(i)),
        Value::Float(f) => Ok(Value::Float(f.ceil())),
        _ => Err("ceil() expects number.".to_string()),
    }
}

fn native_round(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("round() expects 1 argument.".to_string()); }
    match args[0] {
        Value::Int(i) => Ok(Value::Int(i)),
        Value::Float(f) => Ok(Value::Float(f.round())),
        _ => Err("round() expects number.".to_string()),
    }
}

fn native_input(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    use std::io::{self, Write};
    if !args.is_empty() {
        print!("{}", value_to_string(&memory.heap, &args[0]));
        io::stdout().flush().map_err(|e| e.to_string())?;
    }
    let mut input = String::new();
    io::stdin().read_line(&mut input).map_err(|e| e.to_string())?;
    Ok(Value::String(input.trim_end().to_string()))
}

fn native_string_lower(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("lower() expects 0 arguments (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    Ok(Value::String(s.to_lowercase()))
}

fn native_string_upper(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("upper() expects 0 arguments (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    Ok(Value::String(s.to_uppercase()))
}

fn native_string_len(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("len() expects 0 arguments (plus 'this').".to_string()); }
    match &args[0] {
        Value::String(s) => Ok(Value::Int(s.len() as i64)),
        Value::Object(idx) => {
            if let Some(Some(obj)) = memory.heap.get(*idx) {
                if let ObjType::String(s) = &obj.obj_type {
                    return Ok(Value::Int(s.len() as i64));
                }
            }
            Err("Not a string".to_string())
        }
        _ => Err("Not a string".to_string()),
    }
}

fn native_string_trim(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("trim() expects 0 arguments (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    Ok(Value::String(s.trim().to_string()))
}

fn native_string_replace(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 3 { return Err("replace() expects 2 arguments: from and to (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    let from = value_to_string(&memory.heap, &args[1]);
    let to = value_to_string(&memory.heap, &args[2]);
    Ok(Value::String(s.replace(&from, &to)))
}

fn native_string_contains(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("contains() expects 1 argument: substring (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    let sub = value_to_string(&memory.heap, &args[1]);
    Ok(Value::Bool(s.contains(&sub)))
}

fn native_string_starts_with(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("starts_with() expects 1 argument (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    let prefix = value_to_string(&memory.heap, &args[1]);
    Ok(Value::Bool(s.starts_with(&prefix)))
}

fn native_string_ends_with(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("ends_with() expects 1 argument (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    let suffix = value_to_string(&memory.heap, &args[1]);
    Ok(Value::Bool(s.ends_with(&suffix)))
}

fn native_string_split(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("split() expects 1 argument: delimiter (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    let delim = value_to_string(&memory.heap, &args[1]);
    
    let raw_parts: Vec<String> = s.split(&delim).map(|p| p.to_string()).collect();
    let mut parts = Vec::new();
    for p in raw_parts {
        let idx = memory.alloc(ObjType::String(p));
        parts.push(Value::Object(idx));
    }
    
    let vec_idx = memory.alloc(ObjType::Vector(parts));
    Ok(Value::Object(vec_idx))
}

fn native_string_index_of(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("idxof() expects 1 argument: substring (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    let sub = value_to_string(&memory.heap, &args[1]);
    
    match s.find(&sub) {
        Some(idx) => Ok(Value::Int(idx as i64)),
        None => Ok(Value::Int(-1)),
    }
}

fn native_string_is_empty(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("is_empty() expects 0 arguments (plus 'this').".to_string()); }
    let s = value_to_string(&memory.heap, &args[0]);
    Ok(Value::Bool(s.is_empty()))
}

fn native_vector_push(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("push() expects 1 argument (plus 'this').".to_string()); }
    let val = args[1].clone();
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get_mut(*idx) {
            if let ObjType::Vector(v) = &mut obj.obj_type {
                v.push(val);
                return Ok(Value::Null);
            }
        }
    }
    Err("push() can only be called on vectors.".to_string())
}

fn native_vector_pop(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("pop() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get_mut(*idx) {
            if let ObjType::Vector(v) = &mut obj.obj_type {
                return Ok(v.pop().unwrap_or(Value::Null));
            }
        }
    }
    Err("pop() can only be called on vectors.".to_string())
}

fn native_vector_clear(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("clear() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get_mut(*idx) {
            if let ObjType::Vector(v) = &mut obj.obj_type {
                v.clear();
                return Ok(Value::Null);
            }
        }
    }
    Err("clear() can only be called on vectors.".to_string())
}

fn native_vector_remove(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("remove() expects 1 argument: index (plus 'this').".to_string()); }
    let i = match args[1].as_int() {
        Some(i) => i,
        None => return Err("Index must be an integer.".to_string()),
    };
    
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get_mut(*idx) {
            if let ObjType::Vector(v) = &mut obj.obj_type {
                if i < 0 || i >= v.len() as i64 { return Err("Index out of bounds.".to_string()); }
                return Ok(v.remove(i as usize));
            }
        }
    }
    Err("remove() can only be called on vectors.".to_string())
}

fn native_vector_join(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("join() expects 1 argument: delimiter (plus 'this').".to_string()); }
    let delim = value_to_string(&memory.heap, &args[1]);
    
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get(*idx) {
            if let ObjType::Vector(v) = &obj.obj_type {
                let parts: Vec<String> = v.iter().map(|val| value_to_string(&memory.heap, val)).collect();
                let result = parts.join(&delim);
                let s_idx = memory.alloc(ObjType::String(result));
                return Ok(Value::Object(s_idx));
            }
        }
    }
    Err("join() can only be called on vectors.".to_string())
}

fn native_vector_contains(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("contains() expects 1 argument: item (plus 'this').".to_string()); }
    let target = &args[1];
    
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get(*idx) {
            if let ObjType::Vector(v) = &obj.obj_type {
                for item in v {
                    if values_equal(&memory.heap, item, target) { return Ok(Value::Bool(true)); }
                }
                return Ok(Value::Bool(false));
            }
        }
    }
    Err("contains() can only be called on vectors.".to_string())
}

fn native_vector_reverse(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("reverse() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get_mut(*idx) {
            if let ObjType::Vector(v) = &mut obj.obj_type {
                v.reverse();
                return Ok(Value::Null);
            }
        }
    }
    Err("reverse() can only be called on vectors.".to_string())
}

fn native_vector_is_empty(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("is_empty() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get(*idx) {
            if let ObjType::Vector(v) = &obj.obj_type {
                return Ok(Value::Bool(v.is_empty()));
            }
        }
    }
    Err("is_empty() can only be called on vectors.".to_string())
}

fn native_hashmap_keys(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("keys() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        let raw_keys: Vec<String> = if let Some(Some(obj)) = memory.heap.get(*idx) {
            if let ObjType::HashMap(m) = &obj.obj_type {
                m.keys().cloned().collect()
            } else { return Err("keys() can only be called on hashmaps.".to_string()); }
        } else { return Err("Object not found.".to_string()); };
        
        let mut keys = Vec::new();
        for k in raw_keys {
            let s_idx = memory.alloc(ObjType::String(k));
            keys.push(Value::Object(s_idx));
        }
        
        let vec_idx = memory.alloc(ObjType::Vector(keys));
        return Ok(Value::Object(vec_idx));
    }
    Err("keys() can only be called on hashmaps.".to_string())
}

fn native_hashmap_values(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("values() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        let raw_vals: Vec<Value> = if let Some(Some(obj)) = memory.heap.get(*idx) {
            if let ObjType::HashMap(m) = &obj.obj_type {
                m.values().cloned().collect()
            } else { return Err("values() can only be called on hashmaps.".to_string()); }
        } else { return Err("Object not found.".to_string()); };
        
        let vec_idx = memory.alloc(ObjType::Vector(raw_vals));
        return Ok(Value::Object(vec_idx));
    }
    Err("values() can only be called on hashmaps.".to_string())
}

fn native_hashmap_has(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("has() expects 1 argument: key (plus 'this').".to_string()); }
    let key = value_to_string(&memory.heap, &args[1]);
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get(*idx) {
            if let ObjType::HashMap(m) = &obj.obj_type {
                return Ok(Value::Bool(m.contains_key(&key)));
            }
        }
    }
    Err("has() can only be called on hashmaps.".to_string())
}

fn native_hashmap_clear(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("clear() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get_mut(*idx) {
            if let ObjType::HashMap(m) = &mut obj.obj_type {
                m.clear();
                return Ok(Value::Null);
            }
        }
    }
    Err("clear() can only be called on hashmaps.".to_string())
}

fn native_hashmap_remove(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("remove() expects 1 argument: key (plus 'this').".to_string()); }
    let key = value_to_string(&memory.heap, &args[1]);
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get_mut(*idx) {
            if let ObjType::HashMap(m) = &mut obj.obj_type {
                return Ok(m.remove(&key).unwrap_or(Value::Null));
            }
        }
    }
    Err("remove() can only be called on hashmaps.".to_string())
}

fn native_hashmap_is_empty(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("is_empty() expects 0 arguments (plus 'this').".to_string()); }
    if let Value::Object(idx) = &args[0] {
        if let Some(Some(obj)) = memory.heap.get(*idx) {
            if let ObjType::HashMap(m) = &obj.obj_type {
                return Ok(Value::Bool(m.is_empty()));
            }
        }
    }
    Err("is_empty() can only be called on hashmaps.".to_string())
}

fn native_range(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("range() expects 2 arguments: start and end.".to_string()); }
    let start = match args[0].as_int() { Some(i) => i, None => return Err("start must be int.".to_string()) };
    let end = match args[1].as_int() { Some(i) => i, None => return Err("end must be int.".to_string()) };
    
    let mut vec = Vec::new();
    for i in start..end {
        vec.push(Value::Int(i));
    }
    
    let idx = memory.alloc(ObjType::Vector(vec));
    Ok(Value::Object(idx))
}

fn native_memory_addr(_memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("addr() expects 1 argument.".to_string()); }
    match &args[0] {
        Value::Object(idx) => Ok(Value::Int(*idx as i64)),
        _ => Err("addr() can only be called on heap objects (string, vector, hashmap, instance).".to_string()),
    }
}

fn native_memory_deref(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("unsafe_deref() expects 1 argument (address).".to_string()); }
    let addr = match args[0].as_int() {
        Some(i) => i as usize,
        None => return Err("Address must be an integer.".to_string()),
    };
    
    if addr < memory.heap.len() && memory.heap[addr].is_some() {
        Ok(Value::Object(addr))
    } else {
        Err(format!("Segmentation fault: address {} is invalid or empty.", addr))
    }
}

fn native_memory_delete(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("unsafe_delete() expects 1 argument (address).".to_string()); }
    let addr = match args[0].as_int() {
        Some(i) => i as usize,
        None => return Err("Address must be an integer.".to_string()),
    };
    
    if addr < memory.heap.len() {
        memory.heap[addr] = None;
        Ok(Value::Null)
    } else {
        Err(format!("Address {} out of bounds.", addr))
    }
}

fn native_memory_allocated(memory: &mut Memory, _args: Vec<Value>) -> Result<Value, String> {
    Ok(Value::Int(memory.bytes_allocated as i64))
}

fn native_memory_sizeof(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("sizeof() expects 1 argument.".to_string()); }
    
    let base_size = std::mem::size_of::<Value>();
    
    match &args[0] {
        Value::Int(_) | Value::Float(_) | Value::Bool(_) | Value::Null => Ok(Value::Int(base_size as i64)),
        Value::String(s) => Ok(Value::Int((base_size + s.len()) as i64)),
        Value::Object(idx) => {
            if let Some(Some(obj)) = memory.heap.get(*idx) {
                let obj_header_size = std::mem::size_of::<Obj>();
                let extra_size = match &obj.obj_type {
                    ObjType::String(s) => s.len(),
                    ObjType::Vector(v) => v.len() * std::mem::size_of::<Value>(),
                    ObjType::HashMap(m) => m.len() * (std::mem::size_of::<String>() + std::mem::size_of::<Value>()),
                    ObjType::Namespace(_, m) => m.len() * (std::mem::size_of::<String>() + std::mem::size_of::<Value>()),
                    ObjType::Class(c) => c.name.len() + c.methods.len() * (std::mem::size_of::<String>() + std::mem::size_of::<Value>()),
                    ObjType::Instance(i) => i.fields.len() * (std::mem::size_of::<String>() + std::mem::size_of::<Value>()),
                    ObjType::Closure(c) => c.upvalues.len() * std::mem::size_of::<usize>(),
                    ObjType::Function(f) => f.name.len() + f.chunk.code.len() + f.chunk.constants.len() * std::mem::size_of::<Value>(),
                    _ => 0,
                };
                Ok(Value::Int((obj_header_size + extra_size) as i64))
            } else {
                Ok(Value::Int(0))
            }
        }
        _ => Ok(Value::Int(base_size as i64)),
    }
}

fn native_fs_read(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("read() expects 1 argument (path).".to_string()); }
    let path = value_to_string(&memory.heap, &args[0]);
    let content = std::fs::read_to_string(path).map_err(|e| e.to_string())?;
    let idx = memory.alloc(ObjType::String(content));
    Ok(Value::Object(idx))
}

fn native_fs_write(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 2 { return Err("write() expects 2 arguments: path and content.".to_string()); }
    let path = value_to_string(&memory.heap, &args[0]);
    let content = value_to_string(&memory.heap, &args[1]);
    std::fs::write(path, content).map_err(|e| e.to_string())?;
    Ok(Value::Null)
}

fn native_fs_exists(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("exists() expects 1 argument (path).".to_string()); }
    let path = value_to_string(&memory.heap, &args[0]);
    Ok(Value::Bool(std::path::Path::new(&path).exists()))
}

fn native_os_args(memory: &mut Memory, _args: Vec<Value>) -> Result<Value, String> {
    let raw_args: Vec<String> = std::env::args().collect();
    let mut args = Vec::new();
    for a in raw_args {
        let idx = memory.alloc(ObjType::String(a));
        args.push(Value::Object(idx));
    }
    let vec_idx = memory.alloc(ObjType::Vector(args));
    Ok(Value::Object(vec_idx))
}

fn native_os_env(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("env() expects 1 argument (variable name).".to_string()); }
    let name = value_to_string(&memory.heap, &args[0]);
    match std::env::var(name) {
        Ok(val) => {
            let idx = memory.alloc(ObjType::String(val));
            Ok(Value::Object(idx))
        }
        Err(_) => Ok(Value::Null),
    }
}

fn native_json_stringify(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("stringify() expects 1 argument.".to_string()); }
    
    fn stringify_recursive(heap: &[Option<Obj>], val: &Value) -> Result<String, String> {
        match val {
            Value::Int(i) => Ok(i.to_string()),
            Value::Float(f) => Ok(format!("{:?}", f)),
            Value::Bool(b) => Ok(b.to_string()),
            Value::Null => Ok("null".to_string()),
            Value::String(s) => Ok(format!("\"{}\"", s.replace("\"", "\\\""))),
            Value::Object(idx) => {
                let obj = heap.get(*idx).and_then(|o| o.as_ref()).ok_or("Invalid object")?;
                match &obj.obj_type {
                    ObjType::String(s) => Ok(format!("\"{}\"", s.replace("\"", "\\\""))),
                    ObjType::Vector(v) => {
                        let mut res = "[".to_string();
                        for (i, item) in v.iter().enumerate() {
                            if i > 0 { res.push_str(", "); }
                            res.push_str(&stringify_recursive(heap, item)?);
                        }
                        res.push(']');
                        Ok(res)
                    }
                    ObjType::HashMap(m) => {
                        let mut res = "{".to_string();
                        for (i, (k, v)) in m.iter().enumerate() {
                            if i > 0 { res.push_str(", "); }
                            res.push_str(&format!("\"{}\": ", k.replace("\"", "\\\"")));
                            res.push_str(&stringify_recursive(heap, v)?);
                        }
                        res.push('}');
                        Ok(res)
                    }
                    _ => Ok("null".to_string()),
                }
            }
            _ => Ok("null".to_string()),
        }
    }

    let result = stringify_recursive(&memory.heap, &args[0])?;
    let idx = memory.alloc(ObjType::String(result));
    Ok(Value::Object(idx))
}

fn native_json_parse(memory: &mut Memory, args: Vec<Value>) -> Result<Value, String> {
    if args.len() != 1 { return Err("parse() expects 1 argument.".to_string()); }
    let json_str = value_to_string(&memory.heap, &args[0]);
    
    struct JsonParser<'a> {
        chars: std::iter::Peekable<std::str::Chars<'a>>,
    }

    impl<'a> JsonParser<'a> {
        fn skip_whitespace(&mut self) {
            while let Some(&c) = self.chars.peek() {
                if c.is_whitespace() { self.chars.next(); } else { break; }
            }
        }

        fn parse_value(&mut self, memory: &mut Memory) -> Result<Value, String> {
            self.skip_whitespace();
            match self.chars.peek() {
                Some('{') => self.parse_object(memory),
                Some('[') => self.parse_array(memory),
                Some('"') => self.parse_string(memory),
                Some('t') | Some('f') => self.parse_bool(),
                Some('n') => self.parse_null(),
                Some(c) if c.is_digit(10) || *c == '-' => self.parse_number(),
                _ => Err("Unexpected character in JSON".to_string()),
            }
        }

        fn parse_object(&mut self, memory: &mut Memory) -> Result<Value, String> {
            self.chars.next(); // skip '{'
            let mut map = std::collections::HashMap::new();
            loop {
                self.skip_whitespace();
                if let Some('}') = self.chars.peek() { self.chars.next(); break; }
                
                let key = match self.parse_value(memory)? {
                    Value::Object(idx) => {
                        if let ObjType::String(s) = &memory.heap[idx].as_ref().unwrap().obj_type {
                            s.clone()
                        } else { return Err("JSON object keys must be strings".to_string()); }
                    }
                    Value::String(s) => s,
                    _ => return Err("JSON object keys must be strings".to_string()),
                };

                self.skip_whitespace();
                if self.chars.next() != Some(':') { return Err("Expected ':' in JSON object".to_string()); }
                
                let val = self.parse_value(memory)?;
                map.insert(key, val);

                self.skip_whitespace();
                match self.chars.next() {
                    Some(',') => continue,
                    Some('}') => break,
                    _ => return Err("Expected ',' or '}' in JSON object".to_string()),
                }
            }
            let idx = memory.alloc(ObjType::HashMap(map));
            Ok(Value::Object(idx))
        }

        fn parse_array(&mut self, memory: &mut Memory) -> Result<Value, String> {
            self.chars.next(); // skip '['
            let mut vec = Vec::new();
            loop {
                self.skip_whitespace();
                if let Some(']') = self.chars.peek() { self.chars.next(); break; }
                
                vec.push(self.parse_value(memory)?);

                self.skip_whitespace();
                match self.chars.next() {
                    Some(',') => continue,
                    Some(']') => break,
                    _ => return Err("Expected ',' or ']' in JSON array".to_string()),
                }
            }
            let idx = memory.alloc(ObjType::Vector(vec));
            Ok(Value::Object(idx))
        }

        fn parse_string(&mut self, memory: &mut Memory) -> Result<Value, String> {
            self.chars.next(); // skip '"'
            let mut s = String::new();
            while let Some(c) = self.chars.next() {
                if c == '"' { break; }
                if c == '\\' {
                    match self.chars.next() {
                        Some('\"') => s.push('"'),
                        Some('\\') => s.push('\\'),
                        Some('n') => s.push('\n'),
                        Some('r') => s.push('\r'),
                        Some('t') => s.push('\t'),
                        _ => s.push('\\'),
                    }
                } else {
                    s.push(c);
                }
            }
            let idx = memory.alloc(ObjType::String(s));
            Ok(Value::Object(idx))
        }

        fn parse_bool(&mut self) -> Result<Value, String> {
            let mut s = String::new();
            while let Some(&c) = self.chars.peek() {
                if c.is_alphabetic() { s.push(self.chars.next().unwrap()); } else { break; }
            }
            if s == "true" { Ok(Value::Bool(true)) }
            else if s == "false" { Ok(Value::Bool(false)) }
            else { Err(format!("Invalid boolean in JSON: {}", s)) }
        }

        fn parse_null(&mut self) -> Result<Value, String> {
            let mut s = String::new();
            while let Some(&c) = self.chars.peek() {
                if c.is_alphabetic() { s.push(self.chars.next().unwrap()); } else { break; }
            }
            if s == "null" { Ok(Value::Null) }
            else { Err(format!("Invalid null in JSON: {}", s)) }
        }

        fn parse_number(&mut self) -> Result<Value, String> {
            let mut s = String::new();
            while let Some(&c) = self.chars.peek() {
                if c.is_digit(10) || c == '.' || c == '-' || c == 'e' || c == 'E' {
                    s.push(self.chars.next().unwrap());
                } else {
                    break;
                }
            }
            if s.contains('.') || s.contains('e') || s.contains('E') {
                s.parse::<f64>().map(Value::Float).map_err(|e| e.to_string())
            } else {
                s.parse::<i64>().map(Value::Int).map_err(|e| e.to_string())
            }
        }
    }

    let mut parser = JsonParser { chars: json_str.chars().peekable() };
    parser.parse_value(memory)
}
