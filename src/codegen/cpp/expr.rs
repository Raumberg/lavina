use crate::parser::ast::*;
use crate::lexer::token::TokenType;
use super::CppCodegen;

impl CppCodegen {
    pub(super) fn emit_expr(&mut self, expr: &Expr) -> String {
        match expr {
            Expr::Literal(lit) => self.emit_literal(lit),
            Expr::Unary(op, right) => {
                let r = self.emit_expr(right);
                let op_str = self.token_to_cpp_op(op);
                format!("({}{})", op_str, r)
            }
            Expr::Binary(left, op, right) => {
                let l = self.emit_expr(left);
                let r = self.emit_expr(right);
                let op_str = self.token_to_cpp_op(op);
                format!("({} {} {})", l, op_str, r)
            }
            Expr::Grouping(inner) => {
                let i = self.emit_expr(inner);
                format!("({})", i)
            }
            Expr::Variable(token) => token.lexeme.clone(),
            Expr::Assign(name, value) => {
                let v = self.emit_expr(value);
                format!("{} = {}", name.lexeme, v)
            }
            Expr::Logical(left, op, right) => {
                let l = self.emit_expr(left);
                let r = self.emit_expr(right);
                let op_str = match op.token_type {
                    TokenType::And => "&&",
                    TokenType::Or => "||",
                    _ => "&&",
                };
                format!("({} {} {})", l, op_str, r)
            }
            // Method call: obj.method(args) → intercept known methods
            Expr::Call(callee, _paren, args) => {
                if let Expr::Get(obj, method) = callee.as_ref() {
                    if let Some(result) = self.try_emit_method_call(obj, &method.lexeme, args) {
                        return result;
                    }
                    // Not a built-in method — emit as obj.method(args) directly
                    let o = self.emit_expr(obj);
                    let arg_strs: Vec<String> = args.iter().map(|a| self.emit_expr(a)).collect();
                    return format!("{}.{}({})", o, method.lexeme, arg_strs.join(", "));
                }
                // Static call: Type::method(args)
                if let Expr::StaticGet(obj, method) = callee.as_ref() {
                    let o = self.emit_expr(obj);
                    let arg_strs: Vec<String> = args.iter().map(|a| self.emit_expr(a)).collect();
                    // Enum factory: Enum::Variant(args) -> Enum::make_Variant(args)
                    if let Expr::Variable(tok) = obj.as_ref() {
                        if self.known_enums.contains_key(&tok.lexeme) {
                            return format!("{}::make_{}({})", o, method.lexeme, arg_strs.join(", "));
                        }
                    }
                    return format!("{}::{}({})", o, method.lexeme, arg_strs.join(", "));
                }
                // Global function remapping
                if let Expr::Variable(tok) = callee.as_ref() {
                    if let Some(result) = self.try_emit_global_call(&tok.lexeme, args) {
                        return result;
                    }
                }
                let func = self.emit_expr(callee);
                let arg_strs: Vec<String> = args.iter().map(|a| self.emit_expr(a)).collect();
                format!("{}({})", func, arg_strs.join(", "))
            }
            Expr::Index(object, _bracket, index) => {
                let obj = self.emit_expr(object);
                let idx = self.emit_expr(index);
                format!("{}[{}]", obj, idx)
            }
            Expr::Vector(elements) => {
                let elems: Vec<String> = elements.iter().map(|e| self.emit_expr(e)).collect();
                if elems.is_empty() {
                    "{}".to_string()
                } else {
                    format!("std::vector{{{}}}", elems.join(", "))
                }
            }
            Expr::Map(pairs) => {
                let entries: Vec<String> = pairs
                    .iter()
                    .map(|(k, v)| {
                        let key = self.emit_expr(k);
                        let val = self.emit_expr(v);
                        format!("{{{}, {}}}", key, val)
                    })
                    .collect();
                format!("std::unordered_map{{{{{}}}}}", entries.join(", "))
            }
            Expr::Get(object, property) => {
                let obj = self.emit_expr(object);
                format!("{}.{}", obj, property.lexeme)
            }
            Expr::Set(object, property, value) => {
                let obj = self.emit_expr(object);
                let val = self.emit_expr(value);
                format!("{}.{} = {}", obj, property.lexeme, val)
            }
            Expr::StaticGet(object, member) => {
                let obj = self.emit_expr(object);
                format!("{}::{}", obj, member.lexeme)
            }
            Expr::This(_) => "(*this)".to_string(),
            Expr::Cast(expr, target_type) => {
                let e = self.emit_expr(expr);
                let t = self.emit_type(target_type);
                // Use std::any_cast when casting from dynamic (std::any)
                if self.is_dynamic_expr(expr) {
                    format!("std::any_cast<{}>({})", t, e)
                } else {
                    format!("static_cast<{}>({})", t, e)
                }
            }
            Expr::Throw(expr) => {
                let e = self.emit_expr(expr);
                format!("throw std::runtime_error({})", e)
            }
        }
    }

