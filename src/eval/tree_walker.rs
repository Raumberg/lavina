use crate::parser::ast::{Expr, Stmt, Literal};
use crate::lexer::TokenType;
use crate::eval::value::Value;
use crate::eval::environment::Environment;
use crate::eval::native::get_native_functions;
use crate::error::{LavinaError, ErrorPhase};
use std::rc::Rc;

pub enum ControlFlow {
    None,
    Return(Value),
}

pub struct TreeWalker {
    pub environment: Environment,
    pub source: String,
}

impl TreeWalker {
    pub fn new(source: String) -> Self {
        let env = Environment::new();
        
        // Register native functions
        for (name, func) in get_native_functions() {
            env.define(name.clone(), Value::NativeFunction(name, func));
        }

        Self {
            environment: env,
            source,
        }
    }

    fn error(&self, message: String, line: usize, column: usize) -> LavinaError {
        LavinaError::new(ErrorPhase::Runtime, message, line, column).with_context(&self.source)
    }

    pub fn interpret(&mut self, statements: &[Stmt]) -> Result<(), LavinaError> {
        for statement in statements {
            match self.execute(statement, self.environment.clone()) {
                Ok(ControlFlow::Return(_)) => break,
                Ok(ControlFlow::None) => (),
                Err(e) => return Err(e),
            }
        }
        Ok(())
    }

    pub fn interpret_expression(&mut self, expr: &Expr) -> Result<Value, LavinaError> {
        self.evaluate(expr, self.environment.clone())
    }

    fn execute(&mut self, stmt: &Stmt, env: Environment) -> Result<ControlFlow, LavinaError> {
        match stmt {
            Stmt::Expression(expr) => {
                self.evaluate(expr, env)?;
                Ok(ControlFlow::None)
            }
            Stmt::Let(name, _, initializer) => {
                let mut value = Value::Null;
                if let Some(init) = initializer {
                    value = self.evaluate(init, env.clone())?;
                }
                env.define(name.lexeme.clone(), value);
                Ok(ControlFlow::None)
            }
            Stmt::If(condition, then_branch, else_branch) => {
                let cond_value = self.evaluate(condition, env.clone())?;
                if cond_value.is_truthy() {
                    return self.execute(then_branch, env);
                } else if let Some(else_stmt) = else_branch {
                    return self.execute(else_stmt, env);
                }
                Ok(ControlFlow::None)
            }
            Stmt::While(condition, body) => {
                loop {
                    let cond_value = self.evaluate(condition, env.clone())?;
                    if !cond_value.is_truthy() {
                        break;
                    }
                    if let ControlFlow::Return(val) = self.execute(body, env.clone())? {
                        return Ok(ControlFlow::Return(val));
                    }
                }
                Ok(ControlFlow::None)
            }
            Stmt::Block(statements) => {
                let block_env = Environment::with_enclosing(env);
                for statement in statements {
                    if let ControlFlow::Return(val) = self.execute(statement, block_env.clone())? {
                        return Ok(ControlFlow::Return(val));
                    }
                }
                Ok(ControlFlow::None)
            }
            Stmt::Function(decl) => {
                let function = Value::Function(Rc::new(crate::eval::function::LavinaFunction {
                    declaration: decl.clone(),
                    closure: env.clone(),
                }));
                env.define(decl.name.lexeme.clone(), function);
                Ok(ControlFlow::None)
            }
            Stmt::Return(_keyword, value) => {
                let mut return_val = Value::Null;
                if let Some(expr) = value {
                    return_val = self.evaluate(expr, env)?;
                }
                Ok(ControlFlow::Return(return_val))
            }
            Stmt::Directive(_) => Ok(ControlFlow::None),
        }
    }

