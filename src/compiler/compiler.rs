use crate::compiler::scope::{FunctionType, Local};
use crate::error::ErrorPhase;
use crate::error::LavinaError;
use crate::eval::value::Value;
use crate::lexer::TokenType;
use crate::lexer::scanner::Scanner;
use crate::parser::ast::{Expr, Literal, Stmt, Type, Visibility};
use crate::parser::parser::Parser;
use crate::util::module_resolver::ModuleResolver;
use crate::vm::chunk::{Chunk, UpvalueLoc};
use crate::vm::opcode::OpCode;
use std::collections::HashMap;
use std::fs;
use std::path::PathBuf;

pub struct CompilerLevel {
    pub function: crate::vm::object::ObjFunction,
    pub locals: Vec<Local>,
    pub scope_depth: i32,
    pub upvalues: Vec<UpvalueLoc>,
    pub function_type: FunctionType,
}

pub struct Compiler {
    pub levels: Vec<CompilerLevel>,
    pub compiled_modules: HashMap<PathBuf, crate::vm::object::ObjFunction>,
    pub resolver: ModuleResolver,
}

impl Compiler {
    pub fn new(name: String, f_type: FunctionType) -> Self {
        let mut level = CompilerLevel {
            function: crate::vm::object::ObjFunction {
                arity: 0,
                chunk: Chunk::new(),
                name,
            },
            locals: Vec::new(),
            scope_depth: 0,
            upvalues: Vec::new(),
            function_type: f_type,
        };

        level.locals.push(Local {
            name: "".to_string(),
            depth: 0,
            is_captured: false,
        });

        Self {
            levels: vec![level],
            compiled_modules: HashMap::new(),
            resolver: ModuleResolver::new(String::new()),
        }
    }

    fn current_level(&self) -> &CompilerLevel {
        self.levels.last().unwrap()
    }

    fn current_level_mut(&mut self) -> &mut CompilerLevel {
        self.levels.last_mut().unwrap()
    }

    fn error(&self, message: String, line: usize, column: usize) -> LavinaError {
        LavinaError::new(ErrorPhase::Compiler, message, line, column)
    }

