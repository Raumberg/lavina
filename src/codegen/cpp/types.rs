use crate::parser::ast::Type;
use crate::lexer::token::TokenType;
use super::CppCodegen;

impl CppCodegen {
    pub(super) fn emit_type(&self, ty: &Type) -> String {
        match ty {
            Type::Int => "int64_t".to_string(),
            Type::Float => "double".to_string(),
            Type::Bool => "bool".to_string(),
            Type::String => "std::string".to_string(),
            Type::Void => "void".to_string(),
            Type::Auto => "auto".to_string(),
            Type::Dynamic => "auto".to_string(),
            Type::Null => "std::nullptr_t".to_string(),
            Type::Array(inner) => format!("std::vector<{}>", self.emit_type(inner)),
            Type::Tuple(types) => {
                let ts: Vec<String> = types.iter().map(|t| self.emit_type(t)).collect();
                format!("std::tuple<{}>", ts.join(", "))
            }
            Type::Dict(key, value) => {
                format!(
                    "std::unordered_map<{}, {}>",
                    self.emit_type(key),
                    self.emit_type(value)
                )
            }
            Type::Function(ret, params) => {
                let param_types: Vec<String> = params.iter().map(|t| self.emit_type(t)).collect();
                format!(
                    "std::function<{}({})>",
                    self.emit_type(ret),
                    param_types.join(", ")
                )
            }
            Type::Nullable(inner) => format!("std::optional<{}>", self.emit_type(inner)),
            Type::Custom(name) => name.clone(),
        }
    }

    pub(super) fn token_to_cpp_op(&self, token: &crate::lexer::token::Token) -> &str {
        match token.token_type {
            TokenType::Plus => "+",
            TokenType::Minus => "-",
            TokenType::Star => "*",
            TokenType::Slash => "/",
            TokenType::EqualEqual => "==",
            TokenType::BangEqual => "!=",
            TokenType::Less => "<",
            TokenType::LessEqual => "<=",
            TokenType::Greater => ">",
            TokenType::GreaterEqual => ">=",
            TokenType::Bang => "!",
            TokenType::And => "&&",
            TokenType::Or => "||",
            _ => "/* unknown op */",
        }
    }

    pub(super) fn default_init(&self, cpp_type: &str) -> String {
        match cpp_type {
            "int64_t" => " = 0".to_string(),
            "double" => " = 0.0".to_string(),
            "bool" => " = false".to_string(),
            "std::string" => " = \"\"".to_string(),
            _ => String::new(),
        }
    }
}
