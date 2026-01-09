use crate::lexer::{Token, TokenType};
use crate::parser::ast::{Expr, Literal, Stmt, Type, FunctionDecl, Directive};
use crate::error::{LavinaError, ErrorPhase};

pub struct Parser {
    tokens: Vec<Token>,
    source: String,
    current: usize,
}

impl Parser {
    pub fn new(tokens: Vec<Token>, source: String) -> Self {
        Self {
            tokens,
            source,
            current: 0,
        }
    }

    fn error(&self, message: String, line: usize, column: usize) -> LavinaError {
        LavinaError::new(ErrorPhase::Parser, message, line, column).with_context(&self.source)
    }

    pub fn parse_program(&mut self) -> Result<Vec<Stmt>, LavinaError> {
        let mut statements = Vec::new();
        while !self.is_at_end() {
            if self.match_types(&[TokenType::Newline]) {
                continue;
            }
            statements.push(self.declaration()?);
        }
        Ok(statements)
    }

    pub fn parse_expression(&mut self) -> Result<Expr, LavinaError> {
        self.expression()
    }

    fn declaration(&mut self) -> Result<Stmt, LavinaError> {
        let mut directives = Vec::new();

        while self.check(&TokenType::Hash) || self.check(&TokenType::HashBracket) {
            if self.check(&TokenType::Hash) && self.peek_next().token_type != TokenType::LeftBracket {
                let directive = self.single_directive()?;
                directives.push(directive);
                self.match_types(&[TokenType::Newline]);
            } else if self.check(&TokenType::HashBracket) {
                let mut list = self.directive_list()?;
                directives.append(&mut list);
                self.match_types(&[TokenType::Newline]);
            } else {
                break;
            }
        }

        if self.check(&TokenType::HashSet) {
            let dir = self.set_directive()?;
            return Ok(Stmt::Directive(dir));
        }

        if self.check_function_start() {
            return self.function_declaration(directives);
        }

        if self.match_types(&[TokenType::Let, TokenType::Auto, TokenType::Dynamic, 
                            TokenType::IntType, TokenType::FloatType, TokenType::StringType, TokenType::Bool,
                            TokenType::Vector, TokenType::HashMap]) {
            return self.var_declaration();
        }

        self.statement()
    }

    fn single_directive(&mut self) -> Result<Directive, LavinaError> {
        self.consume(TokenType::Hash, "Expect '#' before directive.")?;
        let name = self.consume_directive_name()?;
        
        if name == "doc" {
            let text = self.consume(TokenType::String, "Expect string after #doc.")?.lexeme.clone();
            return Ok(Directive::Doc(text));
        }

        if self.match_types(&[TokenType::LeftBracket]) {
            let mut args = Vec::new();
            if !self.check(&TokenType::RightBracket) {
                loop {
                    args.push(self.expression()?);
                    if !self.match_types(&[TokenType::Comma]) { break; }
                }
            }
            self.consume(TokenType::RightBracket, "Expect ']' after directive arguments.")?;
            return Ok(Directive::Parametric(name, args));
        }

        Ok(Directive::Simple(name))
    }

    fn directive_list(&mut self) -> Result<Vec<Directive>, LavinaError> {
        self.consume(TokenType::HashBracket, "Expect '#[' to start directive list.")?;
        let mut directives = Vec::new();
        
        loop {
            let name = self.consume_directive_name()?;
            if self.match_types(&[TokenType::LeftBracket]) {
                let mut args = Vec::new();
                if !self.check(&TokenType::RightBracket) {
                    loop {
                        args.push(self.expression()?);
                        if !self.match_types(&[TokenType::Comma]) { break; }
                    }
                }
                self.consume(TokenType::RightBracket, "Expect ']' after directive arguments.")?;
                directives.push(Directive::Parametric(name, args));
            } else {
                directives.push(Directive::Simple(name));
            }

            if !self.match_types(&[TokenType::Comma]) { break; }
        }

        self.consume(TokenType::RightBracket, "Expect ']' to end directive list.")?;
        Ok(directives)
    }

