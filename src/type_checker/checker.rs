use crate::parser::ast::{Expr, Stmt, Literal, Type, FunctionDecl};
use crate::lexer::TokenType; // Removed Token
use crate::error::{LavinaError, ErrorPhase};
use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq)]
enum TypeInfo {
    Variable(Type),
    Function {
        params: Vec<Type>,
        return_type: Type,
        is_variadic: bool,
    },
}

struct TypeEnvironment {
    types: HashMap<String, TypeInfo>,
    enclosing: Option<Box<TypeEnvironment>>,
}

impl TypeEnvironment {
    fn new() -> Self {
        Self {
            types: HashMap::new(),
            enclosing: None,
        }
    }

    fn define(&mut self, name: String, info: TypeInfo) {
        self.types.insert(name, info);
    }

    fn get(&self, name: &str) -> Option<TypeInfo> {
        if let Some(info) = self.types.get(name) {
            return Some(info.clone());
        }
        if let Some(ref enclosing) = self.enclosing {
            return enclosing.get(name);
        }
        None
    }
}

pub struct TypeChecker {
    env: TypeEnvironment,
    pub source: String, // Made public
}

impl TypeChecker {
    pub fn new() -> Self {
        let mut env = TypeEnvironment::new();
        
        // Native functions
        env.define("print".to_string(), TypeInfo::Function {
            params: vec![],
            return_type: Type::Void,
            is_variadic: true,
        });
        env.define("len".to_string(), TypeInfo::Function {
            params: vec![Type::Dynamic],
            return_type: Type::Int,
            is_variadic: false,
        });
        env.define("clock".to_string(), TypeInfo::Function {
            params: vec![],
            return_type: Type::Float,
            is_variadic: false,
        });
        env.define("type".to_string(), TypeInfo::Function {
            params: vec![Type::Dynamic],
            return_type: Type::String,
            is_variadic: false,
        });

        Self {
            env,
            source: String::new(),
        }
    }

    pub fn set_source(&mut self, source: String) {
        self.source = source;
    }

    pub fn check_statements(&mut self, statements: &[Stmt]) -> Result<(), LavinaError> {
        // First pass: register functions
        for stmt in statements {
            if let Stmt::Function(decl) = stmt {
                self.register_function(decl)?;
            }
        }

        // Second pass: check bodies
        for stmt in statements {
            self.check_stmt(stmt)?;
        }
        Ok(())
    }

    fn register_function(&mut self, decl: &FunctionDecl) -> Result<(), LavinaError> {
        let params = decl.params.iter().map(|(_, t)| t.clone()).collect();
        self.env.define(decl.name.lexeme.clone(), TypeInfo::Function {
            params,
            return_type: decl.return_type.clone(),
            is_variadic: false,
        });
        Ok(())
    }

    fn check_stmt(&mut self, stmt: &Stmt) -> Result<(), LavinaError> {
        match stmt {
            Stmt::Expression(expr) => {
                self.check_expr(expr)?;
                Ok(())
            }
            Stmt::Let(name, declared_type, initializer) => {
                let init_type = if let Some(init) = initializer {
                    self.check_expr(init)?
                } else {
                    Type::Nullable(Box::new(Type::Void)) // Default to null
                };

                let final_type = match declared_type {
                    Some(Type::Auto) => init_type, // Inference!
                    Some(t) => {
                        if !self.is_assignable(t, &init_type) {
                            return Err(self.error(format!("Type mismatch: expected {:?}, but got {:?}", t, init_type), name.line, name.column));
                        }
                        t.clone()
                    }
                    None => init_type,
                };

                self.env.define(name.lexeme.clone(), TypeInfo::Variable(final_type));
                Ok(())
            }
            Stmt::Function(decl) => {
                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                
                self.env.enclosing = Some(Box::new(previous_env));
                
                for (param_name, param_type) in &decl.params {
                    self.env.define(param_name.lexeme.clone(), TypeInfo::Variable(param_type.clone()));
                }

                for s in &decl.body {
                    self.check_stmt(s)?;
                }

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::Block(stmts) => {
                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));

                for s in stmts {
                    self.check_stmt(s)?;
                }

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::If(condition, then_branch, else_branch) => {
                let _cond_type = self.check_expr(condition)?;
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
                    Type::Dict(_, value_type) => *value_type,
                    Type::Dynamic => Type::Dynamic,
                    _ => return Err(self.error("Can only iterate over vectors and hashmaps.".to_string(), item_name.line, item_name.column)),
                };

                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));

