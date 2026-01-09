use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::TypeChecker;

#[test]
fn test_type_checker_compound_types() {
    let source = r#"
vector[int] my_vec = null // Should fail, vector itself is not nullable unless declared as vector[int]?
"#.to_string();
    // Actually, in many languages containers can be null. 
    // In Lavina, let's say only explicitly ? marked types are nullable.
}

#[test]
fn test_null_safety_basic() {
    let source = r#"
int? maybe_x = 10
maybe_x = null
int y = maybe_x + 5 // Should fail: maybe_x is nullable
"#.to_string();
    let mut scanner = Scanner::new(source);
    let (tokens, _) = scanner.scan_tokens();
    let mut parser = Parser::new(tokens.clone());
    let program = parser.parse_program().unwrap();
    let mut checker = TypeChecker::new();
    let result = checker.check_program(&program);
    
    assert!(result.is_err());
    assert!(result.unwrap_err().contains("Use a null check first"));
}

#[test]
fn test_null_assignment() {
    let source = r#"
int x = null // Should fail
"#.to_string();
    let mut scanner = Scanner::new(source);
    let (tokens, _) = scanner.scan_tokens();
    let mut parser = Parser::new(tokens.clone());
    let program = parser.parse_program().unwrap();
    let mut checker = TypeChecker::new();
    let result = checker.check_program(&program);
    
    assert!(result.is_err());
}