    fn set_directive(&mut self) -> Result<Directive, LavinaError> {
        self.consume(TokenType::HashSet, "Expect '#set[' to start block directive.")?;
        let mut directives = Vec::new();
        
        loop {
            let name = self.consume_directive_name()?;
            if self.match_types(&[TokenType::LeftBracket]) {
                let mut args = Vec::new();
                if !self.check(&TokenType::RightBracket) {
                    loop {
                        args.push(self.expression()?);
                        if !self.match_types(&[TokenType::Comma]) { break; }
                    }
                }
                self.consume(TokenType::RightBracket, "Expect ']' after directive arguments.")?;
                directives.push(Directive::Parametric(name, args));
            } else {
                directives.push(Directive::Simple(name));
            }

            if !self.match_types(&[TokenType::Comma]) { break; }
        }

        self.consume(TokenType::RightBracket, "Expect ']' after #set directives.")?;
        self.consume(TokenType::Colon, "Expect ':' before #set block.")?;
        
        let body = self.block()?;
        Ok(Directive::Block(directives, body))
    }

    fn consume_directive_name(&mut self) -> Result<String, LavinaError> {
        if self.match_types(&[TokenType::Identifier, TokenType::Inline, TokenType::Comptime]) {
            return Ok(self.previous().lexeme.clone());
        }
        let t = self.peek();
        Err(self.error(format!("Expect directive name. Got {}", t.token_type.to_string()), t.line, t.column))
    }

    fn check_function_start(&self) -> bool {
        let t = self.peek().token_type.clone();
        if t == TokenType::Fn || t == TokenType::Inline || t == TokenType::Comptime {
            return true;
        }
        if self.is_type_token(&t) && self.peek_next().token_type == TokenType::Fn {
            return true;
        }
        false
    }

    fn is_type_token(&self, t: &TokenType) -> bool {
        matches!(t, TokenType::IntType | TokenType::FloatType | TokenType::StringType | TokenType::Bool | TokenType::Void | TokenType::Auto | TokenType::Dynamic | TokenType::Vector | TokenType::HashMap)
    }

    fn function_declaration(&mut self, directives: Vec<Directive>) -> Result<Stmt, LavinaError> {
        let mut is_inline = false;
        let mut is_comptime = false;
        
        if self.match_types(&[TokenType::Inline]) { is_inline = true; }
        if self.match_types(&[TokenType::Comptime]) { is_comptime = true; }

        let mut return_type = Type::Void;
        if self.is_type_token(&self.peek().token_type) {
            return_type = self.parse_type()?;
        }

        self.consume(TokenType::Fn, "Expect 'fn' keyword.")?;
        let name = self.consume(TokenType::Identifier, "Expect function name.")?.clone();
        
        self.consume(TokenType::LeftParen, "Expect '(' after function name.")?;
        let mut params = Vec::new();
        if !self.check(&TokenType::RightParen) {
            loop {
                let param_type = self.parse_type()?;
                let param_name = self.consume(TokenType::Identifier, "Expect parameter name.")?.clone();
                params.push((param_name, param_type));
                if !self.match_types(&[TokenType::Comma]) { break; }
            }
        }
        self.consume(TokenType::RightParen, "Expect ')' after parameters.")?;

        if self.match_types(&[TokenType::Arrow]) {
            return_type = self.parse_type()?;
        }

        self.consume(TokenType::Colon, "Expect ':' before function body.")?;
        let body = self.block()?;

        Ok(Stmt::Function(FunctionDecl {
            name,
            params,
            return_type,
            body,
            directives,
            is_inline,
            is_comptime,
        }))
    }

