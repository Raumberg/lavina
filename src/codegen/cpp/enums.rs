use crate::parser::ast::EnumVariant;
use super::CppCodegen;

impl CppCodegen {
    pub(super) fn emit_enum(&mut self, name: &crate::lexer::token::Token, variants: &[EnumVariant]) {
        let enum_name = &name.lexeme;

        // Collect all unique data types
        let mut variant_types: Vec<String> = Vec::new();
        for v in variants {
            if !v.types.is_empty() {
                let t = self.emit_type(&v.types[0]);
                if !variant_types.contains(&t) {
                    variant_types.push(t);
                }
            }
        }

        self.output.push_str(&format!("{}struct {} {{\n", self.indent(), enum_name));
        self.indent_level += 1;

        // Tag field
        self.output.push_str(&format!("{}std::string _tag;\n", self.indent()));

        // Data field
        if !variant_types.is_empty() {
            let all_types = std::iter::once("std::monostate".to_string())
                .chain(variant_types.iter().cloned())
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
            if v.types.is_empty() {
                self.output.push_str(&format!(
                    "{}static {} {}() {{ return {{\"{}\"{}}}; }}\n",
                    self.indent(),
                    enum_name,
                    vname,
                    vname,
                    if variant_types.is_empty() { "" } else { ", std::monostate{}" }
                ));
            } else {
                let data_type = self.emit_type(&v.types[0]);
                self.output.push_str(&format!(
                    "{}static {} {}({} value) {{ return {{\"{}\", value}}; }}\n",
                    self.indent(),
                    enum_name,
                    vname,
                    data_type,
                    vname,
                ));
            }
        }

        self.output.push('\n');

        // operator[] for tag-based access
        self.output.push_str(&format!(
            "{}std::string operator[](const std::string& key) const {{\n",
            self.indent()
        ));
        self.indent_level += 1;
        self.output.push_str(&format!(
            "{}if (key == \"_tag\") return _tag;\n",
            self.indent()
        ));

        if !variant_types.is_empty() {
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
                "{}else if constexpr (std::is_same_v<T, std::string>) return arg;\n",
                self.indent()
            ));
            self.output.push_str(&format!(
                "{}else return std::to_string(arg);\n",
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
