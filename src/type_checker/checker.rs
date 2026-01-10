use crate::parser::ast::{Expr, Stmt, Type, Literal, Visibility};
use crate::error::{LavinaError, ErrorPhase};
use crate::lexer::TokenType;
use crate::lexer::scanner::Scanner;
use crate::parser::parser::Parser;
use crate::type_checker::env::{TypeInfo, TypeEnvironment};
use crate::util::module_resolver::ModuleResolver;
use std::collections::HashMap;
use std::path::PathBuf;
use std::fs;

pub struct TypeChecker {
    env: TypeEnvironment,
    source: String,
    checked_modules: HashMap<PathBuf, HashMap<String, (TypeInfo, Visibility)>>,
    resolver: ModuleResolver,
    current_class: Option<String>,
    current_return_types: Vec<Type>,
}

impl TypeChecker {
    pub fn new() -> Self {
        let mut checker = Self {
            env: TypeEnvironment::new(),
            source: String::new(),
            checked_modules: HashMap::new(),
            resolver: ModuleResolver::new(String::new()),
            current_class: None,
            current_return_types: Vec::new(),
        };
        
        // Register internal native functions
        checker.env.define("__native_print".to_string(), TypeInfo::Function(Type::Void, vec![Type::Dynamic], false, true), Visibility::Private);
        checker.env.define("__native_len".to_string(), TypeInfo::Function(Type::Int, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_clock".to_string(), TypeInfo::Function(Type::Float, vec![], false, false), Visibility::Private);
        checker.env.define("__native_typeof".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_sqrt".to_string(), TypeInfo::Function(Type::Float, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_abs".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_sin".to_string(), TypeInfo::Function(Type::Float, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_cos".to_string(), TypeInfo::Function(Type::Float, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_tan".to_string(), TypeInfo::Function(Type::Float, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_min".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic, Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_max".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic, Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_floor".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_ceil".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_round".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_input".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_string_lower".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_string_upper".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_string_len".to_string(), TypeInfo::Function(Type::Int, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_string_trim".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_string_replace".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic, Type::String, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_string_contains".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_string_starts_with".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_string_ends_with".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_string_split".to_string(), TypeInfo::Function(Type::Array(Box::new(Type::String)), vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_string_idxof".to_string(), TypeInfo::Function(Type::Int, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_string_is_empty".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_vector_push".to_string(), TypeInfo::Function(Type::Void, vec![Type::Dynamic, Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_vector_pop".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_vector_clear".to_string(), TypeInfo::Function(Type::Void, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_vector_remove".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic, Type::Int], false, false), Visibility::Private);
        checker.env.define("__native_vector_join".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_vector_contains".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic, Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_vector_reverse".to_string(), TypeInfo::Function(Type::Void, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_vector_is_empty".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_hashmap_has".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_hashmap_clear".to_string(), TypeInfo::Function(Type::Void, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_hashmap_remove".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_hashmap_keys".to_string(), TypeInfo::Function(Type::Array(Box::new(Type::String)), vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_hashmap_values".to_string(), TypeInfo::Function(Type::Array(Box::new(Type::Dynamic)), vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_hashmap_is_empty".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_range".to_string(), TypeInfo::Function(Type::Array(Box::new(Type::Int)), vec![Type::Int, Type::Int], false, false), Visibility::Private);
        checker.env.define("__native_string_starts_with".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_string_ends_with".to_string(), TypeInfo::Function(Type::Bool, vec![Type::Dynamic, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_memory_addr".to_string(), TypeInfo::Function(Type::Int, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_memory_deref".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::Int], false, false), Visibility::Private);
        checker.env.define("__native_memory_delete".to_string(), TypeInfo::Function(Type::Void, vec![Type::Int], false, false), Visibility::Private);
        checker.env.define("__native_memory_allocated".to_string(), TypeInfo::Function(Type::Int, vec![], false, false), Visibility::Private);
        checker.env.define("__native_memory_sizeof".to_string(), TypeInfo::Function(Type::Int, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_fs_read".to_string(), TypeInfo::Function(Type::String, vec![Type::String], false, false), Visibility::Private);
        checker.env.define("__native_fs_write".to_string(), TypeInfo::Function(Type::Void, vec![Type::String, Type::String], false, false), Visibility::Private);
        checker.env.define("__native_fs_exists".to_string(), TypeInfo::Function(Type::Bool, vec![Type::String], false, false), Visibility::Private);
        checker.env.define("__native_os_args".to_string(), TypeInfo::Function(Type::Array(Box::new(Type::String)), vec![], false, false), Visibility::Private);
        checker.env.define("__native_os_env".to_string(), TypeInfo::Function(Type::Nullable(Box::new(Type::String)), vec![Type::String], false, false), Visibility::Private);
        checker.env.define("__native_json_stringify".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic], false, false), Visibility::Private);
        checker.env.define("__native_json_parse".to_string(), TypeInfo::Function(Type::Dynamic, vec![Type::String], false, false), Visibility::Private);
        
        // Expose top-level print/len/etc for convenience (until we have core lib)
        checker.env.define("print".to_string(), TypeInfo::Function(Type::Void, vec![Type::Dynamic], false, true), Visibility::Public);
        checker.env.define("len".to_string(), TypeInfo::Function(Type::Int, vec![Type::Dynamic], false, false), Visibility::Public);
        checker.env.define("clock".to_string(), TypeInfo::Function(Type::Float, vec![], false, false), Visibility::Public);
        checker.env.define("typeof".to_string(), TypeInfo::Function(Type::String, vec![Type::Dynamic], false, false), Visibility::Public);
        checker.env.define("range".to_string(), TypeInfo::Function(Type::Array(Box::new(Type::Int)), vec![Type::Int, Type::Int], false, false), Visibility::Public);
        
        checker
    }

    pub fn set_source(&mut self, source: String) {
        self.source = source.clone();
        self.resolver = ModuleResolver::new(source);
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
                let final_type = if let Some(init) = initializer {
                    let inferred_type = self.check_expr(init)?;
                    if let Some(ann) = type_ann {
                        if ann != &Type::Auto && !self.is_assignable(ann, &inferred_type) {
                            return Err(LavinaError::new(
                                ErrorPhase::TypeChecker,
                                format!("Type mismatch: expected {:?}, but got {:?}", ann, inferred_type),
                                name.line,
                                name.column,
                            ).with_context(&self.source));
                        }
                        if ann == &Type::Auto { inferred_type } else { ann.clone() }
                    } else {
                        inferred_type
                    }
                } else {
                    if let Some(ann) = type_ann {
                        if ann == &Type::Auto {
                            return Err(LavinaError::new(
                                ErrorPhase::TypeChecker,
                                "Variable with 'auto' type must have an initializer.".to_string(),
                                name.line,
                                name.column,
                            ).with_context(&self.source));
                        }
                        ann.clone()
                    } else {
                        Type::Dynamic // Default for unannotated, uninitialized variables
                    }
                };

                self.env.define(name.lexeme.clone(), TypeInfo::Variable(final_type), visibility.clone());
                Ok(())
            }
            Stmt::Function(decl) => {
                let param_types: Vec<Type> = decl.params.iter().map(|(_, t)| t.clone()).collect();
                self.env.define(decl.name.lexeme.clone(), TypeInfo::Function(decl.return_type.clone(), param_types, decl.is_static, false), decl.visibility.clone());
                
                // Check body in a new scope
                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));

                // If it's an instance method, define 'this'
                if !decl.is_static {
                    if let Some(class_name) = &self.current_class {
                        self.env.define("this".to_string(), TypeInfo::Variable(Type::Custom(class_name.clone())), Visibility::Public);
                    }
                }

                for (param_name, param_type) in &decl.params {
                    self.env.define(param_name.lexeme.clone(), TypeInfo::Variable(param_type.clone()), Visibility::Public);
                }

                let old_return_types = std::mem::take(&mut self.current_return_types);
                self.check_statements(&decl.body)?;
                let captured_returns = std::mem::replace(&mut self.current_return_types, old_return_types);

                if decl.return_type == Type::Auto {
                    let mut inferred = Type::Void;
                    if !captured_returns.is_empty() {
                        inferred = captured_returns[0].clone();
                        for t in &captured_returns[1..] {
                            if &inferred != t && t != &Type::Dynamic {
                                inferred = Type::Dynamic;
                                break;
                            }
                        }
                    }
                    // Update function signature in environment
                    let pts: Vec<Type> = decl.params.iter().map(|(_, t)| t.clone()).collect();
                    self.env.enclosing.as_mut().unwrap().define(decl.name.lexeme.clone(), TypeInfo::Function(inferred, pts, decl.is_static, false), decl.visibility.clone());
                } else {
                    for t in captured_returns {
                        if !self.is_assignable(&decl.return_type, &t) {
                            return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Function '{}' returns {:?}, but body returns {:?}.", decl.name.lexeme, decl.return_type, t), decl.name.line, decl.name.column).with_context(&self.source));
                        }
                    }
                }

                let mut current_env = self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut current_env);
                Ok(())
            }
            Stmt::Namespace(name, body, visibility) => {
                // Collect members of the namespace
                let mut members = HashMap::new();
                
                // First pass: register names
                for s in body {
                    match s {
                        Stmt::Function(f) => {
                            let pts: Vec<Type> = f.params.iter().map(|(_, t)| t.clone()).collect();
                        members.insert(f.name.lexeme.clone(), (TypeInfo::Function(f.return_type.clone(), pts, f.is_static, false), f.visibility.clone()));
                        }
                        Stmt::Let(n, t, _, v) => {
                            let final_t = t.clone().unwrap_or(Type::Auto);
                            members.insert(n.lexeme.clone(), (TypeInfo::Variable(final_t), v.clone()));
                        }
                        _ => {}
                    }
                }

                self.env.define(name.lexeme.clone(), TypeInfo::Namespace(name.lexeme.clone(), members), visibility.clone());
                
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
                    let t = self.check_expr(expr)?;
                    self.current_return_types.push(t);
                } else {
                    self.current_return_types.push(Type::Void);
                }
                Ok(())
            }
            Stmt::Import(path_tokens, alias) => {
                let path = self.resolver.resolve(path_tokens, ErrorPhase::TypeChecker)?;
                let module_name = path_tokens.last().unwrap().lexeme.clone();
                let namespace_name = alias.as_ref().map(|t| t.lexeme.clone()).unwrap_or(module_name.clone());

                if !self.checked_modules.contains_key(&path) {
                    let source = fs::read_to_string(&path).map_err(|e| {
                        LavinaError::new(
                            ErrorPhase::TypeChecker,
                            format!("Could not read module file {:?}: {}", path, e),
                            path_tokens[0].line,
                            path_tokens[0].column,
                        ).with_context(&self.source)
                    })?;

                    let mut scanner = Scanner::new(source.clone());
                    let (tokens, errors) = scanner.scan_tokens();
                    if !errors.is_empty() {
                        return Err(errors[0].clone());
                    }

                    let mut parser = Parser::new(tokens.clone(), source.clone());
                    let statements = parser.parse_program().map_err(|e| e)?;

                    let mut sub_checker = TypeChecker::new();
                    sub_checker.set_source(source);
                    sub_checker.checked_modules = self.checked_modules.clone();
                    sub_checker.check_statements(&statements)?;
                    
                    // After checking, merge back the modules checked by sub_checker
                    self.checked_modules = sub_checker.checked_modules;
                    
                    // Filter public members for the new namespace
                    let mut public_members = HashMap::new();
                    for (name, (info, vis)) in sub_checker.env.values {
                        if vis == Visibility::Public {
                            public_members.insert(name, (info, vis));
                        }
                    }
                    self.checked_modules.insert(path.clone(), public_members);
                }

                let members = self.checked_modules.get(&path).unwrap().clone();
                self.env.define(
                    namespace_name.clone(),
                    TypeInfo::Namespace(namespace_name, members),
                    Visibility::Private, // The import itself is private to this module unless specified otherwise
                );

                Ok(())
            }
            Stmt::Directive(_) => Ok(()),
            Stmt::Class(name, body, visibility) => {
                let name_str = name.lexeme.clone();
                let mut members = HashMap::new();
                
                // First pass: register method signatures and fields
                for s in body {
                    match s {
                        Stmt::Function(decl) => {
                            let ret_type = decl.return_type.clone();
                            let mut param_types = Vec::new();
                            for (_, t) in &decl.params {
                                param_types.push(t.clone());
                            }
                            members.insert(decl.name.lexeme.clone(), (TypeInfo::Function(ret_type, param_types, decl.is_static, false), decl.visibility.clone()));
                        }
                        Stmt::Let(name, type_ann, _, visibility) => {
                            let field_type = type_ann.clone().unwrap_or(Type::Dynamic);
                            members.insert(name.lexeme.clone(), (TypeInfo::Variable(field_type), visibility.clone()));
                        }
                        _ => {}
                    }
                }
                
                self.env.define(name_str.clone(), TypeInfo::Class(name_str.clone(), members), visibility.clone());
                
                // Second pass: check method bodies
                let old_class = self.current_class.take();
                self.current_class = Some(name_str);
                
                for s in body {
                    self.check_stmt(s)?;
                }
                
                self.current_class = old_class;
                Ok(())
            }
            Stmt::Struct(name, body, visibility) => {
                self.check_stmt(&Stmt::Class(name.clone(), body.clone(), visibility.clone()))
            }
            Stmt::Enum(name, variants, visibility) => {
                let mut members = HashMap::new();
                for variant in variants {
                    let mut param_types = Vec::new();
                    for t in &variant.types {
                        param_types.push(t.clone());
                    }
                    if param_types.is_empty() {
                        members.insert(variant.name.lexeme.clone(), (TypeInfo::Variable(Type::Dynamic), Visibility::Public));
                    } else {
                        members.insert(variant.name.lexeme.clone(), (TypeInfo::Function(Type::Dynamic, param_types, true, false), Visibility::Public));
                    }
                }
                self.env.define(name.lexeme.clone(), TypeInfo::Namespace(name.lexeme.clone(), members), visibility.clone());
                Ok(())
            }
            Stmt::Try(try_body, _, exception_name, catch_body) => {
                self.check_stmt(try_body)?;
                
                let mut previous_env = TypeEnvironment::new();
                std::mem::swap(&mut self.env, &mut previous_env);
                self.env.enclosing = Some(Box::new(previous_env));
                
                if let Some(name) = exception_name {
                    self.env.define(name.lexeme.clone(), TypeInfo::Variable(Type::Dynamic), Visibility::Public);
                }
                
                self.check_stmt(catch_body)?;
                
                let mut original_env = *self.env.enclosing.take().unwrap();
                std::mem::swap(&mut self.env, &mut original_env);
                Ok(())
            }
        }
    }

    pub fn check_expr(&mut self, expr: &Expr) -> Result<Type, LavinaError> {
        match expr {
            Expr::Literal(lit) => match lit {
                Literal::Int(_) => Ok(Type::Int),
                Literal::Float(_) => Ok(Type::Float),
                Literal::String(_) => Ok(Type::String),
                Literal::Bool(_) => Ok(Type::Bool),
                Literal::Null => Ok(Type::Null),
            },
            Expr::Variable(name) => {
                match self.env.get(&name.lexeme) {
                    Some(TypeInfo::Variable(t)) => Ok(t),
                    Some(TypeInfo::Function(ret, params, _, _)) => Ok(Type::Function(Box::new(ret), params)),
                    Some(TypeInfo::Namespace(_, _)) => Ok(Type::Dynamic),
                    Some(TypeInfo::Class(_, _)) => Ok(Type::Dynamic),
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
            Expr::Call(callee, paren, args) => {
                let callee_type = self.check_expr(callee)?;
                let mut arg_types = Vec::new();
                for arg in args {
                    arg_types.push(self.check_expr(arg)?);
                }
                
                if let Expr::Variable(name) = &**callee {
                    if let Some(TypeInfo::Class(class_name, members)) = self.env.get(&name.lexeme) {
                        // Check __init__ method if exists
                        if let Some((TypeInfo::Function(_, params, _, _), _)) = members.get("__init__") {
                            if params.len() != arg_types.len() {
                                return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Class '{}' constructor expected {} arguments, but got {}.", class_name, params.len(), arg_types.len()), paren.line, paren.column).with_context(&self.source));
                            }
                            for (i, (expected, actual)) in params.iter().zip(arg_types.iter()).enumerate() {
                                if !self.is_assignable(expected, actual) {
                                    return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Type mismatch for argument {} of '{}' constructor: expected {:?}, but got {:?}.", i + 1, class_name, expected, actual), paren.line, paren.column).with_context(&self.source));
                                }
                            }
                        } else if !arg_types.is_empty() {
                            return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Class '{}' has no constructor, expected 0 arguments.", class_name), paren.line, paren.column).with_context(&self.source));
                        }
                        return Ok(Type::Custom(class_name));
                    }
                }

                match callee_type {
                    Type::Function(ret, params) => {
                        // Check if it's a known function from environment to see if it's variadic
                        let mut is_variadic = false;
                        if let Expr::Variable(name) = &**callee {
                            if let Some(TypeInfo::Function(_, _, _, variadic)) = self.env.get(&name.lexeme) {
                                is_variadic = variadic;
                            }
                        } else if let Expr::Get(obj, name) = &**callee {
                             let obj_type = self.check_expr(obj)?;
                             let cn = match obj_type {
                                Type::Custom(n) => Some(n),
                                Type::String => Some("String".to_string()),
                                Type::Int => Some("Int".to_string()),
                                Type::Float => Some("Float".to_string()),
                                Type::Bool => Some("Bool".to_string()),
                                Type::Array(_) => Some("Vector".to_string()),
                                Type::Dict(_, _) => Some("HashMap".to_string()),
                                _ => None,
                             };
                             if let Some(c) = cn {
                                 if let Some(TypeInfo::Class(_, members)) = self.env.get(&c) {
                                     if let Some((TypeInfo::Function(_, _, _, variadic), _)) = members.get(&name.lexeme) {
                                         is_variadic = *variadic;
                                     }
                                 }
                             }
                        }

                        if is_variadic {
                            if arg_types.len() < params.len() {
                                return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Variadic function expected at least {} arguments, but got {}.", params.len(), arg_types.len()), paren.line, paren.column).with_context(&self.source));
                            }
                            // Check fixed arguments
                            for (i, (expected, actual)) in params.iter().zip(arg_types.iter()).enumerate() {
                                if !self.is_assignable(expected, actual) {
                                    return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Type mismatch for argument {}: expected {:?}, but got {:?}.", i + 1, expected, actual), paren.line, paren.column).with_context(&self.source));
                                }
                            }
                        } else {
                            if params.len() != arg_types.len() {
                                return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Expected {} arguments, but got {}.", params.len(), arg_types.len()), paren.line, paren.column).with_context(&self.source));
                            }
                            for (i, (expected, actual)) in params.iter().zip(arg_types.iter()).enumerate() {
                                if !self.is_assignable(expected, actual) {
                                    return Err(LavinaError::new(ErrorPhase::TypeChecker, format!("Type mismatch for argument {}: expected {:?}, but got {:?}.", i + 1, expected, actual), paren.line, paren.column).with_context(&self.source));
                                }
                            }
                        }
                        Ok(*ret)
                    }
                    Type::Dynamic => Ok(Type::Dynamic),
                    _ => Ok(Type::Dynamic),
                }
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
            Expr::Unary(_, expr) => {
                let _ = self.check_expr(expr)?;
                Ok(Type::Dynamic) // Simplified
            }
            Expr::Logical(left, _, right) => {
                let _ = self.check_expr(left)?;
                let _ = self.check_expr(right)?;
                Ok(Type::Bool)
            }
            Expr::Get(obj, name) => {
                // If the object is a variable, check if it's a namespace
                if let Expr::Variable(v) = &**obj {
                    if let Some(TypeInfo::Namespace(ns_name, _)) = self.env.get(&v.lexeme) {
                        return Err(LavinaError::new(
                            ErrorPhase::TypeChecker,
                            format!("'{}' is a namespace. Use '::' to access its members (e.g., '{}::{}').", ns_name, ns_name, name.lexeme),
                            name.line,
                            name.column,
                        ).with_context(&self.source));
                    }
                }

                let obj_type = self.check_expr(obj)?;

                let class_name = match obj_type {
                    Type::Custom(name) => Some(name),
                    Type::String => Some("String".to_string()),
                    Type::Int => Some("Int".to_string()),
                    Type::Float => Some("Float".to_string()),
                    Type::Bool => Some("Bool".to_string()),
                    Type::Array(_) => Some("Vector".to_string()),
                    Type::Dict(_, _) => Some("HashMap".to_string()),
                    Type::Dynamic => return Ok(Type::Dynamic),
                    _ => None,
                };

                if let Some(cn) = class_name {
                    if let Some(TypeInfo::Class(_, members)) = self.env.get(&cn) {
                        if let Some((info, _)) = members.get(&name.lexeme) {
                            if let TypeInfo::Function(_, _, is_static, _) = info {
                                if *is_static {
                                    return Err(LavinaError::new(
                                        ErrorPhase::TypeChecker,
                                        format!("Member '{}' of class '{}' is static. Use '::' to access it.", name.lexeme, cn),
                                        name.line,
                                        name.column,
                                    ).with_context(&self.source));
                                }
                            }
                            return Ok(match info {
                                TypeInfo::Variable(t) => t.clone(),
                                TypeInfo::Function(ret, params, _, _) => Type::Function(Box::new(ret.clone()), params.clone()),
                                TypeInfo::Namespace(_, _) => Type::Dynamic,
                                TypeInfo::Class(_, _) => Type::Dynamic,
                            });
                        }
                    }
                }
                
                Ok(Type::Dynamic)
            }
            Expr::StaticGet(obj, name) => {
                let _ = self.check_expr(obj)?;
                
                // Static Namespace Access
                if let Expr::Variable(obj_name) = &**obj {
                    if let Some(TypeInfo::Namespace(ns_name, members)) = self.env.get(&obj_name.lexeme) {
                        if let Some((info, visibility)) = members.get(&name.lexeme) {
                            if visibility == &Visibility::Private {
                                return Err(LavinaError::new(
                                    ErrorPhase::TypeChecker,
                                    format!("Cannot access private member '{}' of namespace '{}'", name.lexeme, ns_name),
                                    name.line,
                                    name.column,
                                ).with_context(&self.source));
                            }
                            return Ok(match info {
                                TypeInfo::Variable(t) => t.clone(),
                                TypeInfo::Function(ret, params, _, _) => Type::Function(Box::new(ret.clone()), params.clone()),
                                TypeInfo::Namespace(_, _) => Type::Dynamic,
                                TypeInfo::Class(_, _) => Type::Dynamic,
                            });
                        } else {
                            return Err(LavinaError::new(
                                ErrorPhase::TypeChecker,
                                format!("Namespace '{}' has no member '{}'", ns_name, name.lexeme),
                                name.line,
                                name.column,
                            ).with_context(&self.source));
                        }
                    }
                    
                    if let Some(TypeInfo::Class(class_name, members)) = self.env.get(&obj_name.lexeme) {
                        if let Some((info, _)) = members.get(&name.lexeme) {
                            match info {
                                TypeInfo::Function(_, _, is_static, _) => {
                                    if !is_static {
                                        return Err(LavinaError::new(
                                            ErrorPhase::TypeChecker,
                                            format!("Member '{}' of class '{}' is not static. Use '.' to access it from an instance.", name.lexeme, class_name),
                                            name.line,
                                            name.column,
                                        ).with_context(&self.source));
                                    }
                                }
                                _ => {}
                            }
                            return Ok(match info {
                                TypeInfo::Variable(t) => t.clone(),
                                TypeInfo::Function(ret, params, _, _) => Type::Function(Box::new(ret.clone()), params.clone()),
                                TypeInfo::Namespace(_, _) => Type::Dynamic,
                                TypeInfo::Class(_, _) => Type::Dynamic,
                            });
                        } else {
                            return Err(LavinaError::new(
                                ErrorPhase::TypeChecker,
                                format!("Class '{}' has no static member '{}'", class_name, name.lexeme),
                                name.line,
                                name.column,
                            ).with_context(&self.source));
                        }
                    }
                }
                
                Ok(Type::Dynamic)
            }
            Expr::Set(obj, _, value) => {
                let _ = self.check_expr(obj)?;
                let val_type = self.check_expr(value)?;
                Ok(val_type)
            }
            Expr::This(name) => {
                if let Some(TypeInfo::Variable(t)) = self.env.get(&name.lexeme) {
                    Ok(t.clone())
                } else if let Some(class_name) = &self.current_class {
                    Ok(Type::Custom(class_name.clone()))
                } else {
                    Ok(Type::Dynamic)
                }
            }
            Expr::Cast(expr, target_type) => {
                self.check_expr(expr)?;
                Ok(target_type.clone())
            }
            Expr::Throw(expr) => {
                self.check_expr(expr)?;
                Ok(Type::Dynamic)
            }
        }
    }

    fn is_assignable(&self, target: &Type, value: &Type) -> bool {
        if target == &Type::Auto || target == &Type::Dynamic { return true; }
        if target == value { return true; }
        
        match (target, value) {
            (Type::Array(t1), Type::Array(t2)) => self.is_assignable(t1, t2),
            (Type::Dict(k1, v1), Type::Dict(k2, v2)) => self.is_assignable(k1, k2) && self.is_assignable(v1, v2),
            (Type::Nullable(_), Type::Null) => true,
            (Type::Nullable(t1), t2) => self.is_assignable(t1, t2),
            // Handle intrinsic types as classes
            (Type::String, Type::Custom(name)) if name == "String" => true,
            (Type::Custom(name), Type::String) if name == "String" => true,
            (Type::Int, Type::Custom(name)) if name == "Int" => true,
            (Type::Custom(name), Type::Int) if name == "Int" => true,
            (Type::Float, Type::Custom(name)) if name == "Float" => true,
            (Type::Custom(name), Type::Float) if name == "Float" => true,
            (Type::Bool, Type::Custom(name)) if name == "Bool" => true,
            (Type::Custom(name), Type::Bool) if name == "Bool" => true,
            (Type::Array(_), Type::Custom(name)) if name == "Vector" => true,
            (Type::Custom(name), Type::Array(_)) if name == "Vector" => true,
            (Type::Dict(_, _), Type::Custom(name)) if name == "HashMap" => true,
            (Type::Custom(name), Type::Dict(_, _)) if name == "HashMap" => true,
            _ => false,
        }
    }
}