    /// Check if an expression would produce a dynamic (std::any) value.
    pub(super) fn is_dynamic_expr(&self, expr: &Expr) -> bool {
        match expr {
            Expr::Variable(tok) => self.dynamic_vars.contains(&tok.lexeme),
            Expr::Index(obj, _, _) => self.is_dynamic_expr(obj),
            _ => false,
        }
    }

    /// Try to map `obj.method(args)` to a C++ equivalent.
    fn try_emit_method_call(&mut self, obj: &Expr, method: &str, args: &[Expr]) -> Option<String> {
        let o = self.emit_expr(obj);
        let arg_strs: Vec<String> = args.iter().map(|a| self.emit_expr(a)).collect();

        let result = match method {
            // ── Shared ──────────────────────────────────────────
            "len" => format!("static_cast<int64_t>({}.size())", o),
            "is_empty" => format!("{}.empty()", o),
            "contains" => format!("lv_contains({}, {})", o, arg_strs.join(", ")),

            // ── String methods ──────────────────────────────────
            "upper" => format!("lv_upper({})", o),
            "lower" => format!("lv_lower({})", o),
            "trim" => format!("lv_trim({})", o),
            "replace" => format!("lv_replace({}, {}, {})", o, arg_strs.get(0).cloned().unwrap_or_default(), arg_strs.get(1).cloned().unwrap_or_default()),
            "split" => format!("lv_split({}, {})", o, arg_strs.get(0).cloned().unwrap_or("std::string(\" \")".to_string())),
            "starts_with" => format!("{}.starts_with({})", o, arg_strs.join(", ")),
            "ends_with" => format!("{}.ends_with({})", o, arg_strs.join(", ")),
            "indexOf" | "idxof" => format!("lv_index_of({}, {})", o, arg_strs.join(", ")),
            "charAt" => format!("std::string(1, {}[{}])", o, arg_strs.join(", ")),
            "substring" => {
                if arg_strs.len() >= 2 {
                    format!("{}.substr({}, ({}) - ({}))", o, arg_strs[0], arg_strs[1], arg_strs[0])
                } else {
                    format!("{}.substr({})", o, arg_strs.join(", "))
                }
            }

            // ── Vector methods ──────────────────────────────────
            "push" => format!("{}.push_back({})", o, arg_strs.join(", ")),
            "pop" => format!("lv_pop({})", o),
            "clear" => format!("{}.clear()", o),
            "remove" => format!("lv_remove({}, {})", o, arg_strs.join(", ")),
            "join" => format!("lv_join({}, {})", o, arg_strs.get(0).cloned().unwrap_or("std::string(\"\")".to_string())),
            "reverse" => format!("lv_reverse({})", o),

            // ── HashMap methods ─────────────────────────────────
            "keys" => format!("lv_keys({})", o),
            "values" => format!("lv_values({})", o),
            "has" => format!("({}.count({}) > 0)", o, arg_strs.join(", ")),

            // Unknown method — fall through to regular call
            _ => return None,
        };
        Some(result)
    }

    pub(super) fn emit_literal(&self, lit: &Literal) -> String {
        match lit {
            Literal::Int(n) => format!("{}LL", n),
            Literal::Float(f) => format!("{}", f),
            Literal::String(s) => format!(
                "std::string(\"{}\")",
                s.replace('\\', "\\\\").replace('"', "\\\"").replace('\n', "\\n").replace('\t', "\\t").replace('\r', "\\r")
            ),
            Literal::Bool(b) => if *b { "true" } else { "false" }.to_string(),
            Literal::Null => "std::nullopt".to_string(),
        }
    }

