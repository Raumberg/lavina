use std::path::{PathBuf};
use std::fs;
use crate::error::{LavinaError, ErrorPhase};

pub struct ModuleResolver {
    source: String,
}

impl ModuleResolver {
    pub fn new(source: String) -> Self {
        Self { source }
    }

    pub fn resolve(&self, path_tokens: &[crate::lexer::Token], phase: ErrorPhase) -> Result<PathBuf, LavinaError> {
        let mut relative_path = PathBuf::new();
        for token in path_tokens {
            relative_path.push(&token.lexeme);
        }
        relative_path.set_extension("lv");

        // 1. Check relative to current directory
        if relative_path.exists() {
            return Ok(fs::canonicalize(relative_path).unwrap());
        }

        // 2. Check LVPATH environment variable
        if let Ok(lv_path) = std::env::var("LVPATH") {
            for dir in std::env::split_paths(&lv_path) {
                let full_path = dir.join(&relative_path);
                if full_path.exists() {
                    return Ok(fs::canonicalize(full_path).unwrap());
                }
            }
        }

        // 3. Special case for 'std' (std lib)
        if !path_tokens.is_empty() && path_tokens[0].lexeme == "std" {
            let mut std_path = PathBuf::from("std");
            for token in &path_tokens[1..] {
                std_path.push(&token.lexeme);
            }
            std_path.set_extension("lv");
            if std_path.exists() {
                return Ok(fs::canonicalize(std_path).unwrap());
            }
        }

        Err(LavinaError::new(
            phase,
            format!("Module not found: {:?}", relative_path),
            path_tokens[0].line,
            path_tokens[0].column,
        ).with_context(&self.source))
    }
}
