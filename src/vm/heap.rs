use crate::eval::object::{Obj, ObjType};
use crate::eval::value::Value;

/// The memory manager for Lavina. 
/// Implements a Mark-and-Sweep garbage collector over a vector-based heap.
pub struct Heap {
    objects: Vec<Option<Obj>>,
    /// Threshold for triggering the next GC cycle.
    next_gc: usize,
}

impl Heap {
    pub fn new() -> Self {
        Self {
            objects: Vec::new(),
            next_gc: 1024,
        }
    }

    /// Allocates a new object on the heap and returns its index as a Value.
    pub fn alloc(&mut self, obj_type: ObjType) -> Value {
        // Here we could check if GC is needed, but we'll let the VM decide 
        // because it knows the roots (stack, globals).
        
        let obj = Obj::new(obj_type);
        
        // Find a free slot
        for (i, slot) in self.objects.iter_mut().enumerate() {
            if slot.is_none() {
                *slot = Some(obj);
                return Value::Object(i);
            }
        }

        self.objects.push(Some(obj));
        Value::Object(self.objects.len() - 1)
    }

    /// Returns a reference to the objects slice. Used by GC and Native Functions.
    pub fn objects(&self) -> &[Option<Obj>] {
        &self.objects
    }

    /// Returns a mutable reference to an object at the given index.
    pub fn get_mut(&mut self, idx: usize) -> &mut Obj {
        self.objects[idx].as_mut().expect("Accessing dead object in heap")
    }

    /// Returns an immutable reference to an object at the given index.
    pub fn get(&self, idx: usize) -> &Obj {
        self.objects[idx].as_ref().expect("Accessing dead object in heap")
    }

    /// Checks if we should run a GC cycle.
    pub fn should_collect(&self) -> bool {
        self.objects.len() >= self.next_gc
    }

    /// The Mark phase of GC: reset marks.
    pub fn reset_marks(&mut self) {
        for obj in self.objects.iter_mut().flatten() {
            obj.is_marked = false;
        }
    }

    /// The Sweep phase of GC: remove unmarked objects.
    pub fn sweep(&mut self) {
        for slot in self.objects.iter_mut() {
            if let Some(obj) = slot {
                if !obj.is_marked {
                    *slot = None;
                } else {
                    // Reset for next cycle
                    obj.is_marked = false;
                }
            }
        }
        
        // Adjust threshold
        self.next_gc = (self.objects.len() + 1) * 2;
    }
}
