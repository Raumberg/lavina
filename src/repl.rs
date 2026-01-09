use std::io::{self, Write};
use crate::lexer::Scanner;
use crate::parser::parser::Parser;
use crate::eval::tree_walker::TreeWalker;
use crate::type_checker::checker::TypeChecker;

pub fn run() {
    let mut tree_walker = TreeWalker::new("".to_string());
    let mut type_checker = TypeChecker::new("".to_string());
    
    println!("Lavina REPL (type 'exit' to quit)");
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

        execute(input, &mut tree_walker, &mut type_checker);
    }
}

fn execute(source: String, tree_walker: &mut TreeWalker, type_checker: &mut TypeChecker) {
    let mut scanner = Scanner::new(source.clone());
    let (tokens, errors) = scanner.scan_tokens();

    if !errors.is_empty() {
        for error in errors {
            eprintln!("{}", error);
        }
        return;
    }

    // Update source for correct error reporting context
    tree_walker.source = source.clone();
    type_checker.source = source.clone();

    // Try to parse as an expression first for convenience
    let mut expr_parser = Parser::new(tokens.clone(), source.clone());
    if let Ok(expr) = expr_parser.parse_expression() {
        match type_checker.check_expr(&expr) {
            Ok(_) => {
                match tree_walker.interpret_expression(&expr) {
                    Ok(value) => println!("{}", value),
                    Err(e) => eprintln!("{}", e),
                }
            }
            Err(e) => eprintln!("{}", e),
        }
        return;
    }

    let mut parser = Parser::new(tokens.clone(), source.clone());
    match parser.parse_program() {
        Ok(statements) => {
            if let Err(e) = type_checker.check(&statements) {
                eprintln!("{}", e);
                return;
            }

            if let Err(e) = tree_walker.interpret(&statements) {
                eprintln!("{}", e);
            }
        }
        Err(e) => eprintln!("{}", e),
    }
}
