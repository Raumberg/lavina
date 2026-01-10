use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;
use lavina::compiler::compiler::Compiler;
use lavina::compiler::scope::FunctionType;
use lavina::vm::vm::VM;

fn run_exception_code(source: String) -> Result<(), String> {
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    let mut parser = Parser::new(tokens.clone(), source.clone());
    
    match parser.parse_program() {
        Ok(statements) => {
            let mut checker = TypeChecker::new();
            checker.set_source(source.clone());
            if let Err(e) = checker.check_statements(&statements) {
                return Err(format!("Type error: {}", e));
            }
            
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
        Err(e) => Err(format!("Parser error: {}", e)),
    }
}

#[test]
fn test_basic_try_catch() {
    let code = r#"
try:
    throw "error"
catch e:
    if e != "error":
        throw "fail"
"#;
    assert!(run_exception_code(code.to_string()).is_ok());
}

#[test]
fn test_catch_different_types() {
    let code = r#"
try:
    throw 42
catch e:
    if cast(e, int) != 42:
        throw "fail"
"#;
    assert!(run_exception_code(code.to_string()).is_ok());
}

#[test]
fn test_nested_try_catch() {
    let code = r#"
int x = 0
try:
    try:
        throw "inner"
    catch e:
        if e == "inner":
            x = 1
            throw "outer"
catch e:
    if e == "outer" and x == 1:
        x = 2

if x != 2:
    throw "final fail"
"#;
    assert!(run_exception_code(code.to_string()).is_ok());
}

#[test]
fn test_stack_unwinding_deep() {
    let code = r#"
void fn deep(int n):
    if n == 0:
        throw "boom"
    deep(n - 1)

int caught = 0
try:
    deep(10)
catch e:
    if e == "boom":
        caught = 1

if caught != 1:
    throw "unwinding failed"
"#;
    assert!(run_exception_code(code.to_string()).is_ok());
}

#[test]
fn test_no_exception_flow() {
    let code = r#"
int x = 0
try:
    x = 10
catch e:
    x = 20

if x != 10:
    throw "catch executed without throw"
"#;
    assert!(run_exception_code(code.to_string()).is_ok());
}

#[test]
fn test_catch_variable_scope() {
    let code = r#"
try:
    throw "msg"
catch err:
    auto s = err
"#;
    assert!(run_exception_code(code.to_string()).is_ok());
}
