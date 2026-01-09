use crate::parser::ast::{Expr, Stmt, Literal, Visibility};
use crate::vm::chunk::Chunk;
use crate::vm::opcode::OpCode;
use crate::lexer::TokenType;
use crate::error::LavinaError;
use crate::eval::value::ObjFunction;
use std::rc::Rc;

struct Local {
    name: String,
    depth: i32,
}

#[derive(PartialEq)]
pub enum FunctionType {
    Script,
    Function,
}

pub struct Compiler {
    function: ObjFunction,
    function_type: FunctionType,
    locals: Vec<Local>,
    scope_depth: i32,
}

impl Compiler {
    pub fn new(name: String, f_type: FunctionType) -> Self {
        let mut compiler = Self {
            function: ObjFunction {
                arity: 0,
                chunk: Chunk::new(),
                name,
            },
            function_type: f_type,
            locals: Vec::new(),
            scope_depth: 0,
        };
        
        compiler.locals.push(Local {
            name: "".to_string(),
            depth: 0,
        });
        
        compiler
    }

    pub fn compile(mut self, statements: &[Stmt]) -> Result<Rc<ObjFunction>, LavinaError> {
        for stmt in statements {
            self.compile_stmt(stmt)?;
        }
        
        self.emit_byte(OpCode::Null as u8, 0);
        self.emit_byte(OpCode::Return as u8, 0);
        
        Ok(Rc::new(self.function))
    }

