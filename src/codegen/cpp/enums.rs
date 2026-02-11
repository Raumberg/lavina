use crate::parser::ast::EnumVariant;
use super::CppCodegen;

impl CppCodegen {
    pub(super) fn emit_enum(&mut self, name: &crate::lexer::token::Token, variants: &[EnumVariant]) {
        let enum_name = &name.lexeme;

        // Register for match codegen
        self.known_enums.insert(enum_name.clone(), variants.to_vec());

        // Check if any variant has self-referential fields
        let has_self_ref = variants.iter().any(|v| {
            v.fields.iter().any(|(_, ty)| {
                matches!(ty, crate::parser::ast::Type::Custom(n) if n == enum_name)
            })
        });

        // Forward declaration if self-referential
        if has_self_ref {
            self.output.push_str(&format!("{}struct {};\n", self.indent(), enum_name));
        }

        self.output.push_str(&format!("{}struct {} {{\n", self.indent(), enum_name));
        self.indent_level += 1;

        // Inner structs for each variant with fields
        for v in variants {
            if !v.fields.is_empty() {
                self.output.push_str(&format!("{}struct {} {{ ", self.indent(), v.name.lexeme));
                for (fname, ftype) in &v.fields {
                    let cpp_type = self.emit_type(ftype);
                    // Self-referential fields use shared_ptr
                    let actual_type = if matches!(ftype, crate::parser::ast::Type::Custom(n) if n == enum_name) {
                        format!("std::shared_ptr<{}>", enum_name)
                    } else {
                        cpp_type
                    };
                    self.output.push_str(&format!("{} {}; ", actual_type, fname.lexeme));
                }
                self.output.push_str("};\n");
            }
        }
        self.output.push('\n');

        // Tag field
        self.output.push_str(&format!("{}std::string _tag;\n", self.indent()));

        // Variant data types for std::variant
        let variant_inner_types: Vec<String> = variants
            .iter()
            .filter(|v| !v.fields.is_empty())
            .map(|v| format!("{}::{}", enum_name, v.name.lexeme))
            .collect();

        if !variant_inner_types.is_empty() {
            let all_types = std::iter::once("std::monostate".to_string())
                .chain(variant_inner_types.iter().cloned())
                .collect::<Vec<_>>()
                .join(", ");
            self.output.push_str(&format!(
                "{}std::variant<{}> _data;\n",
                self.indent(),
                all_types
            ));
        }
        self.output.push('\n');

        // Static factory methods
        for v in variants {
            let vname = &v.name.lexeme;
            if v.fields.is_empty() {
                self.output.push_str(&format!(
                    "{}static {} make_{}() {{ return {{\"{}\"{}}}; }}\n",
                    self.indent(),
                    enum_name,
                    vname,
                    vname,
                    if variant_inner_types.is_empty() { "" } else { ", std::monostate{}" }
                ));
            } else {
                let params: Vec<String> = v.fields.iter().map(|(fname, ftype)| {
                    let cpp_type = self.emit_type(ftype);
                    if matches!(ftype, crate::parser::ast::Type::Custom(n) if n == enum_name) {
                        format!("{} {}", enum_name, fname.lexeme)
                    } else {
                        format!("{} {}", cpp_type, fname.lexeme)
                    }
                }).collect();

                let field_inits: Vec<String> = v.fields.iter().map(|(fname, ftype)| {
                    if matches!(ftype, crate::parser::ast::Type::Custom(n) if n == enum_name) {
                        format!("std::make_shared<{}>(std::move({}))", enum_name, fname.lexeme)
                    } else {
                        fname.lexeme.clone()
                    }
                }).collect();

                self.output.push_str(&format!(
                    "{}static {} make_{}({}) {{ return {{\"{}\", {}{{{}}}}}; }}\n",
                    self.indent(),
                    enum_name,
                    vname,
                    params.join(", "),
                    vname,
                    vname,
                    field_inits.join(", "),
                ));
            }
        }
        self.output.push('\n');

        // operator[] for backward compat (tag-based access)
        self.output.push_str(&format!(
            "{}std::string operator[](const std::string& key) const {{\n",
            self.indent()
        ));
        self.indent_level += 1;
        self.output.push_str(&format!(
            "{}if (key == \"_tag\") return _tag;\n",
            self.indent()
        ));

        // For old-style single-field enums, support ["0"] access
        if !variant_inner_types.is_empty() {
            self.output.push_str(&format!(
                "{}if (key == \"0\") {{\n",
                self.indent()
            ));
            self.indent_level += 1;
            self.output.push_str(&format!(
                "{}return std::visit([](auto&& arg) -> std::string {{\n",
                self.indent()
            ));
            self.indent_level += 1;
            self.output.push_str(&format!(
                "{}using T = std::decay_t<decltype(arg)>;\n",
                self.indent()
            ));
            self.output.push_str(&format!(
                "{}if constexpr (std::is_same_v<T, std::monostate>) return \"\";\n",
                self.indent()
            ));
            self.output.push_str(&format!(
                "{}else return \"<variant>\";\n",
                self.indent()
            ));
            self.indent_level -= 1;
            self.output.push_str(&format!("{}}}, _data);\n", self.indent()));
            self.indent_level -= 1;
            self.output.push_str(&format!("{}}}\n", self.indent()));
        }

        self.output.push_str(&format!(
            "{}return \"\";\n",
            self.indent()
        ));
        self.indent_level -= 1;
        self.output.push_str(&format!("{}}}\n", self.indent()));

        self.indent_level -= 1;
        self.output.push_str(&format!("{}}};\n\n", self.indent()));

        // print overload
        self.output.push_str(&format!(
            "void print(const {}& e) {{ std::cout << \"{}(\" << e._tag << \")\" << std::endl; }}\n",
            enum_name, enum_name
        ));
        // string concatenation
        self.output.push_str(&format!(
            "std::string operator+(const std::string& s, const {}& e) {{ return s + e._tag; }}\n",
            enum_name
        ));
        self.output.push_str(&format!(
            "std::string operator+(const {}& e, const std::string& s) {{ return e._tag + s; }}\n\n",
            enum_name
        ));
    }
}