                self.env.define(item_name.lexeme.clone(), TypeInfo::Variable(item_type));
                self.check_stmt(body)?;

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::Return(_keyword, value) => {
                if let Some(expr) = value {
                    self.check_expr(expr)?;
                }
                Ok(())
            }
            Stmt::Directive(_) => Ok(()),
        }
    }

    pub fn check_expr(&mut self, expr: &Expr) -> Result<Type, LavinaError> { // Made public
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(_) => Ok(Type::Int),
                Literal::Float(_) => Ok(Type::Float),
                Literal::String(_) => Ok(Type::String),
                Literal::Bool(_) => Ok(Type::Bool),
                Literal::Null => Ok(Type::Void),
            },
            Expr::Variable(name) => {
                match self.env.get(&name.lexeme) {
                    Some(TypeInfo::Variable(t)) => Ok(t),
                    Some(TypeInfo::Function { .. }) => Ok(Type::Dynamic), // Function as value
                    None => Err(self.error(format!("Undefined variable '{}'", name.lexeme), name.line, name.column)),
                }
            }
            Expr::Assign(name, value) => {
                let val_type = self.check_expr(value)?;
                match self.env.get(&name.lexeme) {
                    Some(TypeInfo::Variable(t)) => {
                        if !self.is_assignable(&t, &val_type) {
                            return Err(self.error(format!("Cannot assign {:?} to variable of type {:?}", val_type, t), name.line, name.column));
                        }
                        Ok(t)
                    }
                    _ => Err(self.error(format!("Undefined variable '{}'", name.lexeme), name.line, name.column)),
                }
            }
            Expr::Binary(left, op, right) => {
                let l_type = self.check_expr(left)?;
                let r_type = self.check_expr(right)?;
                match op.token_type {
                    TokenType::Plus | TokenType::Minus | TokenType::Star | TokenType::Slash => {
                        if (l_type == Type::Int || l_type == Type::Float) && (r_type == Type::Int || r_type == Type::Float) {
                            if l_type == Type::Float || r_type == Type::Float { return Ok(Type::Float); }
                            return Ok(Type::Int);
                        }
                        if op.token_type == TokenType::Plus && l_type == Type::String && r_type == Type::String {
                            return Ok(Type::String);
                        }
                        Err(self.error("Invalid operands for binary operator".to_string(), op.line, op.column))
                    }
                    TokenType::EqualEqual | TokenType::BangEqual | TokenType::Greater | TokenType::GreaterEqual | TokenType::Less | TokenType::LessEqual => {
                        Ok(Type::Bool)
                    }
                    _ => Ok(Type::Dynamic),
                }
            }
            Expr::Call(callee, paren, args) => {
                let _callee_type = self.check_expr(callee)?;
                let mut arg_types = Vec::new();
                for arg in args {
                    arg_types.push(self.check_expr(arg)?);
                }

                if let Expr::Variable(name) = &**callee {
                    if let Some(TypeInfo::Function { params, return_type, is_variadic }) = self.env.get(&name.lexeme) {
                        if !is_variadic && params.len() != args.len() {
                            return Err(self.error(format!("Expected {} arguments but got {}", params.len(), args.len()), paren.line, paren.column));
                        }
                        if !is_variadic {
                            for (i, (expected, actual)) in params.iter().zip(arg_types.iter()).enumerate() {
                                if !self.is_assignable(expected, actual) {
                                    return Err(self.error(format!("Argument {} type mismatch: expected {:?}, got {:?}", i + 1, expected, actual), paren.line, paren.column));
                                }
                            }
                        }
                        return Ok(return_type);
                    }
                }
                Ok(Type::Dynamic)
            }
            Expr::Index(collection, bracket, index) => {
                let coll_type = self.check_expr(collection)?;
                let idx_type = self.check_expr(index)?;

                match coll_type {
                    Type::Array(inner) => {
                        if idx_type != Type::Int && idx_type != Type::Dynamic {
                            return Err(self.error("Vector index must be an integer.".to_string(), bracket.line, bracket.column));
                        }
                        Ok(*inner)
                    }
                    Type::Dict(key_type, val_type) => {
                        if !self.is_assignable(&key_type, &idx_type) {
                            return Err(self.error(format!("Map key type mismatch: expected {:?}, got {:?}", key_type, idx_type), bracket.line, bracket.column));
                        }
                        Ok(*val_type)
                    }
                    Type::Dynamic => Ok(Type::Dynamic),
                    _ => Err(self.error("Can only index vectors and hashmaps.".to_string(), bracket.line, bracket.column)),
                }
            }
            Expr::Vector(elements) => {
                let mut inner_type = Type::Auto;
                for el in elements {
                    let el_type = self.check_expr(el)?;
                    if inner_type == Type::Auto {
                        inner_type = el_type;
                    } else if inner_type != el_type {
                        inner_type = Type::Dynamic;
                    }
                }
                Ok(Type::Array(Box::new(inner_type)))
            }
            Expr::Map(entries) => {
                let mut key_type = Type::Auto;
                let mut val_type = Type::Auto;
                for (k, v) in entries {
                    let k_t = self.check_expr(k)?;
                    let v_t = self.check_expr(v)?;
                    if key_type == Type::Auto {
                        key_type = k_t;
                    } else if key_type != k_t {
                        key_type = Type::Dynamic;
                    }
                    if val_type == Type::Auto {
                        val_type = v_t;
                    } else if val_type != v_t {
                        val_type = Type::Dynamic;
                    }
                }
                Ok(Type::Dict(Box::new(key_type), Box::new(val_type)))
            }
            Expr::Grouping(e) => self.check_expr(e),
            _ => Ok(Type::Dynamic),
        }
    }

    fn is_assignable(&self, target_type: &Type, source_type: &Type) -> bool {
        if target_type == source_type {
            return true;
        }
        match (target_type, source_type) {
            (Type::Auto, _) => true,
            (Type::Dynamic, _) => true,
            (_, Type::Dynamic) => true,
            (Type::Nullable(t_inner), s_inner) => {
                if s_inner == &Type::Void {
                    return true;
                }
                self.is_assignable(t_inner, s_inner)
            }
            (Type::Array(t_inner), Type::Array(s_inner)) => {
                // For containers, we need exact match or target must be dynamic
                if **t_inner == Type::Dynamic { return true; }
                t_inner == s_inner
            }
            (Type::Dict(tk_inner, tv_inner), Type::Dict(sk_inner, sv_inner)) => {
                // For maps, keys and values must match or target must be dynamic
                let key_match = **tk_inner == Type::Dynamic || tk_inner == sk_inner;
                let val_match = **tv_inner == Type::Dynamic || tv_inner == sv_inner;
                key_match && val_match
            }
            _ => false,
        }
    }

    fn error(&self, message: String, line: usize, column: usize) -> LavinaError {
        LavinaError::new(ErrorPhase::TypeChecker, message, line, column).with_context(&self.source)
    }
}
