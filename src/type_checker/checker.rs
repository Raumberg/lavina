use crate::parser::ast::{Expr, Stmt, Type, Literal, Visibility};
use crate::error::{LavinaError, ErrorPhase};
use crate::lexer::TokenType;
use std::collections::HashMap;

#[derive(Clone)]
pub enum TypeInfo {
    Variable(Type),
    Function(Type, Vec<Type>),
    Namespace(String, HashMap<String, (TypeInfo, Visibility)>), // Added visibility to members
}

pub struct TypeEnvironment {
    pub values: HashMap<String, (TypeInfo, Visibility)>, // Added visibility
    pub enclosing: Option<Box<TypeEnvironment>>,
}

impl TypeEnvironment {
    pub fn new() -> Self {
        Self {
            values: HashMap::new(),
            enclosing: None,
        }
    }

    pub fn define(&mut self, name: String, info: TypeInfo, visibility: Visibility) {
        self.values.insert(name, (info, visibility));
    }

    pub fn get(&self, name: &str) -> Option<TypeInfo> {
        if let Some((info, _)) = self.values.get(name) {
            return Some(info.clone());
        }
        if let Some(enclosing) = &self.enclosing {
            return enclosing.get(name);
        }
        None
    }
}

pub struct TypeChecker {
    env: TypeEnvironment,
    source: String,
}

