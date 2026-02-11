use crate::parser::ast::*;
use super::CppCodegen;

impl CppCodegen {
    pub(super) fn emit_stmt(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Expression(expr) => {
                let e = self.emit_expr(expr);
                self.output.push_str(&format!("{}{};\n", self.indent(), e));
            }
            Stmt::Let(name, ty, init, _vis) => {
                let cpp_type = match ty {
                    Some(t) => self.emit_type(t),
                    None => "auto".to_string(),
                };
                // Track dynamic variables
                let is_dynamic = Self::type_contains_dynamic(ty)
                    || (ty.is_none() && init.as_ref().map_or(false, |e| self.is_dynamic_expr(e)));
                if is_dynamic {
                    self.dynamic_vars.insert(name.lexeme.clone());
                }
                let init_str = match init {
                    Some(expr) => format!(" = {}", self.emit_expr(expr)),
                    None => self.default_init(&cpp_type),
                };
                self.output.push_str(&format!(
                    "{}{} {}{};\n",
                    self.indent(),
                    cpp_type,
                    name.lexeme,
                    init_str
                ));
            }
            Stmt::If(cond, then_branch, else_branch) => {
                let c = self.emit_expr(cond);
                self.output.push_str(&format!("{}if ({}) ", self.indent(), c));
                self.emit_block_or_stmt(then_branch);
                if let Some(else_b) = else_branch {
                    self.output.push_str(&format!("{} else ", self.indent()));
                    self.emit_block_or_stmt(else_b);
                }
            }
            Stmt::While(cond, body) => {
                let c = self.emit_expr(cond);
                self.output.push_str(&format!("{}while ({}) ", self.indent(), c));
                self.emit_block_or_stmt(body);
            }
            Stmt::For(var, iterable, body) => {
                // If iterating over a dynamic variable, the loop var is also dynamic
                let iter_is_dynamic = match iterable {
                    Expr::Variable(tok) => self.dynamic_vars.contains(&tok.lexeme),
                    _ => self.is_dynamic_expr(iterable),
                };
                if iter_is_dynamic {
                    self.dynamic_vars.insert(var.lexeme.clone());
                }
                let iter = self.emit_expr(iterable);
                self.output.push_str(&format!(
                    "{}for (auto& {} : {}) ",
                    self.indent(),
                    var.lexeme,
                    iter
                ));
                self.emit_block_or_stmt(body);
            }
            Stmt::Return(_token, expr) => {
                match expr {
                    Some(e) => {
                        let val = self.emit_expr(e);
                        self.output.push_str(&format!("{}return {};\n", self.indent(), val));
                    }
                    None => {
                        self.output.push_str(&format!("{}return;\n", self.indent()));
                    }
                }
            }
            Stmt::Function(decl) => {
                self.emit_function(decl);
            }
            Stmt::Block(stmts) => {
                self.output.push_str(&format!("{}{{\n", self.indent()));
                self.indent_level += 1;
                for s in stmts {
                    self.emit_stmt(s);
                }
                self.indent_level -= 1;
                self.output.push_str(&format!("{}}}\n", self.indent()));
            }
            Stmt::Try(try_body, _catch_token, exception_name, catch_body) => {
                self.output.push_str(&format!("{}try ", self.indent()));
                self.emit_block_or_stmt(try_body);
                let exc_name = match exception_name {
                    Some(t) => t.lexeme.clone(),
                    None => "e".to_string(),
                };
                self.output.push_str(&format!(
                    "{} catch (const std::exception& {}) ",
                    self.indent(),
                    exc_name
                ));
                self.emit_block_or_stmt(catch_body);
            }
            Stmt::Class(name, body, _vis) | Stmt::Struct(name, body, _vis) => {
                self.emit_class(name, body);
            }
            Stmt::Enum(name, variants, _vis) => {
                self.emit_enum(name, variants);
            }
            Stmt::Const(name, ty, init, _vis) => {
                let cpp_type = self.emit_type(ty);
                let val = self.emit_expr(init);
                self.output.push_str(&format!(
                    "{}const {} {} = {};\n",
                    self.indent(),
                    cpp_type,
                    name.lexeme,
                    val
                ));
            }
            Stmt::Match(expr, arms) => {
                self.emit_match(expr, arms);
            }
            Stmt::Directive(_) | Stmt::Namespace(_, _, _) | Stmt::Import(_, _) => {
                self.output.push_str(&format!(
                    "{}// TODO: unsupported statement\n",
                    self.indent()
                ));
            }
        }
    }

    pub(super) fn emit_block_or_stmt(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Block(stmts) => {
                self.output.push_str("{\n");
                self.indent_level += 1;
                for s in stmts {
                    self.emit_stmt(s);
                }
                self.indent_level -= 1;
                self.output.push_str(&format!("{}}}\n", self.indent()));
            }
            _ => {
                self.output.push_str("{\n");
                self.indent_level += 1;
                self.emit_stmt(stmt);
                self.indent_level -= 1;
                self.output.push_str(&format!("{}}}\n", self.indent()));
            }
        }
    }

    pub(super) fn emit_function(&mut self, decl: &FunctionDecl) {
        let ret_type = self.emit_type(&decl.return_type);
        let params: Vec<String> = decl
            .params
            .iter()
            .map(|(name, ty)| format!("{} {}", self.emit_type(ty), name.lexeme))
            .collect();
        // Track dynamic parameters
        for (name, ty) in &decl.params {
            if matches!(ty, Type::Dynamic) {
                self.dynamic_vars.insert(name.lexeme.clone());
            }
        }
        self.output.push_str(&format!(
            "{}{} {}({}) {{\n",
            self.indent(),
            ret_type,
            decl.name.lexeme,
            params.join(", ")
        ));
        self.indent_level += 1;
        for stmt in &decl.body {
            self.emit_stmt(stmt);
        }
        self.indent_level -= 1;
        self.output.push_str(&format!("{}}}\n\n", self.indent()));
    }

    pub(super) fn emit_method_stmt(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Expression(expr) => {
                let e = self.emit_method_expr(expr);
                self.output.push_str(&format!("{}{};\n", self.indent(), e));
            }
            Stmt::Return(_, expr) => {
                match expr {
                    Some(e) => {
                        let val = self.emit_method_expr(e);
                        self.output.push_str(&format!("{}return {};\n", self.indent(), val));
                    }
                    None => {
                        self.output.push_str(&format!("{}return;\n", self.indent()));
                    }
                }
            }
            Stmt::If(cond, then_branch, else_branch) => {
                let c = self.emit_method_expr(cond);
                self.output.push_str(&format!("{}if ({}) ", self.indent(), c));
                self.emit_method_block_or_stmt(then_branch);
                if let Some(else_b) = else_branch {
                    self.output.push_str(&format!("{} else ", self.indent()));
                    self.emit_method_block_or_stmt(else_b);
                }
            }
            Stmt::While(cond, body) => {
                let c = self.emit_method_expr(cond);
                self.output.push_str(&format!("{}while ({}) ", self.indent(), c));
                self.emit_method_block_or_stmt(body);
            }
            Stmt::For(var, iterable, body) => {
                let iter_is_dynamic = match iterable {
                    Expr::Variable(tok) => self.dynamic_vars.contains(&tok.lexeme),
                    _ => self.is_dynamic_expr(iterable),
                };
                if iter_is_dynamic {
                    self.dynamic_vars.insert(var.lexeme.clone());
                }
                let iter = self.emit_method_expr(iterable);
                self.output.push_str(&format!(
                    "{}for (auto& {} : {}) ",
                    self.indent(),
                    var.lexeme,
                    iter
                ));
                self.emit_method_block_or_stmt(body);
            }
            Stmt::Let(lname, ty, init, _vis) => {
                let cpp_type = match ty {
                    Some(t) => self.emit_type(t),
                    None => "auto".to_string(),
                };
                let is_dynamic = Self::type_contains_dynamic(ty)
                    || (ty.is_none() && init.as_ref().map_or(false, |e| self.is_dynamic_expr(e)));
                if is_dynamic {
                    self.dynamic_vars.insert(lname.lexeme.clone());
                }
                let init_str = match init {
                    Some(expr) => format!(" = {}", self.emit_method_expr(expr)),
                    None => self.default_init(&cpp_type),
                };
                self.output.push_str(&format!(
                    "{}{} {}{};\n",
                    self.indent(),
                    cpp_type,
                    lname.lexeme,
                    init_str
                ));
            }
            Stmt::Block(stmts) => {
                self.output.push_str(&format!("{}{{\n", self.indent()));
                self.indent_level += 1;
                for s in stmts {
                    self.emit_method_stmt(s);
                }
                self.indent_level -= 1;
                self.output.push_str(&format!("{}}}\n", self.indent()));
            }
            Stmt::Try(try_body, _catch_token, exception_name, catch_body) => {
                self.output.push_str(&format!("{}try ", self.indent()));
                self.emit_method_block_or_stmt(try_body);
                let exc_name = match exception_name {
                    Some(t) => t.lexeme.clone(),
                    None => "e".to_string(),
                };
                self.output.push_str(&format!(
                    "{} catch (const std::exception& {}) ",
                    self.indent(),
                    exc_name
                ));
                self.emit_method_block_or_stmt(catch_body);
            }
            _ => self.emit_stmt(stmt),
        }
    }

    pub(super) fn emit_match(&mut self, expr: &Expr, arms: &[MatchArm]) {
        let match_val = self.emit_expr(expr);
        let temp = format!("_match_{}", self.temp_counter);
        self.temp_counter += 1;

        self.output.push_str(&format!("{}{{\n", self.indent()));
        self.indent_level += 1;
        self.output.push_str(&format!("{}auto& {} = {};\n", self.indent(), temp, match_val));

        let mut first = true;
        for arm in arms {
            match &arm.pattern {
                Pattern::Variant(variant_name, bindings) => {
                    let keyword = if first { "if" } else { "else if" };
                    first = false;
                    self.output.push_str(&format!(
                        "{}{} ({}._tag == \"{}\") {{\n",
                        self.indent(), keyword, temp, variant_name.lexeme
                    ));
                    self.indent_level += 1;

                    // Destructure bindings by finding which enum owns this variant
                    if !bindings.is_empty() {
                        let ename = self.find_enum_for_variant(&variant_name.lexeme);
                        if let Some(ref ename) = ename {
                            self.output.push_str(&format!(
                                "{}auto& _v = std::get<{}::{}>({}._data);\n",
                                self.indent(), ename, variant_name.lexeme, temp
                            ));
                            let fields: Vec<(String, Type)> = self.known_enums.get(ename.as_str())
                                .and_then(|variants| variants.iter().find(|v| v.name.lexeme == variant_name.lexeme))
                                .map(|v| v.fields.iter().map(|(f, t)| (f.lexeme.clone(), t.clone())).collect())
                                .unwrap_or_default();

                            for (i, binding) in bindings.iter().enumerate() {
                                let (field_name, field_type) = fields.get(i).cloned()
                                    .unwrap_or_else(|| (format!("_{}", i), Type::Dynamic));
                                let is_self_ref = matches!(&field_type, Type::Custom(n) if n == ename);
                                if is_self_ref {
                                    self.output.push_str(&format!(
                                        "{}auto& {} = *_v.{};\n",
                                        self.indent(), binding.lexeme, field_name
                                    ));
                                } else {
                                    self.output.push_str(&format!(
                                        "{}auto& {} = _v.{};\n",
                                        self.indent(), binding.lexeme, field_name
                                    ));
                                }
                            }
                        }
                    }

                    for s in &arm.body {
                        self.emit_stmt(s);
                    }
                    self.indent_level -= 1;
                    self.output.push_str(&format!("{}}}\n", self.indent()));
                }
                Pattern::Wildcard => {
                    if first {
                        self.output.push_str(&format!("{}{{\n", self.indent()));
                    } else {
                        self.output.push_str(&format!("{}else {{\n", self.indent()));
                    }
                    self.indent_level += 1;
                    for s in &arm.body {
                        self.emit_stmt(s);
                    }
                    self.indent_level -= 1;
                    self.output.push_str(&format!("{}}}\n", self.indent()));
                }
            }
        }

        self.indent_level -= 1;
        self.output.push_str(&format!("{}}}\n", self.indent()));
    }

    /// Find which known enum contains a variant with the given name
    fn find_enum_for_variant(&self, variant_name: &str) -> Option<String> {
        for (enum_name, variants) in &self.known_enums {
            if variants.iter().any(|v| v.name.lexeme == variant_name) {
                return Some(enum_name.clone());
            }
        }
        None
    }

    pub(super) fn emit_method_block_or_stmt(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Block(stmts) => {
                self.output.push_str("{\n");
                self.indent_level += 1;
                for s in stmts {
                    self.emit_method_stmt(s);
                }
                self.indent_level -= 1;
                self.output.push_str(&format!("{}}}\n", self.indent()));
            }
            _ => {
                self.output.push_str("{\n");
                self.indent_level += 1;
                self.emit_method_stmt(stmt);
                self.indent_level -= 1;
                self.output.push_str(&format!("{}}}\n", self.indent()));
            }
        }
    }
}
