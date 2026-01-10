use crate::parser::ast::{Type, Visibility};
use std::collections::HashMap;

#[derive(Clone)]
pub enum TypeInfo {
    Variable(Type),
    Function(Type, Vec<Type>),
    Namespace(String, HashMap<String, (TypeInfo, Visibility)>),
}

pub struct TypeEnvironment {
    pub values: HashMap<String, (TypeInfo, Visibility)>,
    pub enclosing: Option<Box<TypeEnvironment>>,
}

impl TypeEnvironment {
    pub fn new() -> Self {
        Self {
            values: HashMap::new(),
            enclosing: None,
        }
    }

    pub fn define(&mut self, name: String, info: TypeInfo, visibility: Visibility) {
        self.values.insert(name, (info, visibility));
    }

    pub fn get(&self, name: &str) -> Option<TypeInfo> {
        if let Some((info, _)) = self.values.get(name) {
            return Some(info.clone());
        }
        if let Some(enclosing) = &self.enclosing {
            return enclosing.get(name);
        }
        None
    }
}
