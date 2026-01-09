use std::io::{self, Write};
use crate::lexer::scanner::Scanner;
use crate::parser::parser::Parser;
use crate::type_checker::checker::TypeChecker;
use crate::compiler::compiler::{Compiler, FunctionType};
use crate::vm::vm::{VM, InterpretResult};

pub fn run() {
    let mut line = String::new();
    let mut vm = VM::new();
    let mut type_checker = TypeChecker::new(); // Persistent TypeChecker

    println!("Lavina REPL (type 'exit' to quit)");
    println!("Note: Blocks (after ':') must be indented with spaces.");

    loop {
        print!("> ");
        io::stdout().flush().unwrap();
        line.clear();

        if io::stdin().read_line(&mut line).unwrap() == 0 {
            break;
        }

        let trimmed_line = line.trim();
        if trimmed_line == "exit" {
            break;
        }
        if trimmed_line.is_empty() {
            continue;
        }

        let source = trimmed_line.to_string();

        // Lexing
        let mut scanner = Scanner::new(source.clone());
        let (tokens, lex_errors) = scanner.scan_tokens();
        if !lex_errors.is_empty() {
            for e in lex_errors {
                eprintln!("{}", e);
            }
            continue;
        }

        // Parsing
        let mut parser = Parser::new(tokens.clone(), source.clone());
        
        // Try to parse as expression first (for immediate printing)
        let mut is_expr = false;
        let mut expr_res = None;
        if let Ok(expr) = parser.parse_expression() {
            if parser.is_at_end() {
                is_expr = true;
                expr_res = Some(expr);
            }
        }

        if is_expr {
            let expr = expr_res.unwrap();
            type_checker.source = source.clone();
            if let Ok(_) = type_checker.check_expr(&expr) {
                // To evaluate expression in VM, we wrap it in a print or just compile it
                // For now, let's wrap it in a statement for the compiler
                let statements = vec![crate::parser::ast::Stmt::Expression(expr)];
                let compiler = Compiler::new("<repl>".to_string(), FunctionType::Script);
                if let Ok(function) = compiler.compile(&statements) {
                    match vm.interpret(function) {
                        InterpretResult::Ok => {
                            // If the last instruction pushed a value, print it
                            // (In our current VM implementation, we might need a special REPL mode)
                        },
                        _ => {}
                    }
                }
            }
            continue;
        }

        // Otherwise parse as full program
        let mut parser = Parser::new(tokens.clone(), source.clone());
        let statements = match parser.parse_program() {
            Ok(s) => s,
            Err(e) => {
                eprintln!("{}", e);
                continue;
            }
        };

        // Type Checking
        type_checker.source = source.clone();
        if let Err(e) = type_checker.check_statements(&statements) {
            eprintln!("{}", e);
            continue;
        }

        // Compiling
        let compiler = Compiler::new("<repl>".to_string(), FunctionType::Script);
        let function = match compiler.compile(&statements) {
            Ok(f) => f,
            Err(e) => {
                eprintln!("{}", e);
                continue;
            }
        };

        // Interpreting (VM)
        match vm.interpret(function) {
            InterpretResult::Ok => {},
            InterpretResult::CompileError => eprintln!("Compile Error in REPL."),
            InterpretResult::RuntimeError => {}, // VM already prints runtime error
        }
    }
}
