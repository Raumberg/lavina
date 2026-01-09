use crate::parser::ast::{Expr, Stmt, Literal, Type};
use crate::lexer::TokenType;
use crate::error::{LavinaError, ErrorPhase};
use std::collections::HashMap;

#[derive(Clone, Debug, PartialEq)]
pub enum TypeInfo {
    Primitive(Type),
    Function {
        params: Vec<Type>,
        return_type: Box<Type>,
        is_variadic: bool,
    },
}

pub struct TypeEnvironment {
    types: HashMap<String, TypeInfo>,
    enclosing: Option<Box<TypeEnvironment>>,
}

impl TypeEnvironment {
    pub fn new() -> Self {
        let mut env = Self {
            types: HashMap::new(),
            enclosing: None,
        };
        // Add native functions
        env.define("print".to_string(), TypeInfo::Function {
            params: vec![Type::Dynamic],
            return_type: Box::new(Type::Void),
            is_variadic: true, // print can take many arguments
        });
        env.define("len".to_string(), TypeInfo::Function {
            params: vec![Type::Dynamic],
            return_type: Box::new(Type::Int),
            is_variadic: false,
        });
        env.define("clock".to_string(), TypeInfo::Function {
            params: vec![],
            return_type: Box::new(Type::Float),
            is_variadic: false,
        });
        env.define("type".to_string(), TypeInfo::Function {
            params: vec![Type::Dynamic],
            return_type: Box::new(Type::String),
            is_variadic: false,
        });
        env
    }

    pub fn define(&mut self, name: String, type_info: TypeInfo) {
        self.types.insert(name, type_info);
    }

    pub fn get(&self, name: &str) -> Option<TypeInfo> {
        if let Some(t) = self.types.get(name) {
            Some(t.clone())
        } else if let Some(enclosing) = &self.enclosing {
            enclosing.get(name)
        } else {
            None
        }
    }
}

pub struct TypeChecker {
    pub env: TypeEnvironment,
    pub source: String,
}

impl TypeChecker {
    pub fn new(source: String) -> Self {
        Self {
            env: TypeEnvironment::new(),
            source,
        }
    }

    fn error(&self, message: String, line: usize, column: usize) -> LavinaError {
        LavinaError::new(ErrorPhase::TypeChecker, message, line, column).with_context(&self.source)
    }

    pub fn check(&mut self, statements: &[Stmt]) -> Result<(), LavinaError> {
        for stmt in statements {
            if let Stmt::Function(func) = stmt {
                let param_types: Vec<Type> = func.params.iter().map(|(_, t)| t.clone()).collect();
                self.env.define(func.name.lexeme.clone(), TypeInfo::Function {
                    params: param_types,
                    return_type: Box::new(func.return_type.clone()),
                    is_variadic: false,
                });
            }
        }

        for stmt in statements {
            self.check_stmt(stmt)?;
        }
        Ok(())
    }

