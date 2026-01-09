use crate::vm::opcode::OpCode;
use crate::eval::value::Value;

#[derive(Debug, Clone, PartialEq)]
pub struct Chunk {
    pub code: Vec<u8>,
    pub constants: Vec<Value>,
    pub lines: Vec<usize>,
}

impl Chunk {
    pub fn new() -> Self {
        Self {
            code: Vec::new(),
            constants: Vec::new(),
            lines: Vec::new(),
        }
    }

    pub fn write(&mut self, byte: u8, line: usize) {
        self.code.push(byte);
        self.lines.push(line);
    }

    pub fn write_opcode(&mut self, opcode: OpCode, line: usize) {
        self.write(opcode as u8, line);
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
            OpCode::Add => self.simple_instruction("OP_ADD", offset),
            OpCode::Subtract => self.simple_instruction("OP_SUBTRACT", offset),
            OpCode::Multiply => self.simple_instruction("OP_MULTIPLY", offset),
            OpCode::Divide => self.simple_instruction("OP_DIVIDE", offset),
            OpCode::Negate => self.simple_instruction("OP_NEGATE", offset),
            OpCode::True => self.simple_instruction("OP_TRUE", offset),
            OpCode::False => self.simple_instruction("OP_FALSE", offset),
            OpCode::Null => self.simple_instruction("OP_NULL", offset),
            OpCode::Not => self.simple_instruction("OP_NOT", offset),
            OpCode::Equal => self.simple_instruction("OP_EQUAL", offset),
            OpCode::Greater => self.simple_instruction("OP_GREATER", offset),
            OpCode::Less => self.simple_instruction("OP_LESS", offset),
            OpCode::Pop => self.simple_instruction("OP_POP", offset),
            OpCode::DefineGlobal => self.constant_instruction("OP_DEFINE_GLOBAL", offset),
            OpCode::GetGlobal => self.constant_instruction("OP_GET_GLOBAL", offset),
            OpCode::SetGlobal => self.constant_instruction("OP_SET_GLOBAL", offset),
            OpCode::GetLocal => self.byte_instruction("OP_GET_LOCAL", offset),
            OpCode::SetLocal => self.byte_instruction("OP_SET_LOCAL", offset),
            OpCode::Jump => self.short_instruction("OP_JUMP", offset),
            OpCode::JumpIfFalse => self.short_instruction("OP_JUMP_IF_FALSE", offset),
            OpCode::Loop => self.short_instruction("OP_LOOP", offset),
            OpCode::Call => self.byte_instruction("OP_CALL", offset),
            OpCode::Vector => self.byte_instruction("OP_VECTOR", offset),
            OpCode::HashMap => self.byte_instruction("OP_HASH_MAP", offset),
        }
    }

    fn simple_instruction(&self, name: &str, offset: usize) -> usize {
        println!("{}", name);
        offset + 1
    }

    fn byte_instruction(&self, name: &str, offset: usize) -> usize {
        let slot = self.code[offset + 1];
        println!("{:<16} {:4}", name, slot);
        offset + 2
    }

    fn short_instruction(&self, name: &str, offset: usize) -> usize {
        let mut jump = (self.code[offset + 1] as u16) << 8;
        jump |= self.code[offset + 2] as u16;
        println!("{:<16} {:4}", name, jump);
        offset + 3
    }

    fn constant_instruction(&self, name: &str, offset: usize) -> usize {
        let constant = self.code[offset + 1];
        print!("{:<16} {:4} '", name, constant);
        print!("{}", self.constants[constant as usize]);
        println!("'");
        offset + 2
    }
}