    fn compile_stmt(&mut self, stmt: &Stmt) -> Result<(), LavinaError> {
        match stmt {
            Stmt::Expression(expr) => {
                self.compile_expr(expr)?;
                self.emit_byte(OpCode::Pop as u8, 0);
                Ok(())
            }
            Stmt::Let(name, _, initializer, _visibility) => {
                if let Some(init) = initializer {
                    self.compile_expr(init)?;
                } else {
                    self.emit_byte(OpCode::Null as u8, name.line);
                }
                
                if self.scope_depth > 0 {
                    self.add_local(name.lexeme.clone());
                } else {
                    let constant = self.function.chunk.add_constant(crate::eval::value::Value::String(name.lexeme.clone()));
                    self.emit_byte(OpCode::DefineGlobal as u8, name.line);
                    self.emit_byte(constant as u8, name.line);
                }
                Ok(())
            }
            Stmt::Function(decl) => {
                let mut compiler = Compiler::new(decl.name.lexeme.clone(), FunctionType::Function);
                compiler.function.arity = decl.params.len();
                compiler.begin_scope();
                
                for (param_name, _) in &decl.params {
                    compiler.add_local(param_name.lexeme.clone());
                }
                
                let function = compiler.compile(&decl.body)?;
                let constant = self.function.chunk.add_constant(crate::eval::value::Value::ObjFunction(function));
                
                self.emit_byte(OpCode::Constant as u8, decl.name.line);
                self.emit_byte(constant as u8, decl.name.line);
                
                let name_const = self.function.chunk.add_constant(crate::eval::value::Value::String(decl.name.lexeme.clone()));
                self.emit_byte(OpCode::DefineGlobal as u8, decl.name.line);
                self.emit_byte(name_const as u8, decl.name.line);
                Ok(())
            }
            Stmt::Namespace(name, body) => {
                // In a true namespace system, we'd handle visibility here.
                // For now, let's keep the object-based approach but we could
                // also do name mangling (e.g., math::add).
                
                // Let's use a simpler approach for now:
                // Compile the body as if it were a function that returns a map of its public members.
                
                let mut compiler = Compiler::new(name.lexeme.clone(), FunctionType::Script);
                
                for s in body {
                    compiler.compile_stmt(s)?;
                }
                
                // Now collect all public members into a map
                let mut public_members = Vec::new();
                for s in body {
                    match s {
                        Stmt::Function(f) if f.visibility == Visibility::Public => {
                            public_members.push(f.name.lexeme.clone());
                        }
                        Stmt::Let(n, _, _, v) if v == &Visibility::Public => {
                            public_members.push(n.lexeme.clone());
                        }
                        _ => {}
                    }
                }

                // Emit code to build the namespace object
                for m_name in &public_members {
                    let name_const = compiler.function.chunk.add_constant(crate::eval::value::Value::String(m_name.clone()));
                    compiler.emit_byte(OpCode::Constant as u8, 0);
                    compiler.emit_byte(name_const as u8, 0);
                    
                    compiler.emit_byte(OpCode::GetGlobal as u8, 0);
                    compiler.emit_byte(name_const as u8, 0);
                }
                
                compiler.emit_byte(OpCode::HashMap as u8, 0);
                compiler.emit_byte(public_members.len() as u8, 0);
                compiler.emit_byte(OpCode::Return as u8, 0);

                let function = Rc::new(compiler.function);
                let constant = self.function.chunk.add_constant(crate::eval::value::Value::ObjFunction(function));
                
                self.emit_byte(OpCode::Constant as u8, name.line);
                self.emit_byte(constant as u8, name.line);
                
                // Call it
                self.emit_byte(OpCode::Call as u8, name.line);
                self.emit_byte(0, name.line);
                
                // Wrap in Namespace object
                let name_const = self.function.chunk.add_constant(crate::eval::value::Value::String(name.lexeme.clone()));
                self.emit_byte(OpCode::Namespace as u8, name.line);
                self.emit_byte(name_const as u8, name.line);
                
                self.emit_byte(OpCode::DefineGlobal as u8, name.line);
                self.emit_byte(name_const as u8, name.line);
                
                Ok(())
            }
            Stmt::Import(path, alias) => {
                let path_str: Vec<String> = path.iter().map(|t| t.lexeme.clone()).collect();
                let name = alias.as_ref().map(|t| t.lexeme.clone()).unwrap_or_else(|| path_str.last().unwrap().clone());
                
                let name_const = self.function.chunk.add_constant(crate::eval::value::Value::String(name));
                self.emit_byte(OpCode::Null as u8, 0);
                self.emit_byte(OpCode::DefineGlobal as u8, 0);
                self.emit_byte(name_const as u8, 0);
                Ok(())
            }
            Stmt::Block(stmts) => {
                self.begin_scope();
                for s in stmts {
                    self.compile_stmt(s)?;
                }
                self.end_scope();
                Ok(())
            }
            Stmt::If(condition, then_branch, else_branch) => {
                self.compile_expr(condition)?;
                let then_jump = self.emit_jump(OpCode::JumpIfFalse as u8);
                self.emit_byte(OpCode::Pop as u8, 0);
                
                self.compile_stmt(then_branch)?;
                let else_jump = self.emit_jump(OpCode::Jump as u8);
                
                self.patch_jump(then_jump);
                self.emit_byte(OpCode::Pop as u8, 0);
                
                if let Some(else_stmt) = else_branch {
                    self.compile_stmt(else_stmt)?;
                }
                self.patch_jump(else_jump);
                Ok(())
            }
            Stmt::While(condition, body) => {
                let loop_start = self.function.chunk.code.len();
                self.compile_expr(condition)?;
                let exit_jump = self.emit_jump(OpCode::JumpIfFalse as u8);
                self.emit_byte(OpCode::Pop as u8, 0);
                
                self.compile_stmt(body)?;
                self.emit_loop(loop_start);
                
                self.patch_jump(exit_jump);
                self.emit_byte(OpCode::Pop as u8, 0);
                Ok(())
            }
            Stmt::For(item_name, collection, body) => {
                self.begin_scope();
                self.compile_expr(collection)?;
                let coll_local = "_collection".to_string();
                self.add_local(coll_local.clone());
                
                let zero = self.function.chunk.add_constant(crate::eval::value::Value::Int(0));
                self.emit_constant(zero as u8, item_name.line);
                let index_local = "_i".to_string();
                self.add_local(index_local.clone());
                
                let loop_start = self.function.chunk.code.len();
                let coll_idx = self.resolve_local(&coll_local).unwrap();
                let i_idx = self.resolve_local(&index_local).unwrap();
                
                self.emit_byte(OpCode::GetLocal as u8, item_name.line);
                self.emit_byte(i_idx as u8, item_name.line);
                
                let len_const = self.function.chunk.add_constant(crate::eval::value::Value::String("len".to_string()));
                self.emit_byte(OpCode::GetGlobal as u8, item_name.line);
                self.emit_byte(len_const as u8, item_name.line);
                self.emit_byte(OpCode::GetLocal as u8, item_name.line);
                self.emit_byte(coll_idx as u8, item_name.line);
                self.emit_byte(OpCode::Call as u8, item_name.line);
                self.emit_byte(1, item_name.line);
                
                self.emit_byte(OpCode::Less as u8, item_name.line);
                let exit_jump = self.emit_jump(OpCode::JumpIfFalse as u8);
                self.emit_byte(OpCode::Pop as u8, 0);
                
                self.begin_scope();
                self.emit_byte(OpCode::GetLocal as u8, item_name.line);
                self.emit_byte(coll_idx as u8, item_name.line);
                self.emit_byte(OpCode::GetLocal as u8, item_name.line);
                self.emit_byte(i_idx as u8, item_name.line);
                self.emit_byte(OpCode::GetIndex as u8, item_name.line);
                self.add_local(item_name.lexeme.clone());
                
                self.compile_stmt(body)?;
                self.end_scope();
                
                self.emit_byte(OpCode::GetLocal as u8, item_name.line);
                self.emit_byte(i_idx as u8, item_name.line);
                let one = self.function.chunk.add_constant(crate::eval::value::Value::Int(1));
                self.emit_constant(one as u8, item_name.line);
                self.emit_byte(OpCode::Add as u8, item_name.line);
                self.emit_byte(OpCode::SetLocal as u8, item_name.line);
                self.emit_byte(i_idx as u8, item_name.line);
                self.emit_byte(OpCode::Pop as u8, 0);
                
                self.emit_loop(loop_start);
                self.patch_jump(exit_jump);
                self.emit_byte(OpCode::Pop as u8, 0);
                self.end_scope();
                Ok(())
            }
            Stmt::Return(keyword, value) => {
                if let Some(expr) = value {
                    self.compile_expr(expr)?;
                } else {
                    self.emit_byte(OpCode::Null as u8, keyword.line);
                }
                self.emit_byte(OpCode::Return as u8, keyword.line);
                Ok(())
            }
            Stmt::Directive(_) => Ok(()),
        }
    }

