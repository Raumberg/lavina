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
            Stmt::Let(lname, ty, init, _vis) => {
                let cpp_type = match ty {
                    Some(t) => self.emit_type(t),
                    None => "auto".to_string(),
                };
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
            _ => self.emit_stmt(stmt),
        }
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