    fn var_declaration(&mut self) -> Result<Stmt, LavinaError> {
        let token = self.previous().clone();
        
        let (name, final_type) = if token.token_type == TokenType::Let {
            let name = self.consume(TokenType::Identifier, "Expect variable name.")?.clone();
            let mut t = None;
            if self.match_types(&[TokenType::Colon]) {
                t = Some(self.parse_type()?);
            }
            (name, t)
        } else {
            let mut t = match token.token_type {
                TokenType::Auto => Type::Auto,
                TokenType::Dynamic => Type::Dynamic,
                TokenType::IntType => Type::Int,
                TokenType::FloatType => Type::Float,
                TokenType::StringType => Type::String,
                TokenType::Bool => Type::Bool,
                TokenType::Vector => {
                    self.consume(TokenType::LeftBracket, "Expect '[' after 'vector'.")?;
                    let inner = self.parse_type()?;
                    self.consume(TokenType::RightBracket, "Expect ']' after vector type.")?;
                    Type::Array(Box::new(inner))
                }
                TokenType::HashMap => {
                    self.consume(TokenType::LeftBracket, "Expect '[' after 'hashmap'.")?;
                    let key = self.parse_type()?;
                    self.consume(TokenType::Comma, "Expect ',' between key and value types in hashmap.")?;
                    let value = self.parse_type()?;
                    self.consume(TokenType::RightBracket, "Expect ']' after hashmap types.")?;
                    Type::Dict(Box::new(key), Box::new(value))
                }
                _ => {
                    let t = self.peek();
                    return Err(self.error(format!("Unexpected token in variable declaration: {:?}", token), t.line, t.column));
                }
            };

            if self.match_types(&[TokenType::Question]) {
                t = Type::Nullable(Box::new(t));
            }

            let name = self.consume(TokenType::Identifier, "Expect variable name.")?.clone();
            (name, Some(t))
        };

        let mut initializer = None;
        if self.match_types(&[TokenType::Equal]) {
            initializer = Some(self.expression()?);
        }

        self.match_types(&[TokenType::Semicolon, TokenType::Newline]);
        Ok(Stmt::Let(name, final_type, initializer))
    }

    fn statement(&mut self) -> Result<Stmt, LavinaError> {
        if self.match_types(&[TokenType::If]) { return self.if_statement(); }
        if self.match_types(&[TokenType::While]) { return self.while_statement(); }
        if self.match_types(&[TokenType::For]) { return self.for_statement(); }
        if self.match_types(&[TokenType::Return]) { return self.return_statement(); }
        
        self.expression_statement()
    }

    fn if_statement(&mut self) -> Result<Stmt, LavinaError> {
        let condition = self.expression()?;
        self.consume(TokenType::Colon, "Expect ':' after if condition.")?;
        let then_branch = Box::new(Stmt::Block(self.block()?));
        
        let mut else_branch = None;
        if self.match_types(&[TokenType::Else]) {
            if self.match_types(&[TokenType::Colon]) {
                else_branch = Some(Box::new(Stmt::Block(self.block()?)));
            } else if self.check(&TokenType::If) {
                else_branch = Some(Box::new(self.declaration()?));
            }
        }

        Ok(Stmt::If(condition, then_branch, else_branch))
    }

    fn while_statement(&mut self) -> Result<Stmt, LavinaError> {
        let condition = self.expression()?;
        self.consume(TokenType::Colon, "Expect ':' after while condition.")?;
        let body = Box::new(Stmt::Block(self.block()?));
        Ok(Stmt::While(condition, body))
    }

    fn for_statement(&mut self) -> Result<Stmt, LavinaError> {
        let item_name = self.consume(TokenType::Identifier, "Expect variable name after 'for'.")?.clone();
        self.consume(TokenType::In, "Expect 'in' after variable name.")?;
        let collection = self.expression()?;
        self.consume(TokenType::Colon, "Expect ':' after for loop header.")?;
        let body = Box::new(Stmt::Block(self.block()?));
        Ok(Stmt::For(item_name, collection, body))
    }