    pub(super) fn emit_method_expr(&mut self, expr: &Expr) -> String {
        match expr {
            Expr::Set(obj, prop, value) if matches!(obj.as_ref(), Expr::This(_)) => {
                let val = self.emit_method_expr(value);
                format!("this->{} = {}", prop.lexeme, val)
            }
            Expr::Get(obj, prop) if matches!(obj.as_ref(), Expr::This(_)) => {
                format!("this->{}", prop.lexeme)
            }
            Expr::Get(obj, prop) => {
                let o = self.emit_method_expr(obj);
                format!("{}.{}", o, prop.lexeme)
            }
            Expr::Set(obj, prop, value) => {
                let o = self.emit_method_expr(obj);
                let v = self.emit_method_expr(value);
                format!("{}.{} = {}", o, prop.lexeme, v)
            }
            Expr::This(_) => "(*this)".to_string(),
            Expr::Grouping(inner) => {
                let i = self.emit_method_expr(inner);
                format!("({})", i)
            }
            Expr::Index(object, _bracket, index) => {
                let obj = self.emit_method_expr(object);
                let idx = self.emit_method_expr(index);
                format!("{}[{}]", obj, idx)
            }
            Expr::Vector(elements) => {
                let elems: Vec<String> = elements.iter().map(|e| self.emit_method_expr(e)).collect();
                if elems.is_empty() {
                    "{}".to_string()
                } else {
                    format!("std::vector{{{}}}", elems.join(", "))
                }
            }
            Expr::Binary(left, op, right) => {
                let l = self.emit_method_expr(left);
                let r = self.emit_method_expr(right);
                let op_str = self.token_to_cpp_op(op);
                format!("({} {} {})", l, op_str, r)
            }
            Expr::Unary(op, right) => {
                let r = self.emit_method_expr(right);
                let op_str = self.token_to_cpp_op(op);
                format!("({}{})", op_str, r)
            }
            Expr::Call(callee, _paren, args) => {
                // Also intercept method calls inside method bodies
                if let Expr::Get(obj, method) = callee.as_ref() {
                    if let Some(result) = self.try_emit_method_call_in_method(obj, &method.lexeme, args) {
                        return result;
                    }
                }
                // Enum factory: Enum::Variant(args) -> Enum::make_Variant(args)
                if let Expr::StaticGet(obj, method) = callee.as_ref() {
                    let o = self.emit_method_expr(obj);
                    let arg_strs: Vec<String> = args.iter().map(|a| self.emit_method_expr(a)).collect();
                    if let Expr::Variable(tok) = obj.as_ref() {
                        if self.known_enums.contains_key(&tok.lexeme) {
                            return format!("{}::make_{}({})", o, method.lexeme, arg_strs.join(", "));
                        }
                    }
                    return format!("{}::{}({})", o, method.lexeme, arg_strs.join(", "));
                }
                let func = self.emit_method_expr(callee);
                let arg_strs: Vec<String> =
                    args.iter().map(|a| self.emit_method_expr(a)).collect();
                format!("{}({})", func, arg_strs.join(", "))
            }
            Expr::Logical(left, op, right) => {
                let l = self.emit_method_expr(left);
                let r = self.emit_method_expr(right);
                let op_str = match op.token_type {
                    TokenType::And => "&&",
                    TokenType::Or => "||",
                    _ => "&&",
                };
                format!("({} {} {})", l, op_str, r)
            }
            Expr::Assign(aname, value) => {
                let v = self.emit_method_expr(value);
                format!("{} = {}", aname.lexeme, v)
            }
            _ => self.emit_expr(expr),
        }
    }

    /// Remap global function calls (exit, fs_read, etc.)
    fn try_emit_global_call(&mut self, name: &str, args: &[Expr]) -> Option<String> {
        let arg_strs: Vec<String> = args.iter().map(|a| self.emit_expr(a)).collect();
        let result = match name {
            "exit" => format!("lv_exit({})", arg_strs.join(", ")),
            _ => return None,
        };
        Some(result)
    }

    /// Method call interception within class method bodies
    fn try_emit_method_call_in_method(&mut self, obj: &Expr, method: &str, args: &[Expr]) -> Option<String> {
        let o = self.emit_method_expr(obj);
        let arg_strs: Vec<String> = args.iter().map(|a| self.emit_method_expr(a)).collect();

        let result = match method {
            "len" => format!("static_cast<int64_t>({}.size())", o),
            "is_empty" => format!("{}.empty()", o),
            "contains" => format!("lv_contains({}, {})", o, arg_strs.join(", ")),
            "upper" => format!("lv_upper({})", o),
            "lower" => format!("lv_lower({})", o),
            "trim" => format!("lv_trim({})", o),
            "replace" => format!("lv_replace({}, {}, {})", o, arg_strs.get(0).cloned().unwrap_or_default(), arg_strs.get(1).cloned().unwrap_or_default()),
            "split" => format!("lv_split({}, {})", o, arg_strs.get(0).cloned().unwrap_or("std::string(\" \")".to_string())),
            "starts_with" => format!("{}.starts_with({})", o, arg_strs.join(", ")),
            "ends_with" => format!("{}.ends_with({})", o, arg_strs.join(", ")),
            "indexOf" | "idxof" => format!("lv_index_of({}, {})", o, arg_strs.join(", ")),
            "charAt" => format!("std::string(1, {}[{}])", o, arg_strs.join(", ")),
            "substring" => {
                if arg_strs.len() >= 2 {
                    format!("{}.substr({}, ({}) - ({}))", o, arg_strs[0], arg_strs[1], arg_strs[0])
                } else {
                    format!("{}.substr({})", o, arg_strs.join(", "))
                }
            }
            "push" => format!("{}.push_back({})", o, arg_strs.join(", ")),
            "pop" => format!("lv_pop({})", o),
            "clear" => format!("{}.clear()", o),
            "remove" => format!("lv_remove({}, {})", o, arg_strs.join(", ")),
            "join" => format!("lv_join({}, {})", o, arg_strs.get(0).cloned().unwrap_or("std::string(\"\")".to_string())),
            "reverse" => format!("lv_reverse({})", o),
            "keys" => format!("lv_keys({})", o),
            "values" => format!("lv_values({})", o),
            "has" => format!("({}.count({}) > 0)", o, arg_strs.join(", ")),
            _ => return None,
        };
        Some(result)
    }
}
