use crate::parser::ast::{Expr, Stmt, Literal};
use crate::lexer::TokenType;
use crate::error::{LavinaError, ErrorPhase};
use crate::eval::environment::Environment;
use crate::eval::native::get_native_functions;
use crate::eval::value::Value;
use crate::vm::memory::Memory;
use std::rc::Rc;
use std::cell::RefCell;

pub enum ControlFlow {
    None,
    Return(Value),
}

pub struct TreeWalker {
    env: Rc<RefCell<Environment>>,
    source: String,
    memory: Memory,
}

impl TreeWalker {
    pub fn new(source: String) -> Self {
        let env = Rc::new(RefCell::new(Environment::new()));
        let memory = Memory::new();
        
        for (name, func) in get_native_functions() {
            env.borrow_mut().define(name.clone(), Value::NativeFunction(name, func));
        }

        Self {
            env,
            source,
            memory,
        }
    }

    pub fn interpret(&mut self, statements: &[Stmt]) -> Result<(), LavinaError> {
        for stmt in statements {
            match self.execute(stmt)? {
                ControlFlow::Return(_) => break,
                ControlFlow::None => {}
            }
        }
        Ok(())
    }

    fn execute(&mut self, stmt: &Stmt) -> Result<ControlFlow, LavinaError> {
        match stmt {
            Stmt::Expression(expr) => {
                self.evaluate(expr)?;
                Ok(ControlFlow::None)
            }
            Stmt::Let(name, _, initializer, _) => {
                let value = if let Some(init) = initializer {
                    self.evaluate(init)?
                } else {
                    Value::Null
                };
                self.env.borrow_mut().define(name.lexeme.clone(), value);
                Ok(ControlFlow::None)
            }
            Stmt::If(condition, then_branch, else_branch) => {
                let cond_val = self.evaluate(condition)?;
                if cond_val.is_truthy() {
                    self.execute(then_branch)
                } else if let Some(else_stmt) = else_branch {
                    self.execute(else_stmt)
                } else {
                    Ok(ControlFlow::None)
                }
            }
            Stmt::While(condition, body) => {
                while self.evaluate(condition)?.is_truthy() {
                    match self.execute(body)? {
                        ControlFlow::Return(val) => return Ok(ControlFlow::Return(val)),
                        ControlFlow::None => {}
                    }
                }
                Ok(ControlFlow::None)
            }
            Stmt::For(_, _, _) => {
                todo!("For loop not implemented in tree-walker.");
            }
            Stmt::Block(stmts) => {
                let previous = self.env.clone();
                let new_env = Environment::with_enclosing(previous.borrow().clone());
                self.env = Rc::new(RefCell::new(new_env));
                
                let mut result = ControlFlow::None;
                for s in stmts {
                    match self.execute(s)? {
                        ControlFlow::Return(val) => {
                            result = ControlFlow::Return(val);
                            break;
                        }
                        ControlFlow::None => {}
                    }
                }

                self.env = previous;
                Ok(result)
            }
            Stmt::Function(_) => {
                todo!("Functions in tree-walker are deprecated.");
            }
            Stmt::Return(_, value) => {
                let result = if let Some(expr) = value {
                    self.evaluate(expr)?
                } else {
                    Value::Null
                };
                Ok(ControlFlow::Return(result))
            }
            Stmt::Namespace(_, _, _) => todo!("Namespace not implemented in tree-walker."),
            Stmt::Import(_, _) => todo!("Import not implemented in tree-walker."),
            Stmt::Directive(_) => Ok(ControlFlow::None),
            Stmt::Class(_, _, _) | Stmt::Struct(_, _, _) | Stmt::Enum(_, _, _) | Stmt::Try(_, _, _, _) => {
                todo!("OOP and Try/Catch in tree-walker not implemented")
            }
        }
    }

    fn evaluate(&mut self, expr: &Expr) -> Result<Value, LavinaError> {
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(i) => Ok(Value::Int(*i)),
                Literal::Float(f) => Ok(Value::Float(*f)),
                Literal::String(s) => Ok(Value::String(s.clone())),
                Literal::Bool(b) => Ok(Value::Bool(*b)),
                Literal::Null => Ok(Value::Null),
            },
            Expr::Grouping(e) => self.evaluate(e),
            Expr::Unary(op, right) => {
                let r_val = self.evaluate(right)?;
                match op.token_type {
                    TokenType::Minus => match r_val {
                        Value::Int(i) => Ok(Value::Int(-i)),
                        Value::Float(f) => Ok(Value::Float(-f)),
                        _ => Err(self.error("Operand must be a number.".to_string(), op.line, op.column)),
                    },
                    TokenType::Bang => Ok(Value::Bool(!r_val.is_truthy())),
                    _ => Err(self.error("Invalid unary operator.".to_string(), op.line, op.column)),
                }
            }
            Expr::Binary(left, op, right) => {
                let l_val = self.evaluate(left)?;
                let r_val = self.evaluate(right)?;
                match op.token_type {
                    TokenType::Plus => match (l_val, r_val) {
                        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
                        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
                        (Value::String(a), Value::String(b)) => Ok(Value::String(a + &b)),
                        _ => Err(self.error("Operands must be numbers or strings.".to_string(), op.line, op.column)),
                    },
                    TokenType::Minus => match (l_val, r_val) {
                        (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
                        (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
                        _ => Err(self.error("Operands must be numbers.".to_string(), op.line, op.column)),
                    },
                    _ => todo!("Binary operator {:?} not implemented in tree-walker.", op.token_type),
                }
            }
            Expr::Variable(name) => {
                match self.env.borrow().get(&name.lexeme) {
                    Some(val) => Ok(val),
                    None => Err(self.error(format!("Undefined variable '{}'.", name.lexeme), name.line, name.column)),
                }
            }
            Expr::Assign(name, value) => {
                let val = self.evaluate(value)?;
                if self.env.borrow_mut().assign(&name.lexeme, val.clone()) {
                    Ok(val)
                } else {
                    Err(self.error(format!("Undefined variable '{}'.", name.lexeme), name.line, name.column))
                }
            }
            Expr::Call(callee, paren, args) => {
                let func = self.evaluate(callee)?;
                let mut evaluated_args = Vec::new();
                for arg in args {
                    evaluated_args.push(self.evaluate(arg)?);
                }

                match func {
                    Value::NativeFunction(_, native_fn) => {
                        native_fn(&mut self.memory, evaluated_args).map_err(|e| self.error(e, paren.line, paren.column))
                    }
                    _ => Err(self.error("Can only call functions.".to_string(), paren.line, paren.column)),
                }
            }
            Expr::Get(_, _) | Expr::Set(_, _, _) | Expr::StaticGet(_, _) | Expr::This(_) => {
                todo!("OOP in tree-walker not implemented")
            }
            _ => todo!("Expression not implemented in tree-walker."),
        }
    }

    fn error(&self, message: String, line: usize, column: usize) -> LavinaError {
        LavinaError::new(ErrorPhase::Runtime, message, line, column).with_context(&self.source)
    }
}