    fn return_statement(&mut self) -> Result<Stmt, LavinaError> {
        let keyword = self.previous().clone();
        let mut value = None;
        if !self.check(&TokenType::Newline) && !self.check(&TokenType::Dedent) && !self.is_at_end() {
            value = Some(self.expression()?);
        }
        self.match_types(&[TokenType::Newline]);
        Ok(Stmt::Return(keyword, value))
    }

    fn block(&mut self) -> Result<Vec<Stmt>, LavinaError> {
        self.match_types(&[TokenType::Newline]);
        self.consume(TokenType::Indent, "Expect indentation to start a block.")?;
        
        let mut statements = Vec::new();
        while !self.check(&TokenType::Dedent) && !self.is_at_end() {
            if self.match_types(&[TokenType::Newline]) { continue; }
            statements.push(self.declaration()?);
        }

        self.consume(TokenType::Dedent, "Expect dedent to end a block.")?;
        Ok(statements)
    }

    fn expression_statement(&mut self) -> Result<Stmt, LavinaError> {
        let expr = self.expression()?;
        self.match_types(&[TokenType::Semicolon, TokenType::Newline]);
        Ok(Stmt::Expression(expr))
    }

    fn expression(&mut self) -> Result<Expr, LavinaError> {
        self.assignment()
    }

    fn assignment(&mut self) -> Result<Expr, LavinaError> {
        let expr = self.logical_or()?;

        if self.match_types(&[TokenType::Equal]) {
            let equals = self.previous().clone();
            let value = self.assignment()?;

            match expr {
                Expr::Variable(name) => return Ok(Expr::Assign(name, Box::new(value))),
                Expr::Index(_coll, bracket, _idx) => {
                    // For now we don't have AssignIndex in AST, let's just use regular assign 
                    // or maybe we should add it? Actually, collection[idx] = val 
                    // is a special case. Let's just return error for now to keep it simple
                    // or handle it later.
                    return Err(self.error("Index assignment not yet implemented.".to_string(), bracket.line, bracket.column));
                }
                _ => return Err(self.error("Invalid assignment target.".to_string(), equals.line, equals.column)),
            }
        }

        Ok(expr)
    }

    fn logical_or(&mut self) -> Result<Expr, LavinaError> {
        let mut expr = self.logical_and()?;
        while self.match_types(&[TokenType::Or]) {
            let operator = self.previous().clone();
            let right = self.logical_and()?;
            expr = Expr::Logical(Box::new(expr), operator, Box::new(right));
        }
        Ok(expr)
    }

    fn logical_and(&mut self) -> Result<Expr, LavinaError> {
        let mut expr = self.equality()?;
        while self.match_types(&[TokenType::And]) {
            let operator = self.previous().clone();
            let right = self.equality()?;
            expr = Expr::Logical(Box::new(expr), operator, Box::new(right));
        }
        Ok(expr)
    }

    fn equality(&mut self) -> Result<Expr, LavinaError> {
        let mut expr = self.comparison()?;
        while self.match_types(&[TokenType::BangEqual, TokenType::EqualEqual]) {
            let operator = self.previous().clone();
            let right = self.comparison()?;
            expr = Expr::Binary(Box::new(expr), operator, Box::new(right));
        }
        Ok(expr)
    }

    fn comparison(&mut self) -> Result<Expr, LavinaError> {
        let mut expr = self.term()?;
        while self.match_types(&[TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual]) {
            let operator = self.previous().clone();
            let right = self.term()?;
            expr = Expr::Binary(Box::new(expr), operator, Box::new(right));
        }
        Ok(expr)
    }

    fn term(&mut self) -> Result<Expr, LavinaError> {
        let mut expr = self.factor()?;
        while self.match_types(&[TokenType::Minus, TokenType::Plus]) {
            let operator = self.previous().clone();
            let right = self.factor()?;
            expr = Expr::Binary(Box::new(expr), operator, Box::new(right));
        }
        Ok(expr)
    }

