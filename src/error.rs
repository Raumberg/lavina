use std::fmt;

#[derive(Debug, Clone)]
pub enum ErrorPhase {
    Lexer,
    Parser,
    TypeChecker,
    Compiler,
    Runtime,
}

#[derive(Debug, Clone)]
pub struct LavinaError {
    pub phase: ErrorPhase,
    pub message: String,
    pub line: usize,
    pub column: usize,
    pub source_line: Option<String>,
}

impl LavinaError {
    pub fn new(phase: ErrorPhase, message: String, line: usize, column: usize) -> Self {
        Self {
            phase,
            message,
            line,
            column,
            source_line: None,
        }
    }

    pub fn with_context(mut self, source: &str) -> Self {
        let lines: Vec<&str> = source.lines().collect();
        if self.line > 0 && self.line <= lines.len() {
            self.source_line = Some(lines[self.line - 1].to_string());
        }
        self
    }
}

impl fmt::Display for LavinaError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let phase_str = match self.phase {
            ErrorPhase::Lexer => "lexical error",
            ErrorPhase::Parser => "syntax error",
            ErrorPhase::TypeChecker => "type error",
            ErrorPhase::Compiler => "compiler error",
            ErrorPhase::Runtime => "runtime error",
        };

        writeln!(f, "\x1b[1;31merror[{}]\x1b[0m: \x1b[1m{}\x1b[0m", phase_str, self.message)?;
        writeln!(f, "  \x1b[1;34m-->\x1b[0m line {}:{}", self.line, self.column)?;
        
        if let Some(line_content) = &self.source_line {
            writeln!(f, "   \x1b[1;34m|\x1b[0m")?;
            writeln!(f, "{:>3} \x1b[1;34m|\x1b[0m {}", self.line, line_content)?;
            write!(f, "   \x1b[1;34m|\x1b[0m ")?;
            for _ in 0..self.column - 1 {
                write!(f, " ")?;
            }
            writeln!(f, "\x1b[1;31m^\x1b[0m \x1b[1;31mhere\x1b[0m")?;
            writeln!(f, "   \x1b[1;34m|\x1b[0m")?;
        }
        
        Ok(())
    }
}
