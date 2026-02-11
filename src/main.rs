use std::env;
use std::fs;
use lavina::lexer::scanner::Scanner;
use lavina::parser::parser::Parser;
use lavina::type_checker::checker::TypeChecker;
use lavina::interpreter::compiler::compiler::{Compiler};
use lavina::interpreter::compiler::scope::FunctionType;
use lavina::interpreter::vm::vm::VM;
use lavina::codegen::cpp::{CppCodegen, RUNTIME_HEADER};

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() == 1 {
        lavina::repl::run();
        return;
    }

    let mut use_vm = false;
    let mut emit_cpp = false;
    let mut file_path = None;

    for arg in &args[1..] {
        match arg.as_str() {
            "--vm" => use_vm = true,
            "--emit-cpp" => emit_cpp = true,
            _ => file_path = Some(arg.as_str()),
        }
    }

    let path = match file_path {
        Some(p) => p,
        None => {
            eprintln!("Usage: lavina [--vm|--emit-cpp] <file.lv>");
            return;
        }
    };

    if use_vm {
        run_vm(path);
    } else {
        run_cpp(path, emit_cpp);
    }
}

fn run_vm(path: &str) {
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

fn run_cpp(path: &str, emit_only: bool) {
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

            let codegen = CppCodegen::new();
            match codegen.generate(&statements) {
                Ok(cpp_code) => {
                    if emit_only {
                        println!("{}", cpp_code);
                        return;
                    }

                    // Write generated .cpp and runtime header
                    let cpp_path = path.replace(".lv", ".cpp");
                    let bin_path = path.replace(".lv", "");
                    let dir = std::path::Path::new(&cpp_path).parent().unwrap_or(std::path::Path::new("."));
                    let header_path = dir.join("lavina.h");

                    if let Err(e) = fs::write(&cpp_path, &cpp_code) {
                        eprintln!("Error writing C++ file: {}", e);
                        return;
                    }
                    if let Err(e) = fs::write(&header_path, RUNTIME_HEADER) {
                        eprintln!("Error writing runtime header: {}", e);
                        return;
                    }

                    let include_dir = dir.to_str().unwrap_or(".");
                    let compile_status = std::process::Command::new("g++")
                        .args(&["-std=c++23", &format!("-I{}", include_dir), "-o", &bin_path, &cpp_path])
                        .status();

                    match compile_status {
                        Ok(status) if status.success() => {
                            // Remove generated files after successful compilation
                            let _ = fs::remove_file(&cpp_path);
                            let _ = fs::remove_file(&header_path);

                            let run_status = std::process::Command::new(&bin_path)
                                .status();

                            // Clean up binary
                            let _ = fs::remove_file(&bin_path);

                            if let Err(e) = run_status {
                                eprintln!("Error running compiled program: {}", e);
                            }
                        }
                        Ok(status) => {
                            eprintln!("C++ compilation failed with status: {}", status);
                        }
                        Err(e) => {
                            eprintln!("Error running g++: {}", e);
                        }
                    }
                }
                Err(e) => eprintln!("{}", e),
            }
        }
        Err(e) => eprintln!("{}", e),
    }
}