    fn compile_expr(&mut self, expr: &Expr) -> Result<(), LavinaError> {
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(i) => {
                    let constant = self.function.chunk.add_constant(crate::eval::value::Value::Int(*i));
                    self.emit_constant(constant as u8, 0);
                }
                Literal::Float(f) => {
                    let constant = self.function.chunk.add_constant(crate::eval::value::Value::Float(*f));
                    self.emit_constant(constant as u8, 0);
                }
                Literal::String(s) => {
                    let constant = self.function.chunk.add_constant(crate::eval::value::Value::String(s.clone()));
                    self.emit_constant(constant as u8, 0);
                }
                Literal::Bool(true) => self.emit_byte(OpCode::True as u8, 0),
                Literal::Bool(false) => self.emit_byte(OpCode::False as u8, 0),
                Literal::Null => self.emit_byte(OpCode::Null as u8, 0),
            },
            Expr::Variable(name) => {
                if let Some(arg) = self.resolve_local(&name.lexeme) {
                    self.emit_byte(OpCode::GetLocal as u8, name.line);
                    self.emit_byte(arg as u8, name.line);
                } else {
                    let constant = self.function.chunk.add_constant(crate::eval::value::Value::String(name.lexeme.clone()));
                    self.emit_byte(OpCode::GetGlobal as u8, name.line);
                    self.emit_byte(constant as u8, name.line);
                }
            }
            Expr::Assign(name, value) => {
                self.compile_expr(value)?;
                if let Some(arg) = self.resolve_local(&name.lexeme) {
                    self.emit_byte(OpCode::SetLocal as u8, name.line);
                    self.emit_byte(arg as u8, name.line);
                } else {
                    let constant = self.function.chunk.add_constant(crate::eval::value::Value::String(name.lexeme.clone()));
                    self.emit_byte(OpCode::SetGlobal as u8, name.line);
                    self.emit_byte(constant as u8, name.line);
                }
            }
            Expr::Binary(left, op, right) => {
                self.compile_expr(left)?;
                self.compile_expr(right)?;
                match op.token_type {
                    TokenType::Plus => self.emit_byte(OpCode::Add as u8, op.line),
                    TokenType::Minus => self.emit_byte(OpCode::Subtract as u8, op.line),
                    TokenType::Star => self.emit_byte(OpCode::Multiply as u8, op.line),
                    TokenType::Slash => self.emit_byte(OpCode::Divide as u8, op.line),
                    TokenType::EqualEqual => self.emit_byte(OpCode::Equal as u8, op.line),
                    TokenType::Greater => self.emit_byte(OpCode::Greater as u8, op.line),
                    TokenType::GreaterEqual => {
                        self.emit_byte(OpCode::Less as u8, op.line);
                        self.emit_byte(OpCode::Not as u8, op.line);
                    }
                    TokenType::Less => self.emit_byte(OpCode::Less as u8, op.line),
                    TokenType::LessEqual => {
                        self.emit_byte(OpCode::Greater as u8, op.line);
                        self.emit_byte(OpCode::Not as u8, op.line);
                    }
                    TokenType::BangEqual => {
                        self.emit_byte(OpCode::Equal as u8, op.line);
                        self.emit_byte(OpCode::Not as u8, op.line);
                    }
                    _ => {}
                }
            }
            Expr::Call(callee, paren, args) => {
                self.compile_expr(callee)?;
                for arg in args {
                    self.compile_expr(arg)?;
                }
                self.emit_byte(OpCode::Call as u8, paren.line);
                self.emit_byte(args.len() as u8, paren.line);
            }
            Expr::Index(collection, bracket, index) => {
                self.compile_expr(collection)?;
                self.compile_expr(index)?;
                self.emit_byte(OpCode::GetIndex as u8, bracket.line);
            }
            Expr::Vector(elements) => {
                for el in elements {
                    self.compile_expr(el)?;
                }
                self.emit_byte(OpCode::Vector as u8, 0);
                self.emit_byte(elements.len() as u8, 0);
            }
            Expr::Map(entries) => {
                for (key, value) in entries {
                    self.compile_expr(key)?;
                    self.compile_expr(value)?;
                }
                self.emit_byte(OpCode::HashMap as u8, 0);
                self.emit_byte(entries.len() as u8, 0);
            }
            Expr::Grouping(expr) => { self.compile_expr(expr)?; }
            _ => {}
        }
        Ok(())
    }

    fn add_local(&mut self, name: String) {
        self.locals.push(Local { name, depth: self.scope_depth });
    }

    fn resolve_local(&self, name: &str) -> Option<usize> {
        for (i, local) in self.locals.iter().enumerate().rev() {
            if local.name == name {
                return Some(i);
            }
        }
        None
    }

    fn begin_scope(&mut self) {
        self.scope_depth += 1;
    }

    fn end_scope(&mut self) {
        self.scope_depth -= 1;
        while !self.locals.is_empty() && self.locals.last().unwrap().depth > self.scope_depth {
            self.emit_byte(OpCode::Pop as u8, 0);
            self.locals.pop();
        }
    }

    fn emit_byte(&mut self, byte: u8, line: usize) {
        self.function.chunk.write(byte, line);
    }

    fn emit_constant(&mut self, value: u8, line: usize) {
        self.emit_byte(OpCode::Constant as u8, line);
        self.emit_byte(value, line);
    }

    fn emit_jump(&mut self, instruction: u8) -> usize {
        self.emit_byte(instruction, 0);
        self.emit_byte(0xff, 0);
        self.emit_byte(0xff, 0);
        self.function.chunk.code.len() - 2
    }

    fn patch_jump(&mut self, offset: usize) {
        let jump = self.function.chunk.code.len() - offset - 2;
        self.function.chunk.code[offset] = ((jump >> 8) & 0xff) as u8;
        self.function.chunk.code[offset + 1] = (jump & 0xff) as u8;
    }

    fn emit_loop(&mut self, loop_start: usize) {
        self.emit_byte(OpCode::Loop as u8, 0);
        let offset = self.function.chunk.code.len() - loop_start + 2;
        self.emit_byte(((offset >> 8) & 0xff) as u8, 0);
        self.emit_byte((offset & 0xff) as u8, 0);
    }
}
