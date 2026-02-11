use crate::parser::ast::*;
use std::collections::HashMap;
use super::{CppCodegen, FieldInfo, ClassInfo};

impl CppCodegen {
    pub(super) fn emit_class(&mut self, name: &crate::lexer::token::Token, body: &[Stmt]) {
        let mut init_decl: Option<&FunctionDecl> = None;
        let mut methods: Vec<&FunctionDecl> = Vec::new();
        let mut let_fields: Vec<FieldInfo> = Vec::new();

        for stmt in body {
            match stmt {
                Stmt::Function(decl) => {
                    if decl.name.lexeme == "__init__" {
                        init_decl = Some(decl);
                    } else {
                        methods.push(decl);
                    }
                }
                Stmt::Let(fname, ty, _init, _vis) => {
                    let cpp_type = match ty {
                        Some(t) => self.emit_type(t),
                        None => "std::any".to_string(),
                    };
                    let_fields.push(FieldInfo {
                        name: fname.lexeme.clone(),
                        cpp_type,
                    });
                }
                _ => {}
            }
        }

        // Discover implicit fields from __init__ body (this.field = expr)
        let mut init_fields: Vec<FieldInfo> = Vec::new();
        if let Some(init) = init_decl {
            let param_types: HashMap<String, String> = init
                .params
                .iter()
                .map(|(tok, ty)| (tok.lexeme.clone(), self.emit_type(ty)))
                .collect();
            init_fields = self.discover_fields(&init.body, &param_types);
        }

        // Deduplicate: Let fields override discovered fields
        let let_field_names: Vec<String> = let_fields.iter().map(|f| f.name.clone()).collect();
        let all_fields: Vec<&FieldInfo> = init_fields
            .iter()
            .filter(|f| !let_field_names.contains(&f.name))
            .chain(let_fields.iter())
            .collect();

        // Emit struct
        self.output.push_str(&format!("{}struct {} {{\n", self.indent(), name.lexeme));
        self.indent_level += 1;

        for field in &all_fields {
            self.output.push_str(&format!(
                "{}{} {};\n",
                self.indent(),
                field.cpp_type,
                field.name
            ));
        }
        if !all_fields.is_empty() {
            self.output.push('\n');
        }

        // Constructor from __init__
        if let Some(init) = init_decl {
            let params: Vec<String> = init
                .params
                .iter()
                .map(|(pname, ty)| format!("{} {}", self.emit_type(ty), pname.lexeme))
                .collect();

            // Build initializer list from this.field = param assignments
            let mut init_list: Vec<String> = Vec::new();
            let mut body_stmts: Vec<&Stmt> = Vec::new();
            for stmt in &init.body {
                if let Stmt::Expression(Expr::Set(obj, prop, value)) = stmt {
                    if matches!(obj.as_ref(), Expr::This(_)) {
                        if let Expr::Variable(tok) = value.as_ref() {
                            // Simple this.field = param → use initializer list
                            init_list.push(format!("{}({})", prop.lexeme, tok.lexeme));
                            continue;
                        }
                    }
                }
                body_stmts.push(stmt);
            }

            if init_list.is_empty() {
                self.output.push_str(&format!(
                    "{}{}({}) {{\n",
                    self.indent(),
                    name.lexeme,
                    params.join(", ")
                ));
            } else {
                self.output.push_str(&format!(
                    "{}{}({})\n{}    : {} {{\n",
                    self.indent(),
                    name.lexeme,
                    params.join(", "),
                    self.indent(),
                    init_list.join(", ")
                ));
            }
            self.indent_level += 1;
            for stmt in &body_stmts {
                self.emit_method_stmt(stmt);
            }
            self.indent_level -= 1;
            self.output.push_str(&format!("{}}}\n\n", self.indent()));
        }

        // Methods
        for method in &methods {
            let ret_type = self.emit_type(&method.return_type);
            let params: Vec<String> = method
                .params
                .iter()
                .map(|(pname, ty)| format!("{} {}", self.emit_type(ty), pname.lexeme))
                .collect();
            // Track dynamic parameters
            for (pname, ty) in &method.params {
                if matches!(ty, Type::Dynamic) {
                    self.dynamic_vars.insert(pname.lexeme.clone());
                }
            }
            self.output.push_str(&format!(
                "{}{} {}({}) {{\n",
                self.indent(),
                ret_type,
                method.name.lexeme,
                params.join(", ")
            ));
            self.indent_level += 1;
            for stmt in &method.body {
                self.emit_method_stmt(stmt);
            }
            self.indent_level -= 1;
            self.output.push_str(&format!("{}}}\n\n", self.indent()));
        }

        self.indent_level -= 1;
        self.output.push_str(&format!("{}}};\n\n", self.indent()));

        // Register class and emit lv_get_field / lv_set_field overloads
        let class_name = name.lexeme.clone();
        let class_fields: Vec<FieldInfo> = all_fields.iter().map(|f| FieldInfo {
            name: f.name.clone(),
            cpp_type: f.cpp_type.clone(),
        }).collect();

        // Emit lv_get_field overload
        self.output.push_str(&format!("std::any lv_get_field(const {}& obj, const std::string& name) {{\n", class_name));
        for field in &class_fields {
            self.output.push_str(&format!("    if (name == \"{}\") return std::any(obj.{});\n", field.name, field.name));
        }
        self.output.push_str(&format!("    throw std::runtime_error(\"No field '\" + name + \"' on {}\");\n", class_name));
        self.output.push_str("}\n\n");

        // Emit lv_set_field overload
        self.output.push_str(&format!("void lv_set_field({}& obj, const std::string& name, std::any value) {{\n", class_name));
        for field in &class_fields {
            let cast = match field.cpp_type.as_str() {
                "std::string" => format!("obj.{} = std::any_cast<std::string>(value);", field.name),
                "int64_t" => format!("obj.{} = std::any_cast<int64_t>(value);", field.name),
                "double" => format!("obj.{} = std::any_cast<double>(value);", field.name),
                "bool" => format!("obj.{} = std::any_cast<bool>(value);", field.name),
                "std::any" => format!("obj.{} = value;", field.name),
                _ => format!("obj.{} = std::any_cast<{}>(value);", field.name, field.cpp_type),
            };
            self.output.push_str(&format!("    if (name == \"{}\") {{ {} return; }}\n", field.name, cast));
        }
        self.output.push_str(&format!("    throw std::runtime_error(\"No field '\" + name + \"' on {}\");\n", class_name));
        self.output.push_str("}\n\n");

        self.known_classes.push(ClassInfo {
            name: class_name,
            fields: class_fields,
        });
    }

