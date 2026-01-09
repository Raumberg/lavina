use std::env;
use std::fs;
use std::process;
use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;
use lavina::compiler::compiler::{Compiler, FunctionType};
use lavina::vm::VM;
use lavina::repl;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() > 2 {
        println!("Usage: lavina [script]");
        process::exit(64);
    } else if args.len() == 2 {
        run_file(&args[1]);
    } else {
        repl::run();
    }
}

fn run_file(path: &str) {
    let source = fs::read_to_string(path).expect("Could not read file");
    
    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();
    
    if !errors.is_empty() {
        for error in errors { eprintln!("{}", error); }
        process::exit(65);
    }

    let mut parser = Parser::new(tokens.clone(), source.clone());
    match parser.parse_program() {
        Ok(statements) => {
            // Тайпчекер временно отключен, пока не адаптируем под VM
            /*
            let mut type_checker = TypeChecker::new(source.clone());
            if let Err(e) = type_checker.check(&statements) {
                eprintln!("{}", e);
                process::exit(65);
            }
            */

            let compiler = Compiler::new("<script>".to_string(), FunctionType::Script);
            match compiler.compile(&statements) {
                Ok(function) => {
                    let mut vm = VM::new();
                    vm.interpret(function);
                }
                Err(e) => {
                    eprintln!("{}", e);
                    process::exit(65);
                }
            }
        }
        Err(e) => {
            eprintln!("{}", e);
            process::exit(65);
        }
    }
}
