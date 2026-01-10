pub struct CallFrame {
    pub closure_idx: usize, // Index into the heap for the ObjClosure
    pub ip: usize,
    pub slots_offset: usize,
}