    pub fn compile(
        mut self,
        statements: &[Stmt],
    ) -> Result<crate::vm::object::ObjFunction, LavinaError> {
        for stmt in statements {
            self.compile_stmt(stmt)?;
        }

        self.emit_byte(OpCode::Null as u8, 0);
        self.emit_byte(OpCode::Return as u8, 0);

        Ok(self.levels.pop().unwrap().function)
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

                if self.current_level().function_type != FunctionType::Script
                    || self.current_level().scope_depth > 0
                {
                    self.add_local(name.lexeme.clone());
                } else {
                    let constant = self.add_constant(Value::String(name.lexeme.clone()));
                    self.emit_byte(OpCode::DefineGlobal as u8, name.line);
                    self.emit_byte(constant as u8, name.line);
                }
                Ok(())
            }
            Stmt::Function(decl) => {
                let name = decl.name.lexeme.clone();
                let arity = decl.params.len();

                let mut level = CompilerLevel {
                    function: crate::vm::object::ObjFunction {
                        arity,
                        chunk: Chunk::new(),
                        name: name.clone(),
                    },
                    locals: Vec::new(),
                    scope_depth: 0,
                    upvalues: Vec::new(),
                    function_type: FunctionType::Function,
                };
                level.locals.push(Local {
                    name: "".to_string(),
                    depth: 0,
                    is_captured: false,
                });
                for (param_name, _) in &decl.params {
                    level.locals.push(Local {
                        name: param_name.lexeme.clone(),
                        depth: 0,
                        is_captured: false,
                    });
                }
                self.levels.push(level);

                for s in &decl.body {
                    self.compile_stmt(s)?;
                }

                self.emit_byte(OpCode::Null as u8, 0);
                self.emit_byte(OpCode::Return as u8, 0);
                let mut finished_level = self.levels.pop().unwrap();
                finished_level.function.chunk.upvalues = finished_level.upvalues;

                let constant =
                    self.add_constant(Value::TemplateFunction(Box::new(finished_level.function)));
                self.emit_byte(OpCode::Closure as u8, decl.name.line);
                self.emit_byte(constant as u8, decl.name.line);

                if self.current_level().scope_depth > 0 {
                    self.add_local(name);
                } else {
                    let name_const = self.add_constant(Value::String(name));
                    self.emit_byte(OpCode::DefineGlobal as u8, decl.name.line);
                    self.emit_byte(name_const as u8, decl.name.line);
                }

                Ok(())
            }
            Stmt::Namespace(name, body, _) => {
                let mut level = CompilerLevel {
                    function: crate::vm::object::ObjFunction {
                        arity: 0,
                        chunk: Chunk::new(),
                        name: name.lexeme.clone(),
                    },
                    locals: Vec::new(),
                    scope_depth: 0,
                    upvalues: Vec::new(),
                    function_type: FunctionType::Script,
                };
                level.locals.push(Local {
                    name: "".to_string(),
                    depth: 0,
                    is_captured: false,
                });
                self.levels.push(level);

                for s in body {
                    self.compile_stmt(s)?;
                }

                // Epilogue: collect public members
                self.emit_namespace_epilogue(body);

                let mut finished_level = self.levels.pop().unwrap();
                finished_level.function.chunk.upvalues = finished_level.upvalues;

                let constant =
                    self.add_constant(Value::TemplateFunction(Box::new(finished_level.function)));
                self.emit_byte(OpCode::Closure as u8, name.line);
                self.emit_byte(constant as u8, name.line);
                self.emit_byte(OpCode::Call as u8, name.line);
                self.emit_byte(0, name.line);

                let name_const = self.add_constant(Value::String(name.lexeme.clone()));
                self.emit_byte(OpCode::Namespace as u8, name.line);
                self.emit_byte(name_const as u8, name.line);
                self.emit_byte(OpCode::DefineGlobal as u8, name.line);
                self.emit_byte(name_const as u8, name.line);
                Ok(())
            }
            Stmt::Import(path_tokens, alias) => {
                let path = self.resolver.resolve(path_tokens, ErrorPhase::Compiler)?;
                let module_name = path_tokens.last().unwrap().lexeme.clone();
                let namespace_name = alias
                    .as_ref()
                    .map(|t| t.lexeme.clone())
                    .unwrap_or(module_name.clone());

                if !self.compiled_modules.contains_key(&path) {
                    let source = fs::read_to_string(&path).map_err(|e| {
                        LavinaError::new(
                            ErrorPhase::Compiler,
                            format!("Could not read module file {:?}: {}", path, e),
                            path_tokens[0].line,
                            path_tokens[0].column,
                        )
                    })?;

                    let mut scanner = Scanner::new(source.clone());
                    let (tokens, errors) = scanner.scan_tokens();
                    if !errors.is_empty() {
                        return Err(errors[0].clone());
                    }

                    let mut parser = Parser::new(tokens.clone(), source.clone());
                    let statements = parser.parse_program().map_err(|e| e)?;

                    let mut sub_compiler = Compiler::new(module_name.clone(), FunctionType::Script);
                    sub_compiler.compiled_modules = self.compiled_modules.clone();

                    // Manually compile with epilogue
                    for s in &statements {
                        sub_compiler.compile_stmt(s)?;
                    }
                    sub_compiler.emit_namespace_epilogue(&statements);

                    let module_fn = sub_compiler.levels.pop().unwrap().function;
                    self.compiled_modules.insert(path.clone(), module_fn);
                }

                let function = self.compiled_modules.get(&path).unwrap().clone();
                let constant = self.add_constant(Value::TemplateFunction(Box::new(function)));
                self.emit_byte(OpCode::Closure as u8, path_tokens[0].line);
                self.emit_byte(constant as u8, path_tokens[0].line);
                self.emit_byte(OpCode::Call as u8, path_tokens[0].line);
                self.emit_byte(0, path_tokens[0].line);

                let name_const = self.add_constant(Value::String(namespace_name.clone()));
                self.emit_byte(OpCode::Namespace as u8, path_tokens[0].line);
                self.emit_byte(name_const as u8, path_tokens[0].line);
                self.emit_byte(OpCode::DefineGlobal as u8, path_tokens[0].line);
                self.emit_byte(name_const as u8, path_tokens[0].line);
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
                let loop_start = self.current_level().function.chunk.code.len();
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
                self.add_local("_collection".to_string());
                let zero = self.add_constant(Value::Int(0));
                self.emit_constant(zero as u8, item_name.line);
                self.add_local("_i".to_string());

                let loop_start = self.current_level().function.chunk.code.len();
                let coll_idx = self.resolve_local("_collection").unwrap();
                let i_idx = self.resolve_local("_i").unwrap();

                self.emit_byte(OpCode::GetLocal as u8, item_name.line);
                self.emit_byte(i_idx as u8, item_name.line);

                let len_const = self.add_constant(Value::String("len".to_string()));
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
                let one = self.add_constant(Value::Int(1));
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
                    if self.current_level().function.name.ends_with("::__init__") {
                        return Err(self.error(
                            "Can't return a value from an initializer.".to_string(),
                            keyword.line,
                            keyword.column,
                        ));
                    }
                    self.compile_expr(expr)?;
                } else {
                    if self.current_level().function.name.ends_with("::__init__") {
                        self.emit_byte(OpCode::GetLocal as u8, keyword.line);
                        self.emit_byte(0, keyword.line);
                    } else {
                        self.emit_byte(OpCode::Null as u8, keyword.line);
                    }
                }
                self.emit_byte(OpCode::Return as u8, keyword.line);
                Ok(())
            }
            Stmt::Directive(_) => Ok(()),
            Stmt::Class(name, body, _) => {
                let name_str = name.lexeme.clone();
                let name_const = self.add_constant(Value::String(name_str.clone()));
                self.emit_byte(OpCode::Class as u8, name.line);
                self.emit_byte(name_const as u8, name.line);

                // Define global early so we can refer to the class in its methods
                self.emit_byte(OpCode::DefineGlobal as u8, name.line);
                self.emit_byte(name_const as u8, name.line);

                // Now get it back on stack for methods
                self.emit_byte(OpCode::GetGlobal as u8, name.line);
                self.emit_byte(name_const as u8, name.line);

                for s in body {
                    match s {
                        Stmt::Function(decl) => {
                            let method_name = decl.name.lexeme.clone();
                            let is_init = method_name == "__init__";

                            // Push new level for method
                            let mut level = CompilerLevel {
                                function: crate::vm::object::ObjFunction {
                                    arity: decl.params.len(),
                                    chunk: Chunk::new(),
                                    name: format!("{}::{}", name_str, method_name),
                                },
                                locals: Vec::new(),
                                scope_depth: 0,
                                upvalues: Vec::new(),
                                function_type: FunctionType::Function,
                            };

                            if !decl.is_static {
                                // Slot 0 is 'this' for instance methods
                                level.locals.push(Local {
                                    name: "this".to_string(),
                                    depth: 0,
                                    is_captured: false,
                                });
                            } else {
                                // Slot 0 is empty for static methods (script-like)
                                level.locals.push(Local {
                                    name: "".to_string(),
                                    depth: 0,
                                    is_captured: false,
                                });
                            }

                            for (param_name, _) in &decl.params {
                                level.locals.push(Local {
                                    name: param_name.lexeme.clone(),
                                    depth: 0,
                                    is_captured: false,
                                });
                            }
                            self.levels.push(level);

                            for body_stmt in &decl.body {
                                self.compile_stmt(body_stmt)?;
                            }

                            if is_init {
                                self.emit_byte(OpCode::GetLocal as u8, 0);
                                self.emit_byte(0, 0); // slot 0 is 'this'
                            } else {
                                self.emit_byte(OpCode::Null as u8, 0);
                            }
                            self.emit_byte(OpCode::Return as u8, 0);

                            let mut finished_level = self.levels.pop().unwrap();
                            finished_level.function.chunk.upvalues = finished_level.upvalues;

                            let constant = self.add_constant(Value::TemplateFunction(Box::new(
                                finished_level.function,
                            )));
                            self.emit_byte(OpCode::Closure as u8, decl.name.line);
                            self.emit_byte(constant as u8, decl.name.line);

                            let method_const = self.add_constant(Value::String(method_name));
                            self.emit_byte(OpCode::Method as u8, decl.name.line);
                            self.emit_byte(method_const as u8, decl.name.line);
                        }
                        _ => {} // Ignore fields for now, they are dynamic
                    }
                }

                self.emit_byte(OpCode::Pop as u8, name.line);
                Ok(())
            }
            Stmt::Struct(name, body, visibility) => {
                // For now, structs are just classes
                self.compile_stmt(&Stmt::Class(name.clone(), body.clone(), visibility.clone()))?;
                Ok(())
            }
            Stmt::Enum(name, variants, _) => {
                let enum_name = name.lexeme.clone();
                let mut public_members = Vec::new();

                for variant in variants {
                    let variant_name = variant.name.lexeme.clone();
                    let arity = variant.types.len();

                    if arity == 0 {
                        // Constant variant: just a map { "_tag": "variant_name" }
                        let tag_key = self.add_constant(Value::String("_tag".to_string()));
                        let tag_val = self.add_constant(Value::String(variant_name.clone()));
                        self.emit_constant(tag_key as u8, variant.name.line);
                        self.emit_constant(tag_val as u8, variant.name.line);
                        self.emit_byte(OpCode::HashMap as u8, variant.name.line);
                        self.emit_byte(1, variant.name.line); // 1 pair

                        let name_const = self.add_constant(Value::String(variant_name.clone()));
                        self.emit_byte(OpCode::DefineGlobal as u8, variant.name.line);
                        self.emit_byte(name_const as u8, variant.name.line);
                        public_members.push(variant_name);
                    } else {
                        // Function variant
                        let mut level = CompilerLevel {
                            function: crate::vm::object::ObjFunction {
                                arity,
                                chunk: Chunk::new(),
                                name: format!("{}::{}", enum_name, variant_name),
                            },
                            locals: Vec::new(),
                            scope_depth: 0,
                            upvalues: Vec::new(),
                            function_type: FunctionType::Function,
                        };
                        level.locals.push(Local {
                            name: "".to_string(),
                            depth: 0,
                            is_captured: false,
                        });
                        for i in 0..arity {
                            level.locals.push(Local {
                                name: format!("arg{}", i),
                                depth: 0,
                                is_captured: false,
                            });
                        }
                        self.levels.push(level);

                        // Build the enum object
                        // Tag
                        let tag_key = self.add_constant(Value::String("_tag".to_string()));
                        let tag_val = self.add_constant(Value::String(variant_name.clone()));
                        self.emit_constant(tag_key as u8, variant.name.line);
                        self.emit_constant(tag_val as u8, variant.name.line);

                        // Arguments
                        for i in 0..arity {
                            let arg_key = self.add_constant(Value::String(i.to_string()));
                            self.emit_constant(arg_key as u8, variant.name.line);
                            self.emit_byte(OpCode::GetLocal as u8, variant.name.line);
                            self.emit_byte((i + 1) as u8, variant.name.line);
                        }

                        self.emit_byte(OpCode::HashMap as u8, variant.name.line);
                        self.emit_byte((arity + 1) as u8, variant.name.line); // arity + tag

                        self.emit_byte(OpCode::Return as u8, variant.name.line);
                        let finished_level = self.levels.pop().unwrap();
                        let constant = self.add_constant(Value::TemplateFunction(Box::new(
                            finished_level.function,
                        )));

                        self.emit_byte(OpCode::Closure as u8, variant.name.line);
                        self.emit_byte(constant as u8, variant.name.line);

                        let name_const = self.add_constant(Value::String(variant_name.clone()));
                        self.emit_byte(OpCode::DefineGlobal as u8, variant.name.line);
                        self.emit_byte(name_const as u8, variant.name.line);
                        public_members.push(variant_name);
                    }
                }

                // Now create a namespace for the enum
                for member in &public_members {
                    let m_const = self.add_constant(Value::String(member.clone()));
                    self.emit_constant(m_const as u8, name.line);
                    self.emit_byte(OpCode::GetGlobal as u8, name.line);
                    self.emit_byte(m_const as u8, name.line);
                }
                self.emit_byte(OpCode::HashMap as u8, name.line);
                self.emit_byte(public_members.len() as u8, name.line);

                let ns_const = self.add_constant(Value::String(enum_name.clone()));
                self.emit_byte(OpCode::Namespace as u8, name.line);
                self.emit_byte(ns_const as u8, name.line);

                self.emit_byte(OpCode::DefineGlobal as u8, name.line);
                self.emit_byte(ns_const as u8, name.line);

                Ok(())
            }
            Stmt::Try(try_body, catch_token, exception_name, catch_body) => {
                let try_jump = self.emit_jump(OpCode::Try as u8);

                self.compile_stmt(try_body)?;
                self.emit_byte(OpCode::EndTry as u8, catch_token.line);

                let catch_jump = self.emit_jump(OpCode::Jump as u8);

                self.patch_jump(try_jump);

                // Exception value is on stack
                if let Some(name) = exception_name {
                    self.begin_scope();
                    self.add_local(name.lexeme.clone());
                    self.compile_stmt(catch_body)?;
                    self.end_scope();
                } else {
                    self.emit_byte(OpCode::Pop as u8, catch_token.line);
                    self.compile_stmt(catch_body)?;
                }

                self.patch_jump(catch_jump);
                Ok(())
            }
            Stmt::Const(name, _, init, _) => {
                self.compile_expr(init)?;
                if self.current_level().function_type != FunctionType::Script
                    || self.current_level().scope_depth > 0
                {
                    self.add_local(name.lexeme.clone());
                } else {
                    let constant = self.add_constant(Value::String(name.lexeme.clone()));
                    self.emit_byte(OpCode::DefineGlobal as u8, name.line);
                    self.emit_byte(constant as u8, name.line);
                }
                Ok(())
            }
        }
    }

    fn emit_namespace_epilogue(&mut self, body: &[Stmt]) {
        let mut public_members = Vec::new();
        for s in body {
            match s {
                Stmt::Function(f) if f.visibility == Visibility::Public => {
                    public_members.push(f.name.lexeme.clone());
                }
                Stmt::Let(n, _, _, v) if v == &Visibility::Public => {
                    public_members.push(n.lexeme.clone());
                }
                Stmt::Class(n, _, v) if v == &Visibility::Public => {
                    public_members.push(n.lexeme.clone())
                }
                Stmt::Struct(n, _, v) if v == &Visibility::Public => {
                    public_members.push(n.lexeme.clone())
                }
                Stmt::Enum(n, _, v) if v == &Visibility::Public => {
                    public_members.push(n.lexeme.clone())
                }
                Stmt::Namespace(n, _, v) if v == &Visibility::Public => {
                    public_members.push(n.lexeme.clone())
                }
                _ => {}
            }
        }

        for m_name in &public_members {
            let name_const = self.add_constant(Value::String(m_name.clone()));
            self.emit_byte(OpCode::Constant as u8, 0);
            self.emit_byte(name_const as u8, 0);
            self.emit_byte(OpCode::GetGlobal as u8, 0);
            self.emit_byte(name_const as u8, 0);
        }

        let count = public_members.len();
        self.emit_byte(OpCode::HashMap as u8, 0);
        self.emit_byte(count as u8, 0);
        self.emit_byte(OpCode::Return as u8, 0);
    }

    fn compile_expr(&mut self, expr: &Expr) -> Result<(), LavinaError> {
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(i) => {
                    let c = self.add_constant(Value::Int(*i));
                    self.emit_constant(c as u8, 0);
                }
                Literal::Float(f) => {
                    let c = self.add_constant(Value::Float(*f));
                    self.emit_constant(c as u8, 0);
                }
                Literal::String(s) => {
                    let c = self.add_constant(Value::String(s.clone()));
                    self.emit_constant(c as u8, 0);
                }
                Literal::Bool(true) => self.emit_byte(OpCode::True as u8, 0),
                Literal::Bool(false) => self.emit_byte(OpCode::False as u8, 0),
                Literal::Null => self.emit_byte(OpCode::Null as u8, 0),
            },
            Expr::Variable(name) => {
                if let Some(arg) = self.resolve_local(&name.lexeme) {
                    self.emit_byte(OpCode::GetLocal as u8, name.line);
                    self.emit_byte(arg as u8, name.line);
                } else if let Some(arg) = self.resolve_upvalue(self.levels.len() - 1, &name.lexeme)
                {
                    self.emit_byte(OpCode::GetUpvalue as u8, name.line);
                    self.emit_byte(arg as u8, name.line);
                } else {
                    let constant = self.add_constant(Value::String(name.lexeme.clone()));
                    self.emit_byte(OpCode::GetGlobal as u8, name.line);
                    self.emit_byte(constant as u8, name.line);
                }
            }
            Expr::Assign(name, value) => {
                self.compile_expr(value)?;
                if let Some(arg) = self.resolve_local(&name.lexeme) {
                    self.emit_byte(OpCode::SetLocal as u8, name.line);
                    self.emit_byte(arg as u8, name.line);
                } else if let Some(arg) = self.resolve_upvalue(self.levels.len() - 1, &name.lexeme)
                {
                    self.emit_byte(OpCode::SetUpvalue as u8, name.line);
                    self.emit_byte(arg as u8, name.line);
                } else {
                    let constant = self.add_constant(Value::String(name.lexeme.clone()));
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
                    TokenType::Less => self.emit_byte(OpCode::Less as u8, op.line),
                    TokenType::GreaterEqual => {
                        self.emit_byte(OpCode::Less as u8, op.line);
                        self.emit_byte(OpCode::Not as u8, op.line);
                    }
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
            Expr::Grouping(expr) => self.compile_expr(expr)?,
            Expr::Unary(op, expr) => {
                self.compile_expr(expr)?;
                match op.token_type {
                    TokenType::Minus => self.emit_byte(OpCode::Negate as u8, op.line),
                    TokenType::Bang => self.emit_byte(OpCode::Not as u8, op.line),
                    _ => {}
                }
            }
            Expr::Logical(left, op, right) => {
                self.compile_expr(left)?;
                if op.token_type == TokenType::Or {
                    let else_jump = self.emit_jump(OpCode::JumpIfFalse as u8);
                    let end_jump = self.emit_jump(OpCode::Jump as u8);
                    self.patch_jump(else_jump);
                    self.emit_byte(OpCode::Pop as u8, 0);
                    self.compile_expr(right)?;
                    self.patch_jump(end_jump);
                } else {
                    // AND
                    let end_jump = self.emit_jump(OpCode::JumpIfFalse as u8);
                    self.emit_byte(OpCode::Pop as u8, 0);
                    self.compile_expr(right)?;
                    self.patch_jump(end_jump);
                }
            }
            Expr::Get(obj, name) => {
                self.compile_expr(obj)?;
                let name_const = self.add_constant(Value::String(name.lexeme.clone()));
                self.emit_byte(OpCode::GetProperty as u8, name.line);
                self.emit_byte(name_const as u8, name.line);
            }
            Expr::StaticGet(obj, name) => {
                self.compile_expr(obj)?;
                let name_const = self.add_constant(Value::String(name.lexeme.clone()));
                self.emit_byte(OpCode::GetProperty as u8, name.line);
                self.emit_byte(name_const as u8, name.line);
            }
            Expr::Set(obj, name, value) => {
                self.compile_expr(obj)?;
                self.compile_expr(value)?;
                let name_const = self.add_constant(Value::String(name.lexeme.clone()));
                self.emit_byte(OpCode::SetProperty as u8, name.line);
                self.emit_byte(name_const as u8, name.line);
            }
            Expr::This(token) => {
                if let Some(arg) = self.resolve_local("this") {
                    self.emit_byte(OpCode::GetLocal as u8, token.line);
                    self.emit_byte(arg as u8, token.line);
                } else if let Some(arg) = self.resolve_upvalue(self.levels.len() - 1, "this") {
                    self.emit_byte(OpCode::GetUpvalue as u8, token.line);
                    self.emit_byte(arg as u8, token.line);
                } else {
                    return Err(self.error(
                        "Can't use 'this' outside of a class method.".to_string(),
                        token.line,
                        token.column,
                    ));
                }
            }
            Expr::Cast(expr, target_type) => {
                self.compile_expr(expr)?;
                let type_name = self.type_to_string(target_type);
                let constant = self.add_constant(Value::String(type_name));
                self.emit_constant(constant as u8, 0);
                self.emit_byte(OpCode::Cast as u8, 0);
            }
            Expr::Throw(expr) => {
                self.compile_expr(expr)?;
                self.emit_byte(OpCode::Throw as u8, 0);
            }
        }
        Ok(())
    }

    fn type_to_string(&self, t: &Type) -> String {
        match t {
            Type::Int => "int".to_string(),
            Type::Float => "float".to_string(),
            Type::String => "string".to_string(),
            Type::Bool => "bool".to_string(),
            Type::Array(_) => "vector".to_string(),
            Type::Dict(_, _) => "hashmap".to_string(),
            Type::Dynamic => "dynamic".to_string(),
            Type::Null => "null".to_string(),
            Type::Custom(name) => name.clone(),
            _ => "dynamic".to_string(),
        }
    }

    fn add_local(&mut self, name: String) {
        let depth = self.current_level().scope_depth;
        self.current_level_mut().locals.push(Local {
            name,
            depth,
            is_captured: false,
        });
    }

    fn resolve_local(&self, name: &str) -> Option<usize> {
        for (i, local) in self.current_level().locals.iter().enumerate().rev() {
            if local.name == name {
                return Some(i);
            }
        }
        None
    }

    fn resolve_upvalue(&mut self, level_idx: usize, name: &str) -> Option<usize> {
        if level_idx == 0 {
            return None;
        }
        let parent_idx = level_idx - 1;
        let mut local_idx = None;
        for (i, local) in self.levels[parent_idx].locals.iter().enumerate().rev() {
            if local.name == name {
                local_idx = Some(i);
                break;
            }
        }
        if let Some(idx) = local_idx {
            self.levels[parent_idx].locals[idx].is_captured = true;
            return Some(self.add_upvalue(level_idx, idx as u8, true));
        }
        if let Some(upvalue) = self.resolve_upvalue(parent_idx, name) {
            return Some(self.add_upvalue(level_idx, upvalue as u8, false));
        }
        None
    }

    fn add_upvalue(&mut self, level_idx: usize, index: u8, is_local: bool) -> usize {
        let level = &mut self.levels[level_idx];
        for (i, uv) in level.upvalues.iter().enumerate() {
            if uv.index == index && uv.is_local == is_local {
                return i;
            }
        }
        level.upvalues.push(UpvalueLoc { index, is_local });
        level.upvalues.len() - 1
    }

    fn begin_scope(&mut self) {
        self.current_level_mut().scope_depth += 1;
    }
    fn end_scope(&mut self) {
        let depth = self.current_level().scope_depth;
        self.current_level_mut().scope_depth -= 1;
        while !self.current_level().locals.is_empty()
            && self.current_level().locals.last().unwrap().depth == depth
        {
            if self.current_level().locals.last().unwrap().is_captured {
                self.emit_byte(OpCode::CloseUpvalue as u8, 0);
            } else {
                self.emit_byte(OpCode::Pop as u8, 0);
            }
            self.current_level_mut().locals.pop();
        }
    }

    fn emit_byte(&mut self, byte: u8, line: usize) {
        self.current_level_mut().function.chunk.write(byte, line);
    }
    fn emit_constant(&mut self, value: u8, line: usize) {
        self.emit_byte(OpCode::Constant as u8, line);
        self.emit_byte(value, line);
    }
    fn add_constant(&mut self, value: Value) -> usize {
        self.current_level_mut().function.chunk.add_constant(value)
    }
    fn emit_jump(&mut self, instruction: u8) -> usize {
        self.emit_byte(instruction, 0);
        self.emit_byte(0xff, 0);
        self.emit_byte(0xff, 0);
        self.current_level().function.chunk.code.len() - 2
    }
    fn patch_jump(&mut self, offset: usize) {
        let jump = self.current_level().function.chunk.code.len() - offset - 2;
        self.current_level_mut().function.chunk.code[offset] = ((jump >> 8) & 0xff) as u8;
        self.current_level_mut().function.chunk.code[offset + 1] = (jump & 0xff) as u8;
    }
    fn emit_loop(&mut self, loop_start: usize) {
        self.emit_byte(OpCode::Loop as u8, 0);
        let offset = self.current_level().function.chunk.code.len() - loop_start + 2;
        self.emit_byte(((offset >> 8) & 0xff) as u8, 0);
        self.emit_byte((offset & 0xff) as u8, 0);
    }
}
