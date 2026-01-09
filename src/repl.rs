use std::io::{self, Write};
use crate::lexer::Scanner;
use crate::parser::parser::Parser;
use crate::compiler::compiler::{Compiler, FunctionType};
use crate::vm::VM;
use crate::eval::value::Value;

pub fn run() {
    let mut vm = VM::new();
    println!("Lavina REPL (Bytecode VM) [type 'exit' to quit]");
    println!("Note: Blocks (after ':') must be indented with spaces.");

    loop {
        print!("> ");
        io::stdout().flush().unwrap();

        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("Failed to read line");

        if input.trim() == "exit" {
            break;
        }

        if input.trim().is_empty() {
            continue;
        }

        execute(input, &mut vm);
    }
}

fn execute(source: String, vm: &mut VM) {
    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();

    if !errors.is_empty() {
        for error in errors { eprintln!("{}", error); }
        return;
    }

    let mut parser = Parser::new(tokens.clone(), source.clone());
    
    // Пытаемся распарсить как выражение для удобства вывода результата в REPL
    let mut expr_parser = Parser::new(tokens.clone(), source.clone());
    if let Ok(expr) = expr_parser.parse_expression() {
        // Оборачиваем выражение в неявный print или просто компилируем его
        // Для REPL удобно сразу видеть результат выражения
        let compiler = Compiler::new("<repl_expr>".to_string(), FunctionType::Script);
        // Временно создаем стейтмент из выражения, чтобы компилятор его съел
        let stmt = crate::parser::ast::Stmt::Expression(expr);
        if let Ok(function) = compiler.compile(&[stmt]) {
            // Нам нужно, чтобы результат выражения остался на стеке и мы его напечатали
            // Но наш компилятор сейчас добавляет OP_POP в конце compile_stmt
            // Поэтому мы немного схитрим или просто запустим как есть
            vm.interpret(function);
            // Если на стеке что-то осталось (результат выражения до OP_POP), 
            // мы могли бы это напечатать, но сейчас VM чистит за собой.
            return;
        }
    }

    // Если это не просто выражение, а программа (let, fn, etc)
    match parser.parse_program() {
        Ok(statements) => {
            let compiler = Compiler::new("<repl>".to_string(), FunctionType::Script);
            match compiler.compile(&statements) {
                Ok(function) => {
                    vm.interpret(function);
                }
                Err(e) => eprintln!("{}", e),
            }
        }
        Err(e) => eprintln!("{}", e),
    }
}