    fn factor(&mut self) -> Result<Expr, LavinaError> {
        let mut expr = self.unary()?;
        while self.match_types(&[TokenType::Slash, TokenType::Star]) {
            let operator = self.previous().clone();
            let right = self.unary()?;
            expr = Expr::Binary(Box::new(expr), operator, Box::new(right));
        }
        Ok(expr)
    }

    fn unary(&mut self) -> Result<Expr, LavinaError> {
        if self.match_types(&[TokenType::Bang, TokenType::Minus]) {
            let operator = self.previous().clone();
            let right = self.unary()?;
            return Ok(Expr::Unary(operator, Box::new(right)));
        }
        self.call()
    }

    fn call(&mut self) -> Result<Expr, LavinaError> {
        let mut expr = self.primary()?;
        loop {
            if self.match_types(&[TokenType::LeftParen]) {
                expr = self.finish_call(expr)?;
            } else if self.match_types(&[TokenType::LeftBracket]) {
                let index = self.expression()?;
                let bracket = self.consume(TokenType::RightBracket, "Expect ']' after index.")?.clone();
                expr = Expr::Index(Box::new(expr), bracket, Box::new(index));
            } else {
                break;
            }
        }
        Ok(expr)
    }

    fn finish_call(&mut self, callee: Expr) -> Result<Expr, LavinaError> {
        let mut arguments = Vec::new();
        if !self.check(&TokenType::RightParen) {
            loop {
                arguments.push(self.expression()?);
                if !self.match_types(&[TokenType::Comma]) { break; }
            }
        }
        let paren = self.consume(TokenType::RightParen, "Expect ')' after arguments.")?.clone();
        Ok(Expr::Call(Box::new(callee), paren, arguments))
    }

    fn primary(&mut self) -> Result<Expr, LavinaError> {
        if self.match_types(&[TokenType::False]) { return Ok(Expr::Literal(Literal::Bool(false))); }
        if self.match_types(&[TokenType::True]) { return Ok(Expr::Literal(Literal::Bool(true))); }
        if self.match_types(&[TokenType::Null]) { return Ok(Expr::Literal(Literal::Null)); }

        if self.match_types(&[TokenType::Int]) {
            let value = self.previous().lexeme.parse::<i64>().map_err(|_| self.error("Invalid integer".to_string(), self.previous().line, self.previous().column))?;
            return Ok(Expr::Literal(Literal::Int(value)));
        }

        if self.match_types(&[TokenType::Float]) {
            let value = self.previous().lexeme.parse::<f64>().map_err(|_| self.error("Invalid float".to_string(), self.previous().line, self.previous().column))?;
            return Ok(Expr::Literal(Literal::Float(value)));
        }

        if self.match_types(&[TokenType::String]) {
            return Ok(Expr::Literal(Literal::String(self.previous().lexeme.clone())));
        }

        if self.match_types(&[TokenType::Identifier]) {
            return Ok(Expr::Variable(self.previous().clone()));
        }

        if self.match_types(&[TokenType::LeftParen]) {
            let expr = self.expression()?;
            self.consume(TokenType::RightParen, "Expect ')' after expression.")?;
            return Ok(Expr::Grouping(Box::new(expr)));
        }

        if self.match_types(&[TokenType::LeftBracket]) {
            let mut elements = Vec::new();
            self.skip_formatting();
            if !self.check(&TokenType::RightBracket) {
                loop {
                    self.skip_formatting();
                    elements.push(self.expression()?);
                    self.skip_formatting();
                    if !self.match_types(&[TokenType::Comma]) { break; }
                }
            }
            self.skip_formatting();
            self.consume(TokenType::RightBracket, "Expect ']' after vector literal.")?;
            return Ok(Expr::Vector(elements));
        }

        if self.match_types(&[TokenType::LeftBrace]) {
            let mut entries = Vec::new();
            self.skip_formatting();
            if !self.check(&TokenType::RightBrace) {
                loop {
                    self.skip_formatting();
                    let key = self.expression()?;
                    self.skip_formatting();
                    self.consume(TokenType::Colon, "Expect ':' after map key.")?;
                    self.skip_formatting();
                    let value = self.expression()?;
                    self.skip_formatting();
                    entries.push((key, value));
                    self.skip_formatting();
                    if !self.match_types(&[TokenType::Comma]) { break; }
                }
            }
            self.skip_formatting();
            self.consume(TokenType::RightBrace, "Expect '}' after map literal.")?;
            return Ok(Expr::Map(entries));
        }

        let t = self.peek();
        Err(self.error(format!("Expect expression. Got {}", t.token_type.to_string()), t.line, t.column))
    }