    fn discover_fields(&self, stmts: &[Stmt], param_types: &HashMap<String, String>) -> Vec<FieldInfo> {
        let mut fields: Vec<FieldInfo> = Vec::new();
        let mut seen: Vec<String> = Vec::new();

        for stmt in stmts {
            if let Stmt::Expression(Expr::Set(obj, prop, value)) = stmt {
                if matches!(obj.as_ref(), Expr::This(_)) && !seen.contains(&prop.lexeme) {
                    let cpp_type = self.infer_expr_type(value, param_types);
                    seen.push(prop.lexeme.clone());
                    fields.push(FieldInfo {
                        name: prop.lexeme.clone(),
                        cpp_type,
                    });
                }
            }
        }
        fields
    }

    fn infer_expr_type(&self, expr: &Expr, param_types: &HashMap<String, String>) -> String {
        match expr {
            Expr::Literal(Literal::Int(_)) => "int64_t".to_string(),
            Expr::Literal(Literal::Float(_)) => "double".to_string(),
            Expr::Literal(Literal::String(_)) => "std::string".to_string(),
            Expr::Literal(Literal::Bool(_)) => "bool".to_string(),
            Expr::Variable(tok) => {
                if let Some(t) = param_types.get(&tok.lexeme) {
                    if t == "auto" { "std::any".to_string() } else { t.clone() }
                } else {
                    "std::any".to_string()
                }
            }
            Expr::Vector(_) => "std::vector<std::any>".to_string(),
            _ => "std::any".to_string(),
        }
    }
}
