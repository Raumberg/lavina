mod expr;
mod stmt;
mod class;
mod enums;
mod types;

use crate::parser::ast::*;
use crate::error::LavinaError;

/// The Lavina C++ runtime header, embedded at compile time.
pub const RUNTIME_HEADER: &str = include_str!("../../../runtime/lavina.h");

pub struct CppCodegen {
    pub(super) output: String,
    pub(super) indent_level: usize,
    pub(super) temp_counter: usize,
    pub(super) declarations: String,
    pub(super) main_body: String,
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
            main_body: String::new(),
        }
    }

    pub fn generate(mut self, stmts: &[Stmt]) -> Result<String, LavinaError> {
        self.declarations.push_str("#include \"lavina.h\"\n\n");

        for stmt in stmts {
            match stmt {
                Stmt::Function(_) | Stmt::Class(_, _, _)
                | Stmt::Struct(_, _, _) | Stmt::Enum(_, _, _) => {
                    self.emit_stmt(stmt);
                    self.declarations.push_str(&self.output);
                    self.output.clear();
                }
                _ => {
                    self.indent_level = 1;
                    self.emit_stmt(stmt);
                    self.main_body.push_str(&self.output);
                    self.output.clear();
                    self.indent_level = 0;
                }
            }
        }

        let mut result = self.declarations;
        result.push_str("\nint main(int argc, char* argv[]) {\n");
        result.push_str("    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);\n");
        result.push_str(&self.main_body);
        result.push_str("    return 0;\n}\n");

        Ok(result)
    }

    pub(super) fn indent(&self) -> String {
        "    ".repeat(self.indent_level)
    }
}