    fn parse_type(&mut self) -> Result<Type, LavinaError> {
        let mut t = if self.match_types(&[TokenType::IntType]) { Type::Int }
        else if self.match_types(&[TokenType::FloatType]) { Type::Float }
        else if self.match_types(&[TokenType::StringType]) { Type::String }
        else if self.match_types(&[TokenType::Bool]) { Type::Bool }
        else if self.match_types(&[TokenType::Void]) { Type::Void }
        else if self.match_types(&[TokenType::Auto]) { Type::Auto }
        else if self.match_types(&[TokenType::Dynamic]) { Type::Dynamic }
        else if self.match_types(&[TokenType::Vector]) {
            self.consume(TokenType::LeftBracket, "Expect '[' after 'vector'.")?;
            let inner = self.parse_type()?;
            self.consume(TokenType::RightBracket, "Expect ']' after vector type.")?;
            Type::Array(Box::new(inner))
        } else if self.match_types(&[TokenType::HashMap]) {
            self.consume(TokenType::LeftBracket, "Expect '[' after 'hashmap'.")?;
            let key = self.parse_type()?;
            self.consume(TokenType::Comma, "Expect ',' between key and value types in hashmap.")?;
            let value = self.parse_type()?;
            self.consume(TokenType::RightBracket, "Expect ']' after hashmap types.")?;
            Type::Dict(Box::new(key), Box::new(value))
        } else if self.match_types(&[TokenType::Identifier]) {
            Type::Custom(self.previous().lexeme.clone())
        } else {
            let t = self.peek();
            return Err(self.error(format!("Expect type. Got {}", t.token_type.to_string()), t.line, t.column));
        };

        if self.match_types(&[TokenType::Question]) {
            t = Type::Nullable(Box::new(t));
        }

        Ok(t)
    }

    fn skip_formatting(&mut self) {
        while self.match_types(&[TokenType::Newline, TokenType::Indent, TokenType::Dedent]) {}
    }

    fn match_types(&mut self, types: &[TokenType]) -> bool {
        for t in types {
            if self.check(t) {
                self.advance();
                return true;
            }
        }
        false
    }

    fn check(&self, t_type: &TokenType) -> bool {
        if self.is_at_end() { return false; }
        self.peek().token_type == *t_type
    }

    fn advance(&mut self) -> &Token {
        if !self.is_at_end() { self.current += 1; }
        self.previous()
    }

    pub fn is_at_end(&self) -> bool {
        self.peek().token_type == TokenType::Eof
    }

    fn peek(&self) -> &Token {
        &self.tokens[self.current]
    }

    fn peek_next(&self) -> &Token {
        if self.is_at_end() { return self.peek(); }
        &self.tokens[self.current + 1]
    }

    fn previous(&self) -> &Token {
        &self.tokens[self.current - 1]
    }

    fn consume(&mut self, t_type: TokenType, message: &str) -> Result<&Token, LavinaError> {
        if self.check(&t_type) { return Ok(self.advance()); }
        let t = self.peek();
        Err(self.error(format!("{} Got {}", message, t.token_type.to_string()), t.line, t.column))
    }
}
