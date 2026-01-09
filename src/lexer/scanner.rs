use crate::lexer::token::{Token, TokenType};
use crate::error::{LavinaError, ErrorPhase};
use std::collections::HashMap;

pub struct Scanner {
    source: Vec<char>,
    raw_source: String,
    tokens: Vec<Token>,
    errors: Vec<LavinaError>,
    start: usize,
    current: usize,
    line: usize,
    column: usize,
    start_line: usize,
    start_column: usize,
    keywords: HashMap<String, TokenType>,
    indent_stack: Vec<usize>,
    at_line_start: bool,
}

impl Scanner {
    pub fn new(source: String) -> Self {
        let mut keywords = HashMap::new();
        keywords.insert("and".to_string(), TokenType::And);
        keywords.insert("auto".to_string(), TokenType::Auto);
        keywords.insert("bool".to_string(), TokenType::Bool);
        keywords.insert("comptime".to_string(), TokenType::Comptime);
        keywords.insert("dynamic".to_string(), TokenType::Dynamic);
        keywords.insert("else".to_string(), TokenType::Else);
        keywords.insert("false".to_string(), TokenType::False);
        keywords.insert("float".to_string(), TokenType::FloatType);
        keywords.insert("fn".to_string(), TokenType::Fn);
        keywords.insert("if".to_string(), TokenType::If);
        keywords.insert("inline".to_string(), TokenType::Inline);
        keywords.insert("int".to_string(), TokenType::IntType);
        keywords.insert("let".to_string(), TokenType::Let);
        keywords.insert("null".to_string(), TokenType::Null);
        keywords.insert("or".to_string(), TokenType::Or);
        keywords.insert("return".to_string(), TokenType::Return);
        keywords.insert("string".to_string(), TokenType::StringType);
        keywords.insert("true".to_string(), TokenType::True);
        keywords.insert("void".to_string(), TokenType::Void);
        keywords.insert("while".to_string(), TokenType::While);
        keywords.insert("for".to_string(), TokenType::For);
        keywords.insert("in".to_string(), TokenType::In);
        keywords.insert("vector".to_string(), TokenType::Vector);
        keywords.insert("hashmap".to_string(), TokenType::HashMap);

        Self {
            source: source.chars().collect(),
            raw_source: source,
            tokens: Vec::new(),
            errors: Vec::new(),
            start: 0,
            current: 0,
            line: 1,
            column: 1,
            start_line: 1,
            start_column: 1,
            keywords,
            indent_stack: vec![0],
            at_line_start: true,
        }
    }

    pub fn scan_tokens(&mut self) -> (&Vec<Token>, &Vec<LavinaError>) {
        while !self.is_at_end() {
            self.start = self.current;
            self.start_line = self.line;
            self.start_column = self.column;
            self.scan_token();
        }

        while self.indent_stack.len() > 1 {
            self.indent_stack.pop();
            self.add_token_with_lexeme(TokenType::Dedent, "".to_string());
        }

        self.tokens.push(Token::new(TokenType::Eof, "".to_string(), self.line, self.column));
        (&self.tokens, &self.errors)
    }

    fn scan_token(&mut self) {
        if self.at_line_start {
            self.handle_indentation();
            self.start = self.current;
            self.start_line = self.line;
            self.start_column = self.column;
        }

        if self.is_at_end() { return; }

        let c = self.advance();
        match c {
            '(' => self.add_token(TokenType::LeftParen),
            ')' => self.add_token(TokenType::RightParen),
            '[' => self.add_token(TokenType::LeftBracket),
            ']' => self.add_token(TokenType::RightBracket),
            '{' => self.add_token(TokenType::LeftBrace),
            '}' => self.add_token(TokenType::RightBrace),
            ',' => self.add_token(TokenType::Comma),
            '.' => self.add_token(TokenType::Dot),
            '?' => self.add_token(TokenType::Question),
            '-' => {
                if self.match_char('>') {
                    self.add_token(TokenType::Arrow);
                } else {
                    self.add_token(TokenType::Minus);
                }
            }
            '+' => self.add_token(TokenType::Plus),
            ';' => self.add_token(TokenType::Semicolon),
            '*' => self.add_token(TokenType::Star),
            ':' => self.add_token(TokenType::Colon),
            '#' => {
                if self.match_char('[') {
                    self.add_token(TokenType::HashBracket);
                } else if self.match_keyword_at_pos("set") {
                    self.current += 3;
                    self.column += 3;
                    if self.match_char('[') {
                        self.add_token(TokenType::HashSet);
                    } else {
                        self.current -= 3;
                        self.column -= 3;
                        self.add_token(TokenType::Hash);
                    }
                } else {
                    self.add_token(TokenType::Hash);
                }
            }
            '!' => {
                if self.match_char('=') {
                    self.add_token(TokenType::BangEqual);
                } else {
                    self.add_token(TokenType::Bang);
                }
            }
            '=' => {
                if self.match_char('=') {
                    self.add_token(TokenType::EqualEqual);
                } else {
                    self.add_token(TokenType::Equal);
                }
            }
            '<' => {
                if self.match_char('=') {
                    self.add_token(TokenType::LessEqual);
                } else {
                    self.add_token(TokenType::Less);
                }
            }
            '>' => {
                if self.match_char('=') {
                    self.add_token(TokenType::GreaterEqual);
                } else {
                    self.add_token(TokenType::Greater);
                }
            }
            '/' => {
                if self.match_char('/') {
                    while self.peek() != '\n' && !self.is_at_end() {
                        self.advance();
                    }
                } else {
                    self.add_token(TokenType::Slash);
                }
            }
            ' ' | '\r' | '\t' => {}
            '\n' => {
                self.add_token_with_lexeme(TokenType::Newline, "\n".to_string());
                self.line += 1;
                self.column = 1;
                self.at_line_start = true;
            }
            '"' => self.string(),
            _ => {
                if c.is_digit(10) {
                    self.number();
                } else if c.is_alphabetic() || c == '_' {
                    self.identifier();
                } else {
                    let err = LavinaError::new(
                        ErrorPhase::Lexer,
                        format!("Unexpected character: '{}'", c),
                        self.line,
                        self.column - 1,
                    ).with_context(&self.raw_source);
                    self.errors.push(err);
                }
            }
        }
    }

