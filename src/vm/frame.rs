use crate::vm::object::ObjFunction;
use std::rc::Rc;

pub struct CallFrame {
    pub function: Rc<ObjFunction>,
    pub ip: usize,
    pub slots_offset: usize,
}
