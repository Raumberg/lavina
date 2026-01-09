use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;

#[test]
fn test_type_checker_auto_vector() {
    let source = r#"
auto my_vec = [1, 2, 3]
print(my_vec[0])
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    let mut parser = Parser::new(tokens.clone(), source.clone());
    let statements = parser.parse_program().unwrap();
    
    let mut checker = TypeChecker::new();
    checker.set_source(source);
    let result = checker.check_statements(&statements);
    
    assert!(result.is_ok());
}

#[test]
fn test_null_assignment_error() {
    let source = r#"
int x = null
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    let mut parser = Parser::new(tokens.clone(), source.clone());
    let statements = parser.parse_program().unwrap();
    
    let mut checker = TypeChecker::new();
    checker.set_source(source);
    let result = checker.check_statements(&statements);
    
    assert!(result.is_err());
}

#[test]
fn test_map_type_mismatch() {
    let source = r#"
hashmap[int, int] test2 = {"1": 1, 2: 2}
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    let mut parser = Parser::new(tokens.clone(), source.clone());
    let statements = parser.parse_program().unwrap();
    
    let mut checker = TypeChecker::new();
    checker.set_source(source);
    let result = checker.check_statements(&statements);
    
    assert!(result.is_err());
}
