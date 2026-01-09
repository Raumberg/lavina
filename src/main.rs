use std::env;
use std::fs;
use std::process;
use lavina::lexer::Scanner;
use lavina::parser::parser::Parser;
use lavina::eval::tree_walker::TreeWalker;
use lavina::type_checker::checker::TypeChecker;
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
        for error in errors {
            eprintln!("{}", error);
        }
        process::exit(65);
    }

    let mut parser = Parser::new(tokens.clone(), source.clone());
    match parser.parse_program() {
        Ok(statements) => {
            let mut type_checker = TypeChecker::new(source.clone());
            if let Err(e) = type_checker.check(&statements) {
                eprintln!("{}", e);
                process::exit(65);
            }

            let mut tree_walker = TreeWalker::new(source);
            if let Err(e) = tree_walker.interpret(&statements) {
                eprintln!("{}", e);
                process::exit(70);
            }
        }
        Err(e) => {
            eprintln!("{}", e);
            process::exit(65);
        }
    }
}
