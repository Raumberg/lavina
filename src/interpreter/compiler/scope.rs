#[derive(PartialEq, Clone)]
pub enum FunctionType {
    Script,
    Function,
}

pub struct Local {
    pub name: String,
    pub depth: i32,
    pub is_captured: bool,
}
