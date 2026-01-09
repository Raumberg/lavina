use std::io::{self, Write};
use crate::lexer::Scanner;
use crate::parser::parser::Parser;
use crate::compiler::compiler::{Compiler, FunctionType};
use crate::vm::VM;
use crate::eval::value::Value;
use crate::type_checker::checker::TypeChecker;

pub fn run() {
    let mut vm = VM::new();
    let mut type_checker = TypeChecker::new("".to_string());
    
    println!("Lavina REPL (Bytecode VM + Type Safety) [type 'exit' to quit]");
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

        execute(input, &mut vm, &mut type_checker);
    }
}

fn execute(source: String, vm: &mut VM, type_checker: &mut TypeChecker) {
    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();

    if !errors.is_empty() {
        for error in errors { eprintln!("{}", error); }
        return;
    }

    type_checker.source = source.clone();

    // 1. Пробуем как выражение
    let mut expr_parser = Parser::new(tokens.clone(), source.clone());
    if let Ok(expr) = expr_parser.parse_expression() {
        if let Ok(_) = type_checker.check_expr(&expr) {
            let compiler = Compiler::new("<repl_expr>".to_string(), FunctionType::Script);
            let stmt = crate::parser::ast::Stmt::Expression(expr);
            if let Ok(function) = compiler.compile(&[stmt]) {
                vm.interpret(function);
                return;
            }
        } else {
            // Если тайп-чекер нашел ошибку в выражении, выводим её
            match type_checker.check_expr(&expr) {
                Err(e) => { eprintln!("{}", e); return; },
                _ => {}
            }
        }
    }

    // 2. Пробуем как программу
    let mut parser = Parser::new(tokens.clone(), source.clone());
    match parser.parse_program() {
        Ok(statements) => {
            if let Err(e) = type_checker.check(&statements) {
                eprintln!("{}", e);
                return;
            }

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
