use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::parser::ast::{Expr, Literal, Stmt, Type};

#[test]
fn test_parse_simple_arithmetic() {
    let source = "1 + 2 * 3".to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();
    assert!(errors.is_empty());

    let mut parser = Parser::new(tokens.clone(), source);
    let expr = parser.parse_expression().unwrap();

    // Ожидаем: Binary(1, +, Binary(2, *, 3))
    if let Expr::Binary(left, op, right) = expr {
        assert_eq!(op.lexeme, "+");
        if let Expr::Literal(Literal::Int(val)) = *left {
            assert_eq!(val, 1);
        } else { panic!("Left should be 1"); }

        if let Expr::Binary(l, op2, r) = *right {
            assert_eq!(op2.lexeme, "*");
            if let Expr::Literal(Literal::Int(v)) = *l { assert_eq!(v, 2); }
            if let Expr::Literal(Literal::Int(v)) = *r { assert_eq!(v, 3); }
        } else { panic!("Right should be 2 * 3"); }
    } else { panic!("Should be a binary expression"); }
}

#[test]
fn test_parse_grouping() {
    let source = "(1 + 2) * 3".to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    
    let mut parser = Parser::new(tokens.clone(), source);
    let expr = parser.parse_expression().unwrap();

    // Ожидаем: Binary(Grouping(Binary(1, +, 2)), *, 3)
    if let Expr::Binary(left, op, _) = expr {
        assert_eq!(op.lexeme, "*");
        assert!(matches!(*left, Expr::Grouping(_)));
    } else { panic!("Should be a multiplication"); }
}

#[test]
fn test_parse_unary() {
    let source = "-5 * !true".to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();

    let mut parser = Parser::new(tokens.clone(), source);
    let expr = parser.parse_expression().unwrap();

    if let Expr::Binary(left, _, right) = expr {
        assert!(matches!(*left, Expr::Unary(_, _)));
        assert!(matches!(*right, Expr::Unary(_, _)));
    } else { panic!("Should be a binary expression"); }
}

#[test]
fn test_parse_function_with_if() {
    let source = r#"
fn abs(int n) -> int:
    if n < 0:
        return -n
    return n
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();
    assert!(errors.is_empty());

    let mut parser = Parser::new(tokens.clone(), source);
    let program = parser.parse_program().unwrap();

    assert_eq!(program.len(), 1);
    if let Stmt::Function(f) = &program[0] {
        assert_eq!(f.name.lexeme, "abs");
        assert_eq!(f.return_type, Type::Int);
        assert_eq!(f.params.len(), 1);
        assert_eq!(f.body.len(), 2); // if and return
    } else { panic!("Should be a function"); }
}

#[test]
fn test_parse_variable_declarations() {
    let source = r#"
auto x = 10
int y = 20
let z: string = "hello"
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    
    let mut parser = Parser::new(tokens.clone(), source);
    let program = parser.parse_program().unwrap();

    assert_eq!(program.len(), 3);
    assert!(matches!(program[0], Stmt::Let(_, Some(Type::Auto), _)));
    assert!(matches!(program[1], Stmt::Let(_, Some(Type::Int), _)));
    assert!(matches!(program[2], Stmt::Let(_, Some(Type::String), _)));
}

#[test]
fn test_parse_while_loop() {
    let source = r#"
while x > 0:
    x = x - 1
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    
    let mut parser = Parser::new(tokens.clone(), source);
    let program = parser.parse_program().unwrap();

    assert_eq!(program.len(), 1);
    if let Stmt::While(_, body) = &program[0] {
        if let Stmt::Block(stmts) = &**body {
            assert_eq!(stmts.len(), 1);
        } else { panic!("Body should be a block"); }
    } else { panic!("Should be a while loop"); }
}

#[test]
fn test_parse_directives() {
    let source = r#"
#pure
#[inline, optimize[3]]
fn fast():
    return
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    
    let mut parser = Parser::new(tokens.clone(), source);
    let program = parser.parse_program().unwrap();

    assert_eq!(program.len(), 1);
    if let Stmt::Function(f) = &program[0] {
        assert_eq!(f.directives.len(), 3); // #pure, #inline, #optimize[3]
        assert_eq!(f.name.lexeme, "fast");
    } else { panic!("Should be a function"); }
}

#[test]
fn test_parse_set_directive() {
    let source = r#"
#set[target[jit]]:
    fn x():
        return
"#.to_string();
    let mut scanner = Scanner::new(source.clone());
    let (tokens, _) = scanner.scan_tokens();
    
    let mut parser = Parser::new(tokens.clone(), source);
    let program = parser.parse_program().unwrap();

    assert_eq!(program.len(), 1);
    if let Stmt::Directive(lavina::parser::ast::Directive::Block(dirs, body)) = &program[0] {
        assert_eq!(dirs.len(), 1);
        assert_eq!(body.len(), 1);
    } else { panic!("Should be a #set block directive"); }
}
