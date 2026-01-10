use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;
use lavina::compiler::compiler::{Compiler, FunctionType};
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

fn run_lavina_code(source: String, current_dir: PathBuf) -> Result<(), String> {
    // Save current dir to restore later
    let original_dir = env::current_dir().unwrap();
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
                Err(format!("Type error: {}", e))
            } else {
                let compiler = Compiler::new("<test>".to_string(), FunctionType::Script);
                match compiler.compile(&statements) {
                    Ok(function) => {
                        let mut vm = VM::new();
                        vm.interpret(function);
                        Ok(())
                    }
                    Err(e) => Err(format!("Compiler error: {}", e)),
                }
            }
        }
        Err(e) => Err(format!("Parser error: {}", e)),
    };

    env::set_current_dir(original_dir).unwrap();
    result
}

#[test]
fn test_module_import_simple() {
    let test_dir = setup_test_dir("simple_import");
    
    let lib_code = r#"
public int fn add(int a, int b):
    return a + b
"#;
    fs::write(test_dir.join("math.lv"), lib_code).unwrap();

    let main_code = r#"
import math
int res = math.add(5, 10)
if res != 15:
    print("Failed")
else:
    print("Success")
"#;

    let result = run_lavina_code(main_code.to_string(), test_dir);
    assert!(result.is_ok(), "Test failed with error: {:?}", result.err());
}

#[test]
fn test_module_import_alias() {
    let test_dir = setup_test_dir("import_alias");
    
    let lib_code = r#"
public string fn greet(string name):
    return "Hello, " + name
"#;
    fs::write(test_dir.join("lib.lv"), lib_code).unwrap();

    let main_code = r#"
import lib as m
string res = m.greet("Alina")
"#;

    let result = run_lavina_code(main_code.to_string(), test_dir);
    assert!(result.is_ok());
}

#[test]
fn test_module_private_visibility() {
    let test_dir = setup_test_dir("private_visibility");
    
    let lib_code = r#"
private int fn hidden():
    return 42

public int fn visible():
    return hidden()
"#;
    fs::write(test_dir.join("secret.lv"), lib_code).unwrap();

    let main_code = r#"
import secret
int x = secret.visible() // OK
int y = secret.hidden()  // Should fail
"#;

    let result = run_lavina_code(main_code.to_string(), test_dir);
    assert!(result.is_err());
    assert!(result.unwrap_err().contains("Namespace 'secret' has no member 'hidden'"));
}

#[test]
fn test_module_lvpath() {
    let test_dir = setup_test_dir("lvpath_test");
    let external_dir = test_dir.join("external_libs");
    fs::create_dir_all(&external_dir).unwrap();

    let lib_code = r#"
public int fn get_val():
    return 100
"#;
    fs::write(external_dir.join("ext.lv"), lib_code).unwrap();

    let main_code = r#"
import ext
int x = ext.get_val()
"#;

    // Set LVPATH
    let old_lvpath = env::var("LVPATH").ok();
    unsafe {
        env::set_var("LVPATH", external_dir.to_str().unwrap());
    }

    let result = run_lavina_code(main_code.to_string(), test_dir);
    
    // Restore LVPATH
    unsafe {
        if let Some(val) = old_lvpath {
            env::set_var("LVPATH", val);
        } else {
            env::remove_var("LVPATH");
        }
    }

    assert!(result.is_ok(), "Test failed: {:?}", result.err());
}
