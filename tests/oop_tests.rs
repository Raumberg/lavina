use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;
use lavina::compiler::compiler::Compiler;
use lavina::compiler::scope::FunctionType;
use lavina::vm::vm::VM;

fn run_oop_code(source: &str) -> Result<(), String> {
    let source = source.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();
    if !errors.is_empty() {
        return Err(format!("Lexer error: {:?}", errors[0]));
    }

    let mut parser = Parser::new(tokens.clone(), source.clone());
    let statements = parser.parse_program().map_err(|e| format!("Parser error: {}", e))?;

    let mut checker = TypeChecker::new();
    checker.set_source(source);
    checker.check_statements(&statements).map_err(|e| format!("Type error: {}", e))?;

    let compiler = Compiler::new("<test>".to_string(), FunctionType::Script);
    let function = compiler.compile(&statements).map_err(|e| format!("Compiler error: {}", e))?;

    let mut vm = VM::new();
    vm.interpret(function);
    Ok(())
}

#[test]
fn test_class_basic() {
    let code = r#"
class Point:
    int x
    int y

    void fn __init__(int x, int y):
        this.x = x
        this.y = y

    int fn sum():
        return this.x + this.y

auto p = Point(10, 20)
if p.sum() != 30:
    print("Error")
"#;
    let result = run_oop_code(code);
    if let Err(e) = &result {
        println!("Error in test_class_basic: {}", e);
    }
    assert!(result.is_ok());
}

#[test]
fn test_class_static_method() {
    let code = r#"
class Utils:
    static int fn square(int x):
        return x * x

int res = Utils::square(5)
if res != 25:
    print("Error")
"#;
    assert!(run_oop_code(code).is_ok());
}

#[test]
fn test_class_type_error_constructor() {
    let code = r#"
class Test:
    void fn __init__(string s):
        print(s)

auto t = Test(123) // Error: expected string
"#;
    let result = run_oop_code(code);
    assert!(result.is_err());
    let err = result.unwrap_err();
    assert!(err.contains("expected String, but got Int") || err.contains("expected string, but got int"), "Error was: {}", err);
}

#[test]
fn test_class_type_error_method() {
    let code = r#"
class Test:
    void fn take_int(int x):
        print(x)

auto t = Test()
t.take_int("not an int")
"#;
    let result = run_oop_code(code);
    assert!(result.is_err());
    let err = result.unwrap_err();
    assert!(err.contains("expected Int, but got String") || err.contains("expected int, but got string"), "Error was: {}", err);
}

#[test]
fn test_static_vs_instance_error() {
    let code = r#"
class Test:
    static void fn static_fn():
        print("static")
    void fn instance_fn():
        print("instance")

auto t = Test()
t.static_fn() // Error: should use ::
"#;
    let result = run_oop_code(code);
    assert!(result.is_err());
    let err = result.unwrap_err();
    assert!(err.contains("is static. Use '::' to access it"), "Error was: {}", err);

    let code2 = r#"
class Test:
    void fn instance_fn():
        print("instance")

Test::instance_fn() // Error: should use .
"#;
    let result2 = run_oop_code(code2);
    assert!(result2.is_err());
    let err2 = result2.unwrap_err();
    assert!(err2.contains("is not static. Use '.' to access it"), "Error was: {}", err2);
}

#[test]
fn test_enum_basic() {
    let code = r#"
enum Color:
    null Red
    null Green
    int Custom

auto r = Color::Red
auto c = Color::Custom(255)

if r["_tag"] != "Red":
    print("Error")
if c["0"] != 255:
    print("Error")
"#;
    assert!(run_oop_code(code).is_ok());
}

#[test]
fn test_struct_basic() {
    let code = r#"
struct Vec2:
    int x
    int y
    
    void fn __init__(int x, int y):
        this.x = x
        this.y = y

auto v = Vec2(1, 2)
if v.x + v.y != 3:
    print("Error")
"#;
    assert!(run_oop_code(code).is_ok());
}
