mod expr;
mod stmt;
mod class;
mod enums;
mod types;

use std::collections::HashSet;
use crate::parser::ast::*;
use crate::error::LavinaError;

/// The Lavina C++ runtime header, embedded at compile time.
pub const RUNTIME_HEADER: &str = include_str!("../../../runtime/lavina.h");

pub struct CppCodegen {
    pub(super) output: String,
    pub(super) indent_level: usize,
    pub(super) temp_counter: usize,
    pub(super) declarations: String,
    pub(super) known_classes: Vec<ClassInfo>,
    /// Variable names known to hold std::any (declared as `dynamic` or from for-loops over dynamic vectors)
    pub(super) dynamic_vars: HashSet<String>,
    pub(super) has_main: bool,
}

pub(super) struct ClassInfo {
    pub name: String,
    pub fields: Vec<FieldInfo>,
}

pub(super) struct FieldInfo {
    pub name: String,
    pub cpp_type: String,
}

impl CppCodegen {
    pub fn new() -> Self {
        Self {
            output: String::new(),
            indent_level: 0,
            temp_counter: 0,
            declarations: String::new(),
            known_classes: Vec::new(),
            dynamic_vars: HashSet::new(),
            has_main: false,
        }
    }

    pub fn generate(mut self, stmts: &[Stmt]) -> Result<String, LavinaError> {
        self.declarations.push_str("#include \"lavina.h\"\n\n");

        // Two-pass: first collect all class definitions (to know all types),
        // then emit everything in order with dispatchers inserted at the right point.

        // First pass: find the index of the last class/struct definition
        let mut last_class_idx = None;
        for (i, stmt) in stmts.iter().enumerate() {
            match stmt {
                Stmt::Class(_, _, _) | Stmt::Struct(_, _, _) => {
                    last_class_idx = Some(i);
                }
                _ => {}
            }
        }

        for (i, stmt) in stmts.iter().enumerate() {
            if let Stmt::Function(decl) = stmt {
                if decl.name.lexeme == "main" {
                    self.has_main = true;
                    // Emit main as int main(int argc, char* argv[])
                    self.output.push_str("int main(int argc, char* argv[]) {\n");
                    self.output.push_str("    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);\n");
                    self.indent_level = 1;
                    for s in &decl.body {
                        self.emit_stmt(s);
                    }
                    self.indent_level = 0;
                    self.output.push_str("    return 0;\n}\n");
                    self.declarations.push_str(&self.output);
                    self.output.clear();
                    continue;
                }
            }

            self.emit_stmt(stmt);
            self.declarations.push_str(&self.output);
            self.output.clear();

            // After the last class, emit dynamic dispatchers
            if Some(i) == last_class_idx && !self.known_classes.is_empty() {
                self.emit_dynamic_dispatchers();
                self.declarations.push_str(&self.output);
                self.output.clear();
            }
        }

        if !self.has_main {
            return Err(LavinaError::new(
                crate::error::ErrorPhase::Compiler,
                "No 'main()' function defined.".to_string(),
                0, 0,
            ));
        }

        Ok(self.declarations)
    }

    fn emit_dynamic_dispatchers(&mut self) {
        // Generate lv_get_field(const std::any& obj, name) dispatcher
        self.output.push_str("std::any lv_get_field(const std::any& obj, const std::string& name) {\n");
        for class in &self.known_classes {
            self.output.push_str(&format!(
                "    if (auto* p = std::any_cast<{}>(&obj)) return lv_get_field(*p, name);\n",
                class.name
            ));
        }
        self.output.push_str("    throw std::runtime_error(\"lv_get_field: unknown type for field: \" + name);\n");
        self.output.push_str("}\n\n");

        // Generate lv_set_field(std::any& obj, name, value) dispatcher
        self.output.push_str("void lv_set_field(std::any& obj, const std::string& name, std::any value) {\n");
        for class in &self.known_classes {
            self.output.push_str(&format!(
                "    if (auto* p = std::any_cast<{}>(&obj)) {{ lv_set_field(*p, name, std::move(value)); return; }}\n",
                class.name
            ));
        }
        self.output.push_str("    throw std::runtime_error(\"lv_set_field: unknown type for field: \" + name);\n");
        self.output.push_str("}\n\n");
    }

    pub(super) fn indent(&self) -> String {
        "    ".repeat(self.indent_level)
    }

    /// Check if a type annotation involves `dynamic` (std::any)
    pub(super) fn type_contains_dynamic(ty: &Option<Type>) -> bool {
        match ty {
            Some(Type::Dynamic) => true,
            Some(Type::Array(inner)) => Self::type_contains_dynamic(&Some(*inner.clone())),
            _ => false,
        }
    }

}