    fn handle_indentation(&mut self) {
        let mut indent = 0;
        while !self.is_at_end() && (self.peek() == ' ' || self.peek() == '\t') {
            let c = self.advance();
            if c == ' ' {
                indent += 1;
            } else {
                indent += 4;
            }
        }

        if self.is_at_end() || self.peek() == '\n' || (self.peek() == '/' && self.peek_next() == '/') {
            self.at_line_start = self.peek() == '\n';
            return;
        }

        let current_indent = *self.indent_stack.last().unwrap();
        if indent > current_indent {
            self.indent_stack.push(indent);
            self.add_token_with_lexeme(TokenType::Indent, "".to_string());
        } else if indent < current_indent {
            while indent < *self.indent_stack.last().unwrap() {
                self.indent_stack.pop();
                self.add_token_with_lexeme(TokenType::Dedent, "".to_string());
            }
            if indent != *self.indent_stack.last().unwrap() {
                let err = LavinaError::new(
                    ErrorPhase::Lexer,
                    format!("Inconsistent indentation: expected {} spaces, but found {}", self.indent_stack.last().unwrap(), indent),
                    self.line,
                    1,
                ).with_context(&self.raw_source);
                self.errors.push(err);
            }
        }
        self.at_line_start = false;
    }

    fn identifier(&mut self) {
        while self.peek().is_alphanumeric() || self.peek() == '_' {
            self.advance();
        }

        let text: String = self.source[self.start..self.current].iter().collect();
        let token_type = self.keywords.get(&text).cloned().unwrap_or(TokenType::Identifier);
        self.add_token(token_type);
    }

    fn number(&mut self) {
        while self.peek().is_digit(10) {
            self.advance();
        }

        if self.peek() == '.' && self.peek_next().is_digit(10) {
            self.advance();
            while self.peek().is_digit(10) {
                self.advance();
            }
            self.add_token(TokenType::Float);
        } else {
            self.add_token(TokenType::Int);
        }
    }

    fn string(&mut self) {
        while self.peek() != '"' && !self.is_at_end() {
            if self.peek() == '\n' {
                self.line += 1;
                self.column = 1;
            }
            self.advance();
        }

        if self.is_at_end() {
            let err = LavinaError::new(
                ErrorPhase::Lexer,
                "Unterminated string".to_string(),
                self.line,
                self.column,
            ).with_context(&self.raw_source);
            self.errors.push(err);
            return;
        }

        self.advance();
        let value: String = self.source[self.start + 1..self.current - 1].iter().collect();
        self.add_token_with_lexeme(TokenType::String, value);
    }

    fn match_keyword_at_pos(&self, keyword: &str) -> bool {
        if self.current + keyword.len() > self.source.len() {
            return false;
        }
        let next_chars: String = self.source[self.current..self.current + keyword.len()].iter().collect();
        next_chars == keyword
    }

    fn match_char(&mut self, expected: char) -> bool {
        if self.is_at_end() {
            return false;
        }
        if self.source[self.current] != expected {
            return false;
        }

        self.current += 1;
        self.column += 1;
        true
    }

    fn peek(&self) -> char {
        if self.is_at_end() {
            '\0'
        } else {
            self.source[self.current]
        }
    }

    fn peek_next(&self) -> char {
        if self.current + 1 >= self.source.len() {
            '\0'
        } else {
            self.source[self.current + 1]
        }
    }

    fn is_at_end(&self) -> bool {
        self.current >= self.source.len()
    }

    fn advance(&mut self) -> char {
        let c = self.source[self.current];
        self.current += 1;
        self.column += 1;
        c
    }

    fn add_token(&mut self, token_type: TokenType) {
        self.tokens.push(Token::new(token_type, self.source[self.start..self.current].iter().collect(), self.start_line, self.start_column));
    }

    fn add_token_with_lexeme(&mut self, token_type: TokenType, lexeme: String) {
        self.tokens.push(Token::new(token_type, lexeme, self.start_line, self.start_column));
    }
}
