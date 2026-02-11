mod expr;
mod stmt;
mod class;
mod enums;
mod types;

use std::collections::{HashSet, HashMap};
use crate::parser::ast::*;
use crate::error::LavinaError;

/// The Lavina C++ runtime header, embedded at compile time.
pub const RUNTIME_HEADER: &str = include_str!("../../../runtime/lavina.h");

pub struct CppCodegen {
    pub(super) output: String,
    pub(super) indent_level: usize,
    pub(super) temp_counter: usize,
    pub(super) declarations: String,
    /// Variable names known to hold std::any (declared as `dynamic` or from for-loops over dynamic vectors)
    pub(super) dynamic_vars: HashSet<String>,
    pub(super) has_main: bool,
    /// Known enum definitions: enum_name -> variants
    pub(super) known_enums: HashMap<String, Vec<EnumVariant>>,
}

impl CppCodegen {
    pub fn new() -> Self {
        Self {
            output: String::new(),
            indent_level: 0,
            temp_counter: 0,
            declarations: String::new(),
            dynamic_vars: HashSet::new(),
            has_main: false,
            known_enums: HashMap::new(),
        }
    }

    pub fn generate(mut self, stmts: &[Stmt]) -> Result<String, LavinaError> {
        self.declarations.push_str("#include \"lavina.h\"\n\n");

        for stmt in stmts {
            if let Stmt::Function(decl) = stmt {
                if decl.name.lexeme == "main" {
                    self.has_main = true;
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
