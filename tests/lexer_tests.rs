use lavina::lexer::{Scanner, TokenType};

#[test]
fn test_basic_tokens() {
    let source = "int x = 10 + 5.5;".to_string();
    let mut scanner = Scanner::new(source);
    let (tokens, errors) = scanner.scan_tokens();

    assert!(errors.is_empty());
    assert_eq!(tokens[0].token_type, TokenType::IntType);
    assert_eq!(tokens[1].token_type, TokenType::Identifier);
    assert_eq!(tokens[3].token_type, TokenType::Int);
}

#[test]
fn test_unexpected_character() {
    let source = "int x = @;".to_string();
    let mut scanner = Scanner::new(source);
    let (_, errors) = scanner.scan_tokens();

    assert_eq!(errors.len(), 1);
    assert!(errors[0].message.contains("Unexpected character: '@'"));
}

#[test]
fn test_unterminated_string() {
    let source = "string s = \"hello;".to_string();
    let mut scanner = Scanner::new(source);
    let (_, errors) = scanner.scan_tokens();

    assert_eq!(errors.len(), 1);
    assert!(errors[0].message.contains("Unterminated string"));
}

#[test]
fn test_inconsistent_indentation() {
    let source = "
fn main():
    int x = 1
   int y = 2
".to_string();
    let mut scanner = Scanner::new(source);
    let (_, errors) = scanner.scan_tokens();

    assert_eq!(errors.len(), 1);
    assert!(errors[0].message.contains("Inconsistent indentation"));
}

#[test]
fn test_nested_indentation() {
    let source = "
if a:
    if b:
        c
    d
".to_string();
    let mut scanner = Scanner::new(source);
    let (tokens, _) = scanner.scan_tokens();
    let types: Vec<TokenType> = tokens.iter().map(|t| t.token_type.clone()).collect();

    let indents = types.iter().filter(|t| **t == TokenType::Indent).count();
    let dedents = types.iter().filter(|t| **t == TokenType::Dedent).count();

    assert_eq!(indents, 2);
    assert_eq!(dedents, 2);
}
