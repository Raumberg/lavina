use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;
use lavina::compiler::compiler::Compiler;
use lavina::compiler::scope::FunctionType;
use lavina::vm::vm::VM;
use std::fs;
use std::path::PathBuf;
use std::env;

fn setup_test_dir(test_name: &str) -> PathBuf {
    let mut path = env::current_dir().unwrap();
    path.push("target");
    path.push("test_tmp");
    path.push(test_name);
    if path.exists() {
        fs::remove_dir_all(&path).unwrap();
    }
    fs::create_dir_all(&path).unwrap();
    path
}

fn run_stdlib_code(source: String, current_dir: PathBuf) -> Result<(), String> {
    let original_dir = env::current_dir().unwrap();
    
    // Set LVPATH to the project root so std lib can be found
    let project_root = original_dir.clone();
    let old_lvpath = env::var("LVPATH").ok();
    unsafe {
        env::set_var("LVPATH", &project_root);
    }

    if let Err(e) = env::set_current_dir(&current_dir) {
        return Err(format!("Could not change directory to {:?}: {}", current_dir, e));
    }

    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    let mut parser = Parser::new(tokens.clone(), source.clone());
    
    let result = match parser.parse_program() {
        Ok(statements) => {
            let mut checker = TypeChecker::new();
            checker.set_source(source.clone());
            if let Err(e) = checker.check_statements(&statements) {
                println!("Type error in stdlib test: {}", e);
                Err(format!("Type error: {}", e))
            } else {
                let compiler = Compiler::new("<test>".to_string(), FunctionType::Script);
                match compiler.compile(&statements) {
                    Ok(function) => {
                        let mut vm = VM::new();
                        vm.interpret(function);
                        Ok(())
                    }
                    Err(e) => {
                        println!("Compiler error in stdlib test: {}", e);
                        Err(format!("Compiler error: {}", e))
                    }
                }
            }
        }
        Err(e) => {
            println!("Parser error in stdlib test: {}", e);
            Err(format!("Parser error: {}", e))
        }
    };

    env::set_current_dir(original_dir).unwrap();
    
    // Restore LVPATH
    unsafe {
        if let Some(val) = old_lvpath {
            env::set_var("LVPATH", val);
        } else {
            env::remove_var("LVPATH");
        }
    }

    result
}

#[test]
fn test_std_fs() {
    let test_dir = setup_test_dir("std_fs_test");
    let code = r#"
import std::fs
string path = "test_file.txt"
string data = "Lavina FS Test"
fs::write(path, data)
if !fs::exists(path):
    print("Error: File should exist")
string read_back = fs::read(path)
if read_back != data:
    print("Error: Data mismatch")
"#;
    assert!(run_stdlib_code(code.to_string(), test_dir).is_ok());
}

#[test]
fn test_std_json() {
    let test_dir = setup_test_dir("std_json_test");
    let code = r#"
import std::json
auto original = {
    "str": "hello",
    "num": 42,
    "arr": [1, 2],
    "bool": true,
    "null": null
}
string s = json::stringify(original)
auto parsed = json::parse(s)
if parsed["str"] != "hello":
    print("Error: str")
if parsed["num"] != 42:
    print("Error: num")
if len(parsed["arr"]) != 2:
    print("Error: arr")
if parsed["bool"] != true:
    print("Error: bool")
"#;
    assert!(run_stdlib_code(code.to_string(), test_dir).is_ok());
}

#[test]
fn test_std_math() {
    let test_dir = setup_test_dir("std_math_test");
    let code = r#"
import std::math
if math::min(10, 20) != 10:
    print("Error: min")
if math::max(10, 20) != 20:
    print("Error: max")
if math::floor(1.9) != 1.0:
    print("Error: floor")
if math::ceil(1.1) != 2.0:
    print("Error: ceil")
if math::round(1.5) != 2.0:
    print("Error: round")
"#;
    assert!(run_stdlib_code(code.to_string(), test_dir).is_ok());
}

#[test]
fn test_std_memory() {
    let test_dir = setup_test_dir("std_memory_test");
    let code = r#"
import std::memory as mem
class Obj:
    int x
    void fn __init__(int x):
        this.x = x

auto o = Obj(100)
int addr = mem::addrof(o)
if addr <= 0:
    print("Error: addrof")
auto o2 = mem::deref(addr)
if o2.x != 100:
    print("Error: deref")
if mem::sizeof(o) < 8:
    print("Error: sizeof")
mem::delete(addr)
"#;
    assert!(run_stdlib_code(code.to_string(), test_dir).is_ok());
}

#[test]
fn test_std_os() {
    let test_dir = setup_test_dir("std_os_test");
    let code = r#"
import std::os
auto args = os::args()
if len(args) == 0:
    print("Error: no args")
"#;
    assert!(run_stdlib_code(code.to_string(), test_dir).is_ok());
}

#[test]
fn test_std_core_collections() {
    let test_dir = setup_test_dir("std_core_test");
    let code = r#"
// Test Vector methods
auto v = [1, 2, 3]
v.push(4)
if len(v) != 4:
    print("Error: vector push")
if !v.contains(2):
    print("Error: vector contains")
v.reverse()
if v[0] != 4:
    print("Error: vector reverse")

// Test HashMap methods
auto m = {"a": 1, "b": 2}
if !m.has("a"):
    print("Error: map has")
auto keys = m.keys()
if len(keys) != 2:
    print("Error: map keys")

// Test range and typeof
auto r = range(0, 5)
if len(r) != 5:
    print("Error: range")
if typeof("hello") != "string":
    print("Error: typeof")

// Test cast
string s_num = "123"
int i_num = cast(s_num, int)
if i_num != 123:
    print("Error: cast")
"#;
    assert!(run_stdlib_code(code.to_string(), test_dir).is_ok());
}

#[test]
fn test_std_core_strings() {
    let test_dir = setup_test_dir("std_string_test");
    let code = r#"
string s = "  Hello Lavina  "
if s.trim() != "Hello Lavina":
    print("Error: trim")
if s.lower().trim() != "hello lavina":
    print("Error: lower/chain")
auto parts = "a,b,c".split(",")
if len(parts) != 3:
    print("Error: split")
if parts[1] != "b":
    print("Error: split value")
"#;
    assert!(run_stdlib_code(code.to_string(), test_dir).is_ok());
}
