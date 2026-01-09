use std::env;
use std::fs;
use lavina::lexer::scanner::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;
use lavina::compiler::compiler::{Compiler, FunctionType};
use lavina::vm::vm::VM;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() == 1 {
        lavina::repl::run();
    } else if args.len() == 2 {
        run_file(&args[1]);
    } else {
        eprintln!("Usage: lavina [path]");
    }
}

fn run_file(path: &str) {
    let source = match fs::read_to_string(path) {
        Ok(s) => s,
        Err(e) => {
            eprintln!("Error reading file {}: {}", path, e);
            return;
        }
    };

    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();
    
    if !errors.is_empty() {
        for e in errors {
            eprintln!("{}", e);
        }
        return;
    }

    let mut parser = Parser::new(tokens.clone(), source.clone());
    match parser.parse_program() {
        Ok(statements) => {
            let mut type_checker = TypeChecker::new();
            type_checker.set_source(source.clone());
            if let Err(e) = type_checker.check_statements(&statements) {
                eprintln!("{}", e);
                return;
            }

            let compiler = Compiler::new("<script>".to_string(), FunctionType::Script);
            match compiler.compile(&statements) {
                Ok(function) => {
                    let mut vm = VM::new();
                    vm.interpret(function);
                }
                Err(e) => eprintln!("{}", e),
            }
        }
        Err(e) => eprintln!("{}", e),
    }
}
