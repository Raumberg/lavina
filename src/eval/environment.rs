use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use crate::eval::value::Value;

#[derive(Debug, Clone)]
pub struct Environment {
    inner: Rc<RefCell<EnvironmentInner>>,
}

#[derive(Debug, Clone)]
struct EnvironmentInner {
    values: HashMap<String, Value>,
    enclosing: Option<Environment>,
}

impl Environment {
    pub fn new() -> Self {
        Self {
            inner: Rc::new(RefCell::new(EnvironmentInner {
                values: HashMap::new(),
                enclosing: None,
            })),
        }
    }

    pub fn with_enclosing(enclosing: Environment) -> Self {
        Self {
            inner: Rc::new(RefCell::new(EnvironmentInner {
                values: HashMap::new(),
                enclosing: Some(enclosing),
            })),
        }
    }

    pub fn define(&self, name: String, value: Value) {
        self.inner.borrow_mut().values.insert(name, value);
    }

    pub fn get(&self, name: &str) -> Option<Value> {
        let inner = self.inner.borrow();
        if let Some(value) = inner.values.get(name) {
            return Some(value.clone());
        }

        if let Some(enclosing) = &inner.enclosing {
            return enclosing.get(name);
        }

        None
    }

    pub fn assign(&self, name: &str, value: Value) -> bool {
        let mut inner = self.inner.borrow_mut();
        if inner.values.contains_key(name) {
            inner.values.insert(name.to_string(), value);
            return true;
        }

        if let Some(enclosing) = &inner.enclosing {
            return enclosing.assign(name, value);
        }

        false
    }
    
    pub fn get_enclosing(&self) -> Option<Environment> {
        self.inner.borrow().enclosing.clone()
    }
}