    fn evaluate(&mut self, expr: &Expr, env: Environment) -> Result<Value, LavinaError> {
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(i) => Ok(Value::Int(*i)),
                Literal::Float(f) => Ok(Value::Float(*f)),
                Literal::String(s) => Ok(Value::String(s.clone())),
                Literal::Bool(b) => Ok(Value::Bool(*b)),
                Literal::Null => Ok(Value::Null),
            },
            Expr::Grouping(e) => self.evaluate(e, env),
            Expr::Unary(op, right) => {
                let right_val = self.evaluate(right, env)?;
                match op.token_type {
                    TokenType::Minus => match right_val {
                        Value::Int(i) => Ok(Value::Int(-i)),
                        Value::Float(f) => Ok(Value::Float(-f)),
                        _ => Err(self.error("Operand must be a number.".to_string(), op.line, op.column)),
                    },
                    TokenType::Bang => Ok(Value::Bool(!right_val.is_truthy())),
                    _ => Ok(Value::Null),
                }
            }
            Expr::Binary(left, op, right) => {
                let left_val = self.evaluate(left, env.clone())?;
                let right_val = self.evaluate(right, env)?;
                
                match op.token_type {
                    TokenType::Minus => match (left_val, right_val) {
                        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
                        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
                        (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 - b)),
                        (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a - b as f64)),
                        _ => Err(self.error("Operands must be numbers.".to_string(), op.line, op.column)),
                    },
                    TokenType::Slash => match (left_val, right_val) {
                        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a / b)),
                        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a / b)),
                        _ => Err(self.error("Operands must be numbers.".to_string(), op.line, op.column)),
                    },
                    TokenType::Star => match (left_val, right_val) {
                        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
                        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
                        _ => Err(self.error("Operands must be numbers.".to_string(), op.line, op.column)),
                    },
                    TokenType::Plus => match (left_val, right_val) {
                        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
                        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
                        (Value::String(a), Value::String(b)) => Ok(Value::String(format!("{}{}", a, b))),
                        _ => Err(self.error("Operands must be numbers or strings.".to_string(), op.line, op.column)),
                    },
                    TokenType::Greater => Ok(Value::Bool(left_val > right_val)),
                    TokenType::GreaterEqual => Ok(Value::Bool(left_val >= right_val)),
                    TokenType::Less => Ok(Value::Bool(left_val < right_val)),
                    TokenType::LessEqual => Ok(Value::Bool(left_val <= right_val)),
                    TokenType::BangEqual => Ok(Value::Bool(left_val != right_val)),
                    TokenType::EqualEqual => Ok(Value::Bool(left_val == right_val)),
                    _ => Ok(Value::Null),
                }
            }
            Expr::Logical(left, op, right) => {
                let left_val = self.evaluate(left, env.clone())?;
                if op.token_type == TokenType::Or {
                    if left_val.is_truthy() { return Ok(left_val); }
                } else {
                    if !left_val.is_truthy() { return Ok(left_val); }
                }
                self.evaluate(right, env)
            }
            Expr::Call(callee, paren, arguments) => {
                let callee_val = self.evaluate(callee, env.clone())?;
                let mut args = Vec::new();
                for arg in arguments {
                    args.push(self.evaluate(arg, env.clone())?);
                }

                match callee_val {
                    Value::NativeFunction(_, func) => {
                        func(args).map_err(|e| self.error(e, paren.line, paren.column))
                    }
                    Value::Function(func) => {
                        if args.len() != func.declaration.params.len() {
                            return Err(self.error(format!("Expected {} arguments but got {}.", func.declaration.params.len(), args.len()), paren.line, paren.column));
                        }
                        
                        let call_env = Environment::with_enclosing(func.closure.clone());
                        for (i, (param_name, _)) in func.declaration.params.iter().enumerate() {
                            call_env.define(param_name.lexeme.clone(), args[i].clone());
                        }

                        match self.execute(&Stmt::Block(func.declaration.body.clone()), call_env)? {
                            ControlFlow::Return(val) => Ok(val),
                            ControlFlow::None => Ok(Value::Null),
                        }
                    }
                    _ => Err(self.error("Can only call functions and classes.".to_string(), paren.line, paren.column)),
                }
            }
            Expr::Variable(name) => {
                env.get(&name.lexeme).ok_or_else(|| self.error(format!("Undefined variable '{}'.", name.lexeme), name.line, name.column))
            }
            Expr::Assign(name, value) => {
                let val = self.evaluate(value, env.clone())?;
                if !env.assign(&name.lexeme, val.clone()) {
                    return Err(self.error(format!("Undefined variable '{}'.", name.lexeme), name.line, name.column));
                }
                Ok(val)
            }
            Expr::Vector(elements) => {
                let mut values = Vec::new();
                for el in elements {
                    values.push(self.evaluate(el, env.clone())?);
                }
                Ok(Value::Vector(Rc::new(std::cell::RefCell::new(values))))
            }
            Expr::Map(entries) => {
                let mut map = std::collections::HashMap::new();
                for (k, v) in entries {
                    let key_val = self.evaluate(k, env.clone())?;
                    let value_val = self.evaluate(v, env.clone())?;
                    map.insert(key_val.to_string(), value_val);
                }
                Ok(Value::HashMap(Rc::new(std::cell::RefCell::new(map))))
            }
        }
    }
}
