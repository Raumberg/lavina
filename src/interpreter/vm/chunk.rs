use crate::interpreter::vm::opcode::OpCode;
use crate::interpreter::value::Value;

#[derive(Debug, Clone, Copy, PartialEq)]
pub struct UpvalueLoc {
    pub index: u8,
    pub is_local: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Chunk {
    pub code: Vec<u8>,
    pub constants: Vec<Value>,
    pub lines: Vec<usize>,
    pub upvalues: Vec<UpvalueLoc>,
}

impl Chunk {
    pub fn new() -> Self {
        Self {
            code: Vec::new(),
            constants: Vec::new(),
            lines: Vec::new(),
            upvalues: Vec::new(),
        }
    }

    pub fn write(&mut self, byte: u8, line: usize) {
        self.code.push(byte);
        self.lines.push(line);
    }

    pub fn add_constant(&mut self, value: Value) -> usize {
        self.constants.push(value);
        self.constants.len() - 1
    }

    pub fn disassemble(&self, name: &str) {
        println!("== {} ==", name);
        let mut offset = 0;
        while offset < self.code.len() {
            offset = self.disassemble_instruction(offset);
        }
    }

    pub fn disassemble_instruction(&self, offset: usize) -> usize {
        print!("{:04} ", offset);
        if offset > 0 && self.lines[offset] == self.lines[offset - 1] {
            print!("   | ");
        } else {
            print!("{:4} ", self.lines[offset]);
        }

        let instruction = self.code[offset];
        match OpCode::from(instruction) {
            OpCode::Return => self.simple_instruction("OP_RETURN", offset),
            OpCode::Constant => self.constant_instruction("OP_CONSTANT", offset),
            OpCode::True => self.simple_instruction("OP_TRUE", offset),
            OpCode::False => self.simple_instruction("OP_FALSE", offset),
            OpCode::Null => self.simple_instruction("OP_NULL", offset),
            OpCode::Pop => self.simple_instruction("OP_POP", offset),
            OpCode::GetLocal => self.byte_instruction("OP_GET_LOCAL", offset),
            OpCode::SetLocal => self.byte_instruction("OP_SET_LOCAL", offset),
            OpCode::DefineGlobal => self.constant_instruction("OP_DEFINE_GLOBAL", offset),
            OpCode::GetGlobal => self.constant_instruction("OP_GET_GLOBAL", offset),
            OpCode::SetGlobal => self.constant_instruction("OP_SET_GLOBAL", offset),
            OpCode::Equal => self.simple_instruction("OP_EQUAL", offset),
            OpCode::Greater => self.simple_instruction("OP_GREATER", offset),
            OpCode::Less => self.simple_instruction("OP_LESS", offset),
            OpCode::Add => self.simple_instruction("OP_ADD", offset),
            OpCode::Subtract => self.simple_instruction("OP_SUBTRACT", offset),
            OpCode::Multiply => self.simple_instruction("OP_MULTIPLY", offset),
            OpCode::Divide => self.simple_instruction("OP_DIVIDE", offset),
            OpCode::Negate => self.simple_instruction("OP_NEGATE", offset),
            OpCode::Not => self.simple_instruction("OP_NOT", offset),
            OpCode::Jump => self.jump_instruction("OP_JUMP", 1, offset),
            OpCode::JumpIfFalse => self.jump_instruction("OP_JUMP_IF_FALSE", 1, offset),
            OpCode::Loop => self.jump_instruction("OP_LOOP", -1, offset),
            OpCode::Call => self.byte_instruction("OP_CALL", offset),
            OpCode::Vector => self.byte_instruction("OP_VECTOR", offset),
            OpCode::HashMap => self.byte_instruction("OP_HASH_MAP", offset),
            OpCode::GetIndex => self.simple_instruction("OP_GET_INDEX", offset),
            OpCode::SetIndex => self.simple_instruction("OP_SET_INDEX", offset),
            OpCode::Namespace => self.constant_instruction("OP_NAMESPACE", offset),
            OpCode::Closure => {
                let mut offset = offset + 1;
                let constant = self.code[offset] as usize;
                offset += 1;
                print!("{:<16} {:4} ", "OP_CLOSURE", constant);
                println!("{}", self.constants[constant]);
                
                // Disassemble upvalues
                // But we don't know the count here without looking at the constant.
                // In clox, it's encoded in the bytecode.
                offset
            }
            OpCode::GetUpvalue => self.byte_instruction("OP_GET_UPVALUE", offset),
            OpCode::SetUpvalue => self.byte_instruction("OP_SET_UPVALUE", offset),
            OpCode::CloseUpvalue => self.simple_instruction("OP_CLOSE_UPVALUE", offset),
            OpCode::GetProperty => self.constant_instruction("OP_GET_PROPERTY", offset),
            OpCode::SetProperty => self.constant_instruction("OP_SET_PROPERTY", offset),
            OpCode::Class => self.constant_instruction("OP_CLASS", offset),
            OpCode::Method => self.constant_instruction("OP_METHOD", offset),
            OpCode::Cast => self.simple_instruction("OP_CAST", offset),
            OpCode::Try => self.byte_instruction("OP_TRY", offset),
            OpCode::EndTry => self.simple_instruction("OP_END_TRY", offset),
            OpCode::Throw => self.simple_instruction("OP_THROW", offset),
        }
    }

    fn simple_instruction(&self, name: &str, offset: usize) -> usize {
        println!("{}", name);
        offset + 1
    }

    fn constant_instruction(&self, name: &str, offset: usize) -> usize {
        let constant = self.code[offset + 1] as usize;
        print!("{:<16} {:4} '", name, constant);
        print!("{}", self.constants[constant]);
        println!("'");
        offset + 2
    }

    fn byte_instruction(&self, name: &str, offset: usize) -> usize {
        let slot = self.code[offset + 1];
        println!("{:<16} {:4}", name, slot);
        offset + 2
    }

    fn jump_instruction(&self, name: &str, sign: i32, offset: usize) -> usize {
        let mut jump = (self.code[offset + 1] as u16) << 8;
        jump |= self.code[offset + 2] as u16;
        println!("{:<16} {:4} -> {}", name, offset, offset as i32 + 3 + sign * jump as i32);
        offset + 3
    }
}