impl TypeChecker {
    pub fn new() -> Self {
        let mut checker = Self {
            env: TypeEnvironment::new(),
            source: String::new(),
        };
        
        // Register native functions as public
        checker.env.define("print".to_string(), TypeInfo::Function(Type::Void, vec![Type::Dynamic]), Visibility::Public);
        checker.env.define("len".to_string(), TypeInfo::Function(Type::Int, vec![Type::Dynamic]), Visibility::Public);
        checker.env.define("clock".to_string(), TypeInfo::Function(Type::Float, vec![]), Visibility::Public);
        checker.env.define("type".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic]), Visibility::Public);
        
        checker
    }

    pub fn set_source(&mut self, source: String) {
        self.source = source;
    }

    pub fn check_statements(&mut self, statements: &[Stmt]) -> Result<(), LavinaError> {
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
            Stmt::Let(name, type_ann, initializer, visibility) => {
                let mut inferred_type = Type::Auto;
                if let Some(init) = initializer {
                    inferred_type = self.check_expr(init)?;
                }

                let final_type = if let Some(ann) = type_ann {
                    if ann == &Type::Auto {
                        inferred_type
                    } else {
                        if !self.is_assignable(ann, &inferred_type) {
                            return Err(LavinaError::new(
                                ErrorPhase::TypeChecker,
                                format!("Type mismatch: expected {:?}, but got {:?}", ann, inferred_type),
                                name.line,
                                name.column,
                            ).with_context(&self.source));
                        }
                        ann.clone()
                    }
                } else {
                    inferred_type
                };

                self.env.define(name.lexeme.clone(), TypeInfo::Variable(final_type), visibility.clone());
                Ok(())
            }
            Stmt::Function(decl) => {
                let param_types: Vec<Type> = decl.params.iter().map(|(_, t)| t.clone()).collect();
                self.env.define(decl.name.lexeme.clone(), TypeInfo::Function(decl.return_type.clone(), param_types), decl.visibility.clone());
                
                // Check body in a new scope
                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));

                for (param_name, param_type) in &decl.params {
                    self.env.define(param_name.lexeme.clone(), TypeInfo::Variable(param_type.clone()), Visibility::Public);
                }

                self.check_statements(&decl.body)?;

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::Namespace(name, body) => {
                // Collect members of the namespace
                let mut members = HashMap::new();
                
                // First pass: register names
                for s in body {
                    match s {
                        Stmt::Function(f) => {
                            let pts: Vec<Type> = f.params.iter().map(|(_, t)| t.clone()).collect();
                            members.insert(f.name.lexeme.clone(), (TypeInfo::Function(f.return_type.clone(), pts), f.visibility.clone()));
                        }
                        Stmt::Let(n, t, _, v) => {
                            let final_t = t.clone().unwrap_or(Type::Auto);
                            members.insert(n.lexeme.clone(), (TypeInfo::Variable(final_t), v.clone()));
                        }
                        _ => {}
                    }
                }

                self.env.define(name.lexeme.clone(), TypeInfo::Namespace(name.lexeme.clone(), members), Visibility::Public);
                
                // Second pass: check body in namespace scope
                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));

                for s in body {
                    self.check_stmt(s)?;
                }

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::If(condition, then_branch, else_branch) => {
                self.check_expr(condition)?;
                self.check_stmt(then_branch)?;
                if let Some(else_stmt) = else_branch {
                    self.check_stmt(else_stmt)?;
                }
                Ok(())
            }
            Stmt::While(condition, body) => {
                self.check_expr(condition)?;
                self.check_stmt(body)?;
                Ok(())
            }
            Stmt::For(item_name, collection, body) => {
                let coll_type = self.check_expr(collection)?;
                let item_type = match coll_type {
                    Type::Array(inner) => *inner,
                    Type::Dynamic => Type::Dynamic,
                    _ => return Err(LavinaError::new(
                        ErrorPhase::TypeChecker,
                        "Can only iterate over vectors.".to_string(),
                        item_name.line,
                        item_name.column,
                    ).with_context(&self.source)),
                };

                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));

                self.env.define(item_name.lexeme.clone(), TypeInfo::Variable(item_type), Visibility::Public);
                self.check_stmt(body)?;

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::Block(stmts) => {
                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));

                self.check_statements(stmts)?;

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::Return(_, value) => {
                if let Some(expr) = value {
                    self.check_expr(expr)?;
                }
                Ok(())
            }
            Stmt::Import(_, _) => Ok(()),
            Stmt::Directive(_) => Ok(()),
        }
    }

    pub fn check_expr(&mut self, expr: &Expr) -> Result<Type, LavinaError> {
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(_) => Ok(Type::Int),
                Literal::Float(_) => Ok(Type::Float),
                Literal::String(_) => Ok(Type::String),
                Literal::Bool(_) => Ok(Type::Bool),
                Literal::Null => Ok(Type::Dynamic),
            },
            Expr::Variable(name) => {
                match self.env.get(&name.lexeme) {
                    Some(TypeInfo::Variable(t)) => Ok(t),
                    Some(TypeInfo::Function(ret, _)) => Ok(Type::Function(Box::new(ret), vec![])),
                    Some(TypeInfo::Namespace(_, _)) => Ok(Type::Dynamic), // Or a specific Namespace type
                    None => Err(LavinaError::new(
                        ErrorPhase::TypeChecker,
                        format!("Undefined variable '{}'", name.lexeme),
                        name.line,
                        name.column,
                    ).with_context(&self.source)),
                }
            }
            Expr::Index(collection, bracket, index) => {
                let coll_type = self.check_expr(collection)?;
                
                // If it's a static namespace access like math.add
                if let Expr::Variable(name) = &**collection {
                    if let Some(TypeInfo::Namespace(ns_name, members)) = self.env.get(&name.lexeme) {
                        if let Expr::Literal(Literal::String(member_name)) = &**index {
                            if let Some((info, visibility)) = members.get(member_name) {
                                if visibility == &Visibility::Private {
                                    return Err(LavinaError::new(
                                        ErrorPhase::TypeChecker,
                                        format!("Cannot access private member '{}' of namespace '{}'", member_name, ns_name),
                                        bracket.line,
                                        bracket.column,
                                    ).with_context(&self.source));
                                }
                                return Ok(match info {
                                    TypeInfo::Variable(t) => t.clone(),
                                    TypeInfo::Function(ret, _) => ret.clone(),
                                    TypeInfo::Namespace(_, _) => Type::Dynamic,
                                });
                            }
                        }
                    }
                }

                // Normal collection indexing
                match coll_type {
                    Type::Array(inner) => {
                        let idx_type = self.check_expr(index)?;
                        if idx_type != Type::Int && idx_type != Type::Auto {
                            return Err(LavinaError::new(ErrorPhase::TypeChecker, "Array index must be an integer.".to_string(), bracket.line, bracket.column).with_context(&self.source));
                        }
                        Ok(*inner)
                    }
                    Type::Dict(_, val) => {
                        self.check_expr(index)?;
                        Ok(*val)
                    }
                    Type::Dynamic => Ok(Type::Dynamic),
                    _ => Err(LavinaError::new(ErrorPhase::TypeChecker, "Can only index vectors and hashmaps.".to_string(), bracket.line, bracket.column).with_context(&self.source)),
                }
            }
            Expr::Call(callee, _paren, args) => {
                let _callee_type = self.check_expr(callee)?;
                for arg in args {
                    self.check_expr(arg)?;
                }
                // Simplified call check
                Ok(Type::Dynamic)
            }
            Expr::Binary(left, op, right) => {
                let l = self.check_expr(left)?;
                let r = self.check_expr(right)?;
                
                match op.token_type {
                    TokenType::Plus | TokenType::Minus | TokenType::Star | TokenType::Slash => {
                        if l == Type::Int && r == Type::Int { Ok(Type::Int) }
                        else if (l == Type::Float || l == Type::Int) && (r == Type::Float || r == Type::Int) { Ok(Type::Float) }
                        else if l == Type::String || r == Type::String { Ok(Type::String) }
                        else { Ok(Type::Dynamic) }
                    }
                    TokenType::EqualEqual | TokenType::BangEqual | TokenType::Greater | TokenType::GreaterEqual | TokenType::Less | TokenType::LessEqual => Ok(Type::Bool),
                    _ => Ok(Type::Dynamic),
                }
            }
            Expr::Assign(name, value) => {
                let val_type = self.check_expr(value)?;
                if let Some(TypeInfo::Variable(t)) = self.env.get(&name.lexeme) {
                    if !self.is_assignable(&t, &val_type) {
                        return Err(LavinaError::new(ErrorPhase::TypeChecker, "Type mismatch in assignment.".to_string(), name.line, name.column).with_context(&self.source));
                    }
                }
                Ok(val_type)
            }
            Expr::Vector(elements) => {
                let mut element_type = Type::Auto;
                for el in elements {
                    let t = self.check_expr(el)?;
                    if element_type == Type::Auto {
                        element_type = t;
                    } else if element_type != t && t != Type::Dynamic {
                        element_type = Type::Dynamic;
                    }
                }
                if element_type == Type::Auto { element_type = Type::Dynamic; }
                Ok(Type::Array(Box::new(element_type)))
            }
            Expr::Map(entries) => {
                let mut key_type = Type::Auto;
                let mut val_type = Type::Auto;
                for (k, v) in entries {
                    let kt = self.check_expr(k)?;
                    let vt = self.check_expr(v)?;
                    
                    if key_type == Type::Auto { key_type = kt; }
                    else if key_type != kt { key_type = Type::Dynamic; }
                    
                    if val_type == Type::Auto { val_type = vt; }
                    else if val_type != vt { val_type = Type::Dynamic; }
                }
                if key_type == Type::Auto { key_type = Type::Dynamic; }
                if val_type == Type::Auto { val_type = Type::Dynamic; }
                Ok(Type::Dict(Box::new(key_type), Box::new(val_type)))
            }
            Expr::Grouping(expr) => self.check_expr(expr),
            _ => Ok(Type::Dynamic),
        }
    }

    fn is_assignable(&self, target: &Type, value: &Type) -> bool {
        if target == &Type::Auto || target == &Type::Dynamic || value == &Type::Dynamic { return true; }
        if target == value { return true; }
        
        match (target, value) {
            (Type::Array(t1), Type::Array(t2)) => self.is_assignable(t1, t2),
            (Type::Dict(k1, v1), Type::Dict(k2, v2)) => self.is_assignable(k1, k2) && self.is_assignable(v1, v2),
            (Type::Nullable(t1), t2) => self.is_assignable(t1, t2),
            _ => false,
        }
    }
}