    fn check_stmt(&mut self, stmt: &Stmt) -> Result<(), LavinaError> {
        match stmt {
            Stmt::Expression(expr) => {
                self.check_expr(expr)?;
                Ok(())
            }
            Stmt::Let(name, type_annotation, initializer) => {
                let mut inferred_type = Type::Auto;
                if let Some(init) = initializer {
                    inferred_type = self.check_expr(init)?;
                }

                let final_type = if let Some(annotated) = type_annotation {
                    if *annotated == Type::Auto {
                        inferred_type
                    } else if !self.is_assignable(annotated, &inferred_type) {
                        return Err(self.error(
                            format!("Type mismatch for variable '{}': expected {:?}, but got {:?}", name.lexeme, annotated, inferred_type),
                            name.line, name.column
                        ));
                    } else {
                        annotated.clone()
                    }
                } else {
                    inferred_type
                };

                self.env.define(name.lexeme.clone(), TypeInfo::Primitive(final_type));
                Ok(())
            }
            Stmt::Function(func) => {
                let mut inner_env = TypeEnvironment {
                    types: HashMap::new(),
                    enclosing: Some(Box::new(std::mem::replace(&mut self.env, TypeEnvironment::new()))),
                };
                
                self.env = *inner_env.enclosing.take().unwrap();

                let mut checker = TypeChecker {
                    env: inner_env,
                    source: self.source.clone(),
                };
                
                for (param_name, param_type) in &func.params {
                    checker.env.define(param_name.lexeme.clone(), TypeInfo::Primitive(param_type.clone()));
                }

                for body_stmt in &func.body {
                    checker.check_stmt(body_stmt)?;
                }
                Ok(())
            }
            Stmt::If(condition, then_branch, else_branch) => {
                let cond_type = self.check_expr(condition)?;
                if !matches!(cond_type, Type::Bool | Type::Dynamic) {
                    return Err(self.error("If condition must be a boolean".to_string(), 0, 0)); 
                }
                self.check_stmt(then_branch)?;
                if let Some(else_stmt) = else_branch {
                    self.check_stmt(else_stmt)?;
                }
                Ok(())
            }
            Stmt::While(condition, body) => {
                let cond_type = self.check_expr(condition)?;
                if !matches!(cond_type, Type::Bool | Type::Dynamic) {
                    return Err(self.error("While condition must be a boolean".to_string(), 0, 0));
                }
                self.check_stmt(body)?;
                Ok(())
            }
            Stmt::Block(stmts) => {
                let mut inner_env = TypeEnvironment {
                    types: HashMap::new(),
                    enclosing: Some(Box::new(std::mem::replace(&mut self.env, TypeEnvironment::new()))),
                };
                self.env = *inner_env.enclosing.take().unwrap();
                let mut checker = TypeChecker {
                    env: inner_env,
                    source: self.source.clone(),
                };
                for stmt in stmts {
                    checker.check_stmt(stmt)?;
                }
                Ok(())
            }
            Stmt::Return(_, value) => {
                if let Some(expr) = value {
                    self.check_expr(expr)?;
                }
                Ok(())
            }
            Stmt::Directive(_) => Ok(()),
        }
    }

