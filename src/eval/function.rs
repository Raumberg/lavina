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
        self.declaration.name.lexeme == other.declaration.name.lexeme
    }
}

impl PartialOrd for LavinaFunction {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.declaration.name.lexeme.partial_cmp(&other.declaration.name.lexeme)
    }
}
