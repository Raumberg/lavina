use crate::parser::ast::FunctionDecl;
use crate::eval::environment::Environment;

#[derive(Debug, Clone)]
pub struct LavinaFunction {
    pub declaration: FunctionDecl,
    pub closure: Environment,
}

impl LavinaFunction {
    pub fn new(declaration: FunctionDecl, closure: Environment) -> Self {
        Self {
            declaration,
            closure,
        }
    }
}

impl PartialEq for LavinaFunction {
    fn eq(&self, other: &Self) -> bool {
        // Functions are equal if they have the same name and are defined in the same environment
        // For simplicity, we compare names and memory addresses or just names
        self.declaration.name.lexeme == other.declaration.name.lexeme
    }
}