    pub fn check_expr(&self, expr: &Expr) -> Result<Type, LavinaError> {
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(_) => Ok(Type::Int),
                Literal::Float(_) => Ok(Type::Float),
                Literal::String(_) => Ok(Type::String),
                Literal::Bool(_) => Ok(Type::Bool),
                Literal::Null => Ok(Type::Dynamic),
            },
            Expr::Variable(name) => {
                if let Some(type_info) = self.env.get(&name.lexeme) {
                    match type_info {
                        TypeInfo::Primitive(t) => Ok(t),
                        TypeInfo::Function { return_type, .. } => Ok(*return_type.clone()),
                    }
                } else {
                    Err(self.error(format!("Undefined variable '{}'", name.lexeme), name.line, name.column))
                }
            }
            Expr::Assign(name, value) => {
                let value_type = self.check_expr(value)?;
                if let Some(type_info) = self.env.get(&name.lexeme) {
                    match type_info {
                        TypeInfo::Primitive(target_type) => {
                            if self.is_assignable(&target_type, &value_type) {
                                Ok(target_type)
                            } else {
                                Err(self.error(format!("Cannot assign {:?} to variable '{}' of type {:?}", value_type, name.lexeme, target_type), name.line, name.column))
                            }
                        }
                        _ => Err(self.error(format!("Cannot assign to function '{}'", name.lexeme), name.line, name.column)),
                    }
                } else {
                    Err(self.error(format!("Undefined variable '{}'", name.lexeme), name.line, name.column))
                }
            }
            Expr::Binary(left, op, right) => {
                let lt = self.check_expr(left)?;
                let rt = self.check_expr(right)?;
                match op.token_type {
                    TokenType::Plus | TokenType::Minus | TokenType::Star | TokenType::Slash => {
                        if (lt == Type::Int || lt == Type::Float) && (rt == Type::Int || rt == Type::Float) {
                            if lt == Type::Float || rt == Type::Float { Ok(Type::Float) } else { Ok(Type::Int) }
                        } else if lt == Type::String && rt == Type::String && op.token_type == TokenType::Plus {
                            Ok(Type::String)
                        } else {
                            Err(self.error(format!("Invalid binary operands: {:?} and {:?}", lt, rt), op.line, op.column))
                        }
                    }
                    TokenType::Greater | TokenType::GreaterEqual | TokenType::Less | TokenType::LessEqual => {
                        Ok(Type::Bool)
                    }
                    TokenType::EqualEqual | TokenType::BangEqual => {
                        Ok(Type::Bool)
                    }
                    _ => Ok(Type::Dynamic),
                }
            }
            Expr::Unary(op, right) => {
                let rt = self.check_expr(right)?;
                match op.token_type {
                    TokenType::Minus => {
                        if rt == Type::Int || rt == Type::Float { Ok(rt) }
                        else { Err(self.error(format!("Invalid unary operand: {:?}", rt), op.line, op.column)) }
                    }
                    TokenType::Bang => Ok(Type::Bool),
                    _ => Ok(Type::Dynamic),
                }
            }
            Expr::Logical(left, _, right) => {
                self.check_expr(left)?;
                self.check_expr(right)?;
                Ok(Type::Bool)
            }
            Expr::Grouping(e) => self.check_expr(e),
            Expr::Call(callee, paren, args) => {
                if let Expr::Variable(name) = &**callee {
                    if let Some(TypeInfo::Function { params, return_type, is_variadic }) = self.env.get(&name.lexeme) {
                        if is_variadic {
                            // Check that we have at least as many args as non-variadic params
                            if args.len() < params.len() {
                                return Err(self.error(format!("Expected at least {} arguments but got {}", params.len(), args.len()), paren.line, paren.column));
                            }
                            // Check fixed params
                            for (i, p) in params.iter().enumerate() {
                                let arg_type = self.check_expr(&args[i])?;
                                if !self.is_assignable(p, &arg_type) {
                                    return Err(self.error(format!("Argument {}: expected {:?}, but got {:?}", i + 1, p, arg_type), paren.line, paren.column));
                                }
                            }
                            // Check variadic args against the LAST param type (Dynamic for print)
                            let var_type = params.last().unwrap();
                            for i in params.len()..args.len() {
                                let arg_type = self.check_expr(&args[i])?;
                                if !self.is_assignable(var_type, &arg_type) {
                                    return Err(self.error(format!("Variadic argument {}: expected {:?}, but got {:?}", i + 1, var_type, arg_type), paren.line, paren.column));
                                }
                            }
                        } else {
                            if params.len() != args.len() {
                                return Err(self.error(format!("Expected {} arguments but got {}", params.len(), args.len()), paren.line, paren.column));
                            }
                            for (i, arg) in args.iter().enumerate() {
                                let arg_type = self.check_expr(arg)?;
                                if !self.is_assignable(&params[i], &arg_type) {
                                    return Err(self.error(format!("Argument {}: expected {:?}, but got {:?}", i + 1, params[i], arg_type), paren.line, paren.column));
                                }
                            }
                        }
                        return Ok(*return_type);
                    }
                }
                Ok(Type::Dynamic)
            }
            Expr::Vector(elements) => {
                if elements.is_empty() {
                    return Ok(Type::Array(Box::new(Type::Dynamic)));
                }
                let first_type = self.check_expr(&elements[0])?;
                for el in elements.iter().skip(1) {
                    let t = self.check_expr(el)?;
                    if t != first_type {
                        return Ok(Type::Array(Box::new(Type::Dynamic)));
                    }
                }
                Ok(Type::Array(Box::new(first_type)))
            }
            Expr::Map(entries) => {
                if entries.is_empty() {
                    return Ok(Type::Dict(Box::new(Type::Dynamic), Box::new(Type::Dynamic)));
                }
                let first_key_type = self.check_expr(&entries[0].0)?;
                let first_val_type = self.check_expr(&entries[0].1)?;
                
                let mut final_key = first_key_type;
                let mut final_val = first_val_type;

                for (k, v) in entries.iter().skip(1) {
                    if self.check_expr(k)? != final_key { final_key = Type::Dynamic; }
                    if self.check_expr(v)? != final_val { final_val = Type::Dynamic; }
                }
                Ok(Type::Dict(Box::new(final_key), Box::new(final_val)))
            }
        }
    }

    fn is_assignable(&self, target: &Type, value: &Type) -> bool {
        if target == value || *target == Type::Dynamic || *value == Type::Dynamic || *target == Type::Auto {
            return true;
        }
        if let Type::Nullable(inner) = target {
            if *value == Type::Dynamic { return true; } // null is Dynamic
            return self.is_assignable(inner, value);
        }
        false
    }
}
