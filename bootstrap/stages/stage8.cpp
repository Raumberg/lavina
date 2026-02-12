// Stage 8: hashmap[K,V] type support
#include "lavina.h"

const std::string TK_LEFT_PAREN = std::string("LeftParen");
const std::string TK_RIGHT_PAREN = std::string("RightParen");
const std::string TK_LEFT_BRACKET = std::string("LeftBracket");
const std::string TK_RIGHT_BRACKET = std::string("RightBracket");
const std::string TK_LEFT_BRACE = std::string("LeftBrace");
const std::string TK_RIGHT_BRACE = std::string("RightBrace");
const std::string TK_COMMA = std::string("Comma");
const std::string TK_DOT = std::string("Dot");
const std::string TK_MINUS = std::string("Minus");
const std::string TK_PLUS = std::string("Plus");
const std::string TK_SEMICOLON = std::string("Semicolon");
const std::string TK_SLASH = std::string("Slash");
const std::string TK_STAR = std::string("Star");
const std::string TK_COLON = std::string("Colon");
const std::string TK_HASH = std::string("Hash");
const std::string TK_QUESTION = std::string("Question");
const std::string TK_PERCENT = std::string("Percent");
const std::string TK_BANG = std::string("Bang");
const std::string TK_BANG_EQUAL = std::string("BangEqual");
const std::string TK_EQUAL = std::string("Equal");
const std::string TK_EQUAL_EQUAL = std::string("EqualEqual");
const std::string TK_GREATER = std::string("Greater");
const std::string TK_GREATER_EQUAL = std::string("GreaterEqual");
const std::string TK_LESS = std::string("Less");
const std::string TK_LESS_EQUAL = std::string("LessEqual");
const std::string TK_ARROW = std::string("Arrow");
const std::string TK_DOUBLE_COLON = std::string("DoubleColon");
const std::string TK_IDENTIFIER = std::string("Identifier");
const std::string TK_STRING = std::string("String");
const std::string TK_INT = std::string("Int");
const std::string TK_FLOAT = std::string("Float");
const std::string TK_AND = std::string("And");
const std::string TK_AUTO = std::string("Auto");
const std::string TK_BOOL = std::string("Bool");
const std::string TK_COMPTIME = std::string("Comptime");
const std::string TK_DYNAMIC = std::string("Dynamic");
const std::string TK_ELSE = std::string("Else");
const std::string TK_ELIF = std::string("Elif");
const std::string TK_FALSE = std::string("False");
const std::string TK_FLOAT_TYPE = std::string("FloatType");
const std::string TK_FN = std::string("Fn");
const std::string TK_IF = std::string("If");
const std::string TK_INLINE = std::string("Inline");
const std::string TK_INT_TYPE = std::string("IntType");
const std::string TK_NULL = std::string("Null");
const std::string TK_OR = std::string("Or");
const std::string TK_RETURN = std::string("Return");
const std::string TK_STRING_TYPE = std::string("StringType");
const std::string TK_TRUE = std::string("True");
const std::string TK_VOID = std::string("Void");
const std::string TK_WHILE = std::string("While");
const std::string TK_FOR = std::string("For");
const std::string TK_IN = std::string("In");
const std::string TK_VECTOR = std::string("Vector");
const std::string TK_HASHMAP = std::string("HashMap");
const std::string TK_HASHSET = std::string("HashSet");
const std::string TK_IMPORT = std::string("Import");
const std::string TK_AS = std::string("As");
const std::string TK_NAMESPACE = std::string("Namespace");
const std::string TK_PUBLIC = std::string("Public");
const std::string TK_PRIVATE = std::string("Private");
const std::string TK_STATIC = std::string("Static");
const std::string TK_CLASS = std::string("Class");
const std::string TK_STRUCT = std::string("Struct");
const std::string TK_ENUM = std::string("Enum");
const std::string TK_THIS = std::string("This");
const std::string TK_TRY = std::string("Try");
const std::string TK_CATCH = std::string("Catch");
const std::string TK_THROW = std::string("Throw");
const std::string TK_CONST = std::string("Const");
const std::string TK_LET = std::string("Let");
const std::string TK_MATCH = std::string("Match");
const std::string TK_BREAK = std::string("Break");
const std::string TK_CONTINUE = std::string("Continue");
const std::string TK_NOT = std::string("Not");
const std::string TK_INDENT = std::string("Indent");
const std::string TK_DEDENT = std::string("Dedent");
const std::string TK_NEWLINE = std::string("Newline");
const std::string TK_EOF = std::string("Eof");
struct Token {
    std::string token_type;
    std::string lexeme;
    int64_t line;
    int64_t col;

    Token(std::string token_type, std::string lexeme, int64_t line, int64_t col)
        : token_type(token_type), lexeme(lexeme), line(line), col(col) {
    }

    std::string to_string() {
        return this->token_type + std::string("(") + this->lexeme + std::string(") at ") + this->line + std::string(":") + this->col;
    }

};

std::string lookup_keyword(auto w) {
    if (w == std::string("and")) {
        return TK_AND;
    }
     else {
        if (w == std::string("auto")) {
            return TK_AUTO;
        }
         else {
            if (w == std::string("bool")) {
                return TK_BOOL;
            }
             else {
                if (w == std::string("comptime")) {
                    return TK_COMPTIME;
                }
                 else {
                    if (w == std::string("dynamic")) {
                        return TK_DYNAMIC;
                    }
                     else {
                        if (w == std::string("else")) {
                            return TK_ELSE;
                        }
                         else {
                            if (w == std::string("elif")) {
                                return TK_ELIF;
                            }
                             else {
                                if (w == std::string("false")) {
                                    return TK_FALSE;
                                }
                                 else {
                                    if (w == std::string("float")) {
                                        return TK_FLOAT_TYPE;
                                    }
                                     else {
                                        if (w == std::string("fn")) {
                                            return TK_FN;
                                        }
                                         else {
                                            if (w == std::string("if")) {
                                                return TK_IF;
                                            }
                                             else {
                                                if (w == std::string("inline")) {
                                                    return TK_INLINE;
                                                }
                                                 else {
                                                    if (w == std::string("int")) {
                                                        return TK_INT_TYPE;
                                                    }
                                                     else {
                                                        if (w == std::string("null")) {
                                                            return TK_NULL;
                                                        }
                                                         else {
                                                            if (w == std::string("or")) {
                                                                return TK_OR;
                                                            }
                                                             else {
                                                                if (w == std::string("return")) {
                                                                    return TK_RETURN;
                                                                }
                                                                 else {
                                                                    if (w == std::string("string")) {
                                                                        return TK_STRING_TYPE;
                                                                    }
                                                                     else {
                                                                        if (w == std::string("true")) {
                                                                            return TK_TRUE;
                                                                        }
                                                                         else {
                                                                            if (w == std::string("void")) {
                                                                                return TK_VOID;
                                                                            }
                                                                             else {
                                                                                if (w == std::string("while")) {
                                                                                    return TK_WHILE;
                                                                                }
                                                                                 else {
                                                                                    if (w == std::string("for")) {
                                                                                        return TK_FOR;
                                                                                    }
                                                                                     else {
                                                                                        if (w == std::string("in")) {
                                                                                            return TK_IN;
                                                                                        }
                                                                                         else {
                                                                                            if (w == std::string("vector")) {
                                                                                                return TK_VECTOR;
                                                                                            }
                                                                                             else {
                                                                                                if (w == std::string("hashmap")) {
                                                                                                    return TK_HASHMAP;
                                                                                                }
                                                                                                 else {
                                                                                                    if (w == std::string("hashset")) {
                                                                                                        return TK_HASHSET;
                                                                                                    }
                                                                                                     else {
                                                                                                        if (w == std::string("import")) {
                                                                                                            return TK_IMPORT;
                                                                                                        }
                                                                                                         else {
                                                                                                            if (w == std::string("as")) {
                                                                                                                return TK_AS;
                                                                                                            }
                                                                                                             else {
                                                                                                                if (w == std::string("namespace")) {
                                                                                                                    return TK_NAMESPACE;
                                                                                                                }
                                                                                                                 else {
                                                                                                                    if (w == std::string("public")) {
                                                                                                                        return TK_PUBLIC;
                                                                                                                    }
                                                                                                                     else {
                                                                                                                        if (w == std::string("private")) {
                                                                                                                            return TK_PRIVATE;
                                                                                                                        }
                                                                                                                         else {
                                                                                                                            if (w == std::string("static")) {
                                                                                                                                return TK_STATIC;
                                                                                                                            }
                                                                                                                             else {
                                                                                                                                if (w == std::string("class")) {
                                                                                                                                    return TK_CLASS;
                                                                                                                                }
                                                                                                                                 else {
                                                                                                                                    if (w == std::string("struct")) {
                                                                                                                                        return TK_STRUCT;
                                                                                                                                    }
                                                                                                                                     else {
                                                                                                                                        if (w == std::string("enum")) {
                                                                                                                                            return TK_ENUM;
                                                                                                                                        }
                                                                                                                                         else {
                                                                                                                                            if (w == std::string("this")) {
                                                                                                                                                return TK_THIS;
                                                                                                                                            }
                                                                                                                                             else {
                                                                                                                                                if (w == std::string("try")) {
                                                                                                                                                    return TK_TRY;
                                                                                                                                                }
                                                                                                                                                 else {
                                                                                                                                                    if (w == std::string("catch")) {
                                                                                                                                                        return TK_CATCH;
                                                                                                                                                    }
                                                                                                                                                     else {
                                                                                                                                                        if (w == std::string("throw")) {
                                                                                                                                                            return TK_THROW;
                                                                                                                                                        }
                                                                                                                                                         else {
                                                                                                                                                            if (w == std::string("const")) {
                                                                                                                                                                return TK_CONST;
                                                                                                                                                            }
                                                                                                                                                             else {
                                                                                                                                                                if (w == std::string("let")) {
                                                                                                                                                                    return TK_LET;
                                                                                                                                                                }
                                                                                                                                                                 else {
                                                                                                                                                                    if (w == std::string("match")) {
                                                                                                                                                                        return TK_MATCH;
                                                                                                                                                                    }
                                                                                                                                                                     else {
                                                                                                                                                                        if (w == std::string("break")) {
                                                                                                                                                                            return TK_BREAK;
                                                                                                                                                                        }
                                                                                                                                                                         else {
                                                                                                                                                                            if (w == std::string("continue")) {
                                                                                                                                                                                return TK_CONTINUE;
                                                                                                                                                                            }
                                                                                                                                                                             else {
                                                                                                                                                                                if (w == std::string("not")) {
                                                                                                                                                                                    return TK_NOT;
                                                                                                                                                                                }
                                                                                                                                                                            }
                                                                                                                                                                        }
                                                                                                                                                                    }
                                                                                                                                                                }
                                                                                                                                                            }
                                                                                                                                                        }
                                                                                                                                                    }
                                                                                                                                                }
                                                                                                                                            }
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return std::string("");
}

bool is_alpha(auto c) {
    if (c >= std::string("a") && c <= std::string("z")) {
        return true;
    }
     else {
        if (c >= std::string("A") && c <= std::string("Z")) {
            return true;
        }
         else {
            if (c == std::string("_")) {
                return true;
            }
        }
    }
    return false;
}

bool is_digit(auto c) {
    if (c >= std::string("0") && c <= std::string("9")) {
        return true;
    }
    return false;
}

bool is_alnum(auto c) {
    if (is_alpha(c)) {
        return true;
    }
    if (is_digit(c)) {
        return true;
    }
    return false;
}

struct Scanner {
    std::string source;
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    int64_t start;
    int64_t current;
    int64_t line;
    int64_t column;
    int64_t start_line;
    int64_t start_column;
    std::vector<int64_t> indent_stack;
    bool at_line_start;
    bool in_string_interp;
    int64_t interp_brace_depth;

    Scanner(std::string source)
        : source(source) {
        this->tokens = {};
        this->errors = {};
        this->start = 0LL;
        this->current = 0LL;
        this->line = 1LL;
        this->column = 1LL;
        this->start_line = 1LL;
        this->start_column = 1LL;
        this->indent_stack = std::vector{0LL};
        this->at_line_start = true;
        this->in_string_interp = false;
        this->interp_brace_depth = 0LL;
    }

    bool is_at_end() {
        return this->current >= static_cast<int64_t>(this->source.size());
    }

    auto peek() {
        if ((*this).is_at_end()) {
            return std::string("");
        }
        return std::string(1, this->source[this->current]);
    }

    auto peek_next() {
        if (this->current + 1LL >= static_cast<int64_t>(this->source.size())) {
            return std::string("");
        }
        return std::string(1, this->source[this->current + 1LL]);
    }

    auto advance() {
        auto c = std::string(1, this->source[this->current]);
        this->current = this->current + 1LL;
        this->column = this->column + 1LL;
        return c;
    }

    bool match_char(std::string expected) {
        if ((*this).is_at_end()) {
            return false;
        }
        if (std::string(1, this->source[this->current]) != expected) {
            return false;
        }
        this->current = this->current + 1LL;
        this->column = this->column + 1LL;
        return true;
    }

    void add_token(std::string tk_type, std::string lexeme) {
        int64_t sl = this->start_line;
        int64_t sc = this->start_column;
        auto tok = Token(tk_type, lexeme, sl, sc);
        this->tokens.push_back(tok);
    }

    void add_simple_token(std::string tk_type) {
        auto lexeme = this->source.substr(this->start, (this->current) - (this->start));
        (*this).add_token(tk_type, lexeme);
    }

    void add_error(std::string message) {
        auto err = std::string("Error at line ") + this->line + std::string(":") + this->column + std::string(": ") + message;
        this->errors.push_back(err);
    }

    void handle_indentation() {
        int64_t indent = 0LL;
        while (!(*this).is_at_end() && ((*this).peek() == std::string(" ") || (*this).peek() == std::string("\t"))) {
            auto c = (*this).advance();
            if (c == std::string(" ")) {
                indent = indent + 1LL;
            }
             else {
                indent = indent + 4LL;
            }
        }
        if ((*this).is_at_end() || (*this).peek() == std::string("\n")) {
            if ((*this).peek() == std::string("\n")) {
                this->at_line_start = true;
            }
            return;
        }
        auto current_indent = this->indent_stack[static_cast<int64_t>(this->indent_stack.size()) - 1LL];
        if (indent > current_indent) {
            this->indent_stack.push_back(indent);
            (*this).add_token(TK_INDENT, std::string(""));
        }
         else {
            if (indent < current_indent) {
                while (indent < this->indent_stack[static_cast<int64_t>(this->indent_stack.size()) - 1LL]) {
                    lv_pop(this->indent_stack);
                    (*this).add_token(TK_DEDENT, std::string(""));
                }
            }
        }
        this->at_line_start = false;
    }

    void scan_string() {
        int64_t str_start = this->current;
        while (!(*this).is_at_end() && (*this).peek() != std::string("\"")) {
            if ((*this).peek() == std::string("\\")) {
                (*this).advance();
                (*this).advance();
            }
             else {
                if ((*this).peek() == std::string("$") && (*this).peek_next() == std::string("{")) {
                    auto value = this->source.substr(str_start, (this->current) - (str_start));
                    (*this).add_token(TK_STRING, value);
                    (*this).add_token(TK_PLUS, std::string("+"));
                    (*this).add_token(TK_LEFT_PAREN, std::string("("));
                    (*this).advance();
                    (*this).advance();
                    this->in_string_interp = true;
                    this->interp_brace_depth = 1LL;
                    return;
                }
                 else {
                    if ((*this).peek() == std::string("\n")) {
                        this->line = this->line + 1LL;
                        this->column = 1LL;
                    }
                    (*this).advance();
                }
            }
        }
        if ((*this).is_at_end()) {
            (*this).add_error(std::string("Unterminated string"));
            return;
        }
        auto value = this->source.substr(str_start, (this->current) - (str_start));
        (*this).advance();
        (*this).add_token(TK_STRING, value);
    }

    void scan_string_rest() {
        int64_t str_start = this->current;
        while (!(*this).is_at_end() && (*this).peek() != std::string("\"")) {
            if ((*this).peek() == std::string("\\")) {
                (*this).advance();
                (*this).advance();
            }
             else {
                if ((*this).peek() == std::string("$") && (*this).peek_next() == std::string("{")) {
                    auto value = this->source.substr(str_start, (this->current) - (str_start));
                    (*this).add_token(TK_STRING, value);
                    (*this).add_token(TK_PLUS, std::string("+"));
                    (*this).add_token(TK_LEFT_PAREN, std::string("("));
                    (*this).advance();
                    (*this).advance();
                    this->in_string_interp = true;
                    this->interp_brace_depth = 1LL;
                    return;
                }
                 else {
                    if ((*this).peek() == std::string("\n")) {
                        this->line = this->line + 1LL;
                        this->column = 1LL;
                    }
                    (*this).advance();
                }
            }
        }
        if ((*this).is_at_end()) {
            (*this).add_error(std::string("Unterminated string"));
            return;
        }
        auto value = this->source.substr(str_start, (this->current) - (str_start));
        (*this).advance();
        (*this).add_token(TK_STRING, value);
    }

    void scan_number() {
        while (is_digit((*this).peek())) {
            (*this).advance();
        }
        if ((*this).peek() == std::string(".") && is_digit((*this).peek_next())) {
            (*this).advance();
            while (is_digit((*this).peek())) {
                (*this).advance();
            }
            (*this).add_simple_token(TK_FLOAT);
        }
         else {
            (*this).add_simple_token(TK_INT);
        }
    }

    void scan_identifier() {
        while (is_alnum((*this).peek())) {
            (*this).advance();
        }
        auto text = this->source.substr(this->start, (this->current) - (this->start));
        auto kw = lookup_keyword(text);
        if (kw != std::string("")) {
            (*this).add_token(kw, text);
        }
         else {
            (*this).add_token(TK_IDENTIFIER, text);
        }
    }

    void scan_token() {
        if (this->at_line_start) {
            (*this).handle_indentation();
            this->start = this->current;
            this->start_line = this->line;
            this->start_column = this->column;
        }
        if ((*this).is_at_end()) {
            return;
        }
        auto c = (*this).advance();
        if (c == std::string("(")) {
            (*this).add_simple_token(TK_LEFT_PAREN);
        }
         else {
            if (c == std::string(")")) {
                (*this).add_simple_token(TK_RIGHT_PAREN);
            }
             else {
                if (c == std::string("[")) {
                    (*this).add_simple_token(TK_LEFT_BRACKET);
                }
                 else {
                    if (c == std::string("]")) {
                        (*this).add_simple_token(TK_RIGHT_BRACKET);
                    }
                     else {
                        if (c == std::string("{")) {
                            if (this->in_string_interp) {
                                this->interp_brace_depth = this->interp_brace_depth + 1LL;
                            }
                            (*this).add_simple_token(TK_LEFT_BRACE);
                        }
                         else {
                            if (c == std::string("}")) {
                                if (this->in_string_interp) {
                                    this->interp_brace_depth = this->interp_brace_depth - 1LL;
                                    if (this->interp_brace_depth == 0LL) {
                                        this->in_string_interp = false;
                                        (*this).add_token(TK_RIGHT_PAREN, std::string(")"));
                                        (*this).add_token(TK_PLUS, std::string("+"));
                                        (*this).scan_string_rest();
                                        return;
                                    }
                                }
                                (*this).add_simple_token(TK_RIGHT_BRACE);
                            }
                             else {
                                if (c == std::string(",")) {
                                    (*this).add_simple_token(TK_COMMA);
                                }
                                 else {
                                    if (c == std::string(".")) {
                                        (*this).add_simple_token(TK_DOT);
                                    }
                                     else {
                                        if (c == std::string("?")) {
                                            (*this).add_simple_token(TK_QUESTION);
                                        }
                                         else {
                                            if (c == std::string("+")) {
                                                (*this).add_simple_token(TK_PLUS);
                                            }
                                             else {
                                                if (c == std::string(";")) {
                                                    (*this).add_simple_token(TK_SEMICOLON);
                                                }
                                                 else {
                                                    if (c == std::string("*")) {
                                                        (*this).add_simple_token(TK_STAR);
                                                    }
                                                     else {
                                                        if (c == std::string("%")) {
                                                            (*this).add_simple_token(TK_PERCENT);
                                                        }
                                                         else {
                                                            if (c == std::string("-")) {
                                                                if ((*this).match_char(std::string(">"))) {
                                                                    (*this).add_simple_token(TK_ARROW);
                                                                }
                                                                 else {
                                                                    (*this).add_simple_token(TK_MINUS);
                                                                }
                                                            }
                                                             else {
                                                                if (c == std::string(":")) {
                                                                    if ((*this).match_char(std::string(":"))) {
                                                                        (*this).add_simple_token(TK_DOUBLE_COLON);
                                                                    }
                                                                     else {
                                                                        (*this).add_simple_token(TK_COLON);
                                                                    }
                                                                }
                                                                 else {
                                                                    if (c == std::string("#")) {
                                                                        (*this).add_simple_token(TK_HASH);
                                                                    }
                                                                     else {
                                                                        if (c == std::string("!")) {
                                                                            if ((*this).match_char(std::string("="))) {
                                                                                (*this).add_simple_token(TK_BANG_EQUAL);
                                                                            }
                                                                             else {
                                                                                (*this).add_simple_token(TK_BANG);
                                                                            }
                                                                        }
                                                                         else {
                                                                            if (c == std::string("=")) {
                                                                                if ((*this).match_char(std::string("="))) {
                                                                                    (*this).add_simple_token(TK_EQUAL_EQUAL);
                                                                                }
                                                                                 else {
                                                                                    (*this).add_simple_token(TK_EQUAL);
                                                                                }
                                                                            }
                                                                             else {
                                                                                if (c == std::string("<")) {
                                                                                    if ((*this).match_char(std::string("="))) {
                                                                                        (*this).add_simple_token(TK_LESS_EQUAL);
                                                                                    }
                                                                                     else {
                                                                                        (*this).add_simple_token(TK_LESS);
                                                                                    }
                                                                                }
                                                                                 else {
                                                                                    if (c == std::string(">")) {
                                                                                        if ((*this).match_char(std::string("="))) {
                                                                                            (*this).add_simple_token(TK_GREATER_EQUAL);
                                                                                        }
                                                                                         else {
                                                                                            (*this).add_simple_token(TK_GREATER);
                                                                                        }
                                                                                    }
                                                                                     else {
                                                                                        if (c == std::string("/")) {
                                                                                            if ((*this).match_char(std::string("/"))) {
                                                                                                while ((*this).peek() != std::string("\n") && !(*this).is_at_end()) {
                                                                                                    (*this).advance();
                                                                                                }
                                                                                            }
                                                                                             else {
                                                                                                (*this).add_simple_token(TK_SLASH);
                                                                                            }
                                                                                        }
                                                                                         else {
                                                                                            if (c == std::string(" ") || c == std::string("\r") || c == std::string("\t")) {
                                                                                                int64_t noop = 0LL;
                                                                                            }
                                                                                             else {
                                                                                                if (c == std::string("\n")) {
                                                                                                    (*this).add_token(TK_NEWLINE, std::string("\n"));
                                                                                                    this->line = this->line + 1LL;
                                                                                                    this->column = 1LL;
                                                                                                    this->at_line_start = true;
                                                                                                }
                                                                                                 else {
                                                                                                    if (c == std::string("\"")) {
                                                                                                        (*this).scan_string();
                                                                                                    }
                                                                                                     else {
                                                                                                        if (is_digit(c)) {
                                                                                                            (*this).scan_number();
                                                                                                        }
                                                                                                         else {
                                                                                                            if (is_alpha(c)) {
                                                                                                                (*this).scan_identifier();
                                                                                                            }
                                                                                                             else {
                                                                                                                (*this).add_error(std::string("Unexpected character: ") + c);
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void scan_tokens() {
        while (!(*this).is_at_end()) {
            this->start = this->current;
            this->start_line = this->line;
            this->start_column = this->column;
            (*this).scan_token();
        }
        while (static_cast<int64_t>(this->indent_stack.size()) > 1LL) {
            lv_pop(this->indent_stack);
            (*this).add_token(TK_DEDENT, std::string(""));
        }
        (*this).add_token(TK_EOF, std::string(""));
    }

};

void test_scanner() {
    auto test_source = std::string("int fn add(int a, int b):\n    return a + b\n\nprint(add(2, 3))\n");
    auto scanner = Scanner(test_source);
    scanner.scan_tokens();
    print(std::string("Tokens: ") + static_cast<int64_t>(scanner.tokens.size()));
    if (static_cast<int64_t>(scanner.errors.size()) > 0LL) {
        print(std::string("ERRORS:"));
        for (auto& err : scanner.errors) {
            print(err);
        }
    }
     else {
        for (auto& tok : scanner.tokens) {
            print(tok.to_string());
        }
    }
}

struct TypeNode;
struct TypeNode {
    struct Custom { std::string name; };
    struct Array { std::shared_ptr<TypeNode> inner; };
    struct HashSet { std::shared_ptr<TypeNode> inner; };
    struct Dict { std::shared_ptr<TypeNode> key_type; std::shared_ptr<TypeNode> value_type; };
    struct Nullable { std::shared_ptr<TypeNode> inner; };

    std::string _tag;
    std::variant<std::monostate, TypeNode::Custom, TypeNode::Array, TypeNode::HashSet, TypeNode::Dict, TypeNode::Nullable> _data;

    static TypeNode make_None() { return {"None", std::monostate{}}; }
    static TypeNode make_Int() { return {"Int", std::monostate{}}; }
    static TypeNode make_Float() { return {"Float", std::monostate{}}; }
    static TypeNode make_Str() { return {"Str", std::monostate{}}; }
    static TypeNode make_Bool() { return {"Bool", std::monostate{}}; }
    static TypeNode make_Void() { return {"Void", std::monostate{}}; }
    static TypeNode make_Auto() { return {"Auto", std::monostate{}}; }
    static TypeNode make_Dynamic() { return {"Dynamic", std::monostate{}}; }
    static TypeNode make_NullType() { return {"NullType", std::monostate{}}; }
    static TypeNode make_Custom(std::string name) { return {"Custom", Custom{name}}; }
    static TypeNode make_Array(TypeNode inner) { return {"Array", Array{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_HashSet(TypeNode inner) { return {"HashSet", HashSet{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_Dict(TypeNode key_type, TypeNode value_type) { return {"Dict", Dict{std::make_shared<TypeNode>(std::move(key_type)), std::make_shared<TypeNode>(std::move(value_type))}}; }
    static TypeNode make_Nullable(TypeNode inner) { return {"Nullable", Nullable{std::make_shared<TypeNode>(std::move(inner))}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const TypeNode& e) { std::cout << "TypeNode(" << e._tag << ")" << std::endl; }
std::string operator+(const std::string& s, const TypeNode& e) { return s + e._tag; }
std::string operator+(const TypeNode& e, const std::string& s) { return e._tag + s; }

struct Param {
    Token name;
    TypeNode param_type;

    Param(Token name, TypeNode param_type)
        : name(name), param_type(param_type) {
    }

};

struct EnumVariantNode {
    Token name;
    std::vector<TypeNode> types;
    std::vector<std::string> field_names;

    EnumVariantNode(Token name, std::vector<TypeNode> types, std::vector<std::string> field_names)
        : name(name), types(types), field_names(field_names) {
    }

};

struct Expr;
struct Expr {
    struct Literal { std::string kind; std::string value; };
    struct Unary { Token op; std::shared_ptr<Expr> right; };
    struct Binary { std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right; };
    struct Grouping { std::shared_ptr<Expr> inner; };
    struct Variable { Token name; };
    struct Assign { Token name; std::shared_ptr<Expr> value; };
    struct Logical { std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right; };
    struct Call { std::shared_ptr<Expr> callee; Token paren; std::vector<Expr> args; };
    struct Index { std::shared_ptr<Expr> object; Token bracket; std::shared_ptr<Expr> index; };
    struct Vector { std::vector<Expr> elements; };
    struct Map { std::vector<Expr> keys; std::vector<Expr> values; };
    struct Get { std::shared_ptr<Expr> object; Token name; };
    struct Set { std::shared_ptr<Expr> object; Token name; std::shared_ptr<Expr> value; };
    struct StaticGet { std::shared_ptr<Expr> object; Token name; };
    struct This { Token keyword; };
    struct Cast { std::shared_ptr<Expr> expr; TypeNode target_type; };
    struct Throw { std::shared_ptr<Expr> expr; };

    std::string _tag;
    std::variant<std::monostate, Expr::Literal, Expr::Unary, Expr::Binary, Expr::Grouping, Expr::Variable, Expr::Assign, Expr::Logical, Expr::Call, Expr::Index, Expr::Vector, Expr::Map, Expr::Get, Expr::Set, Expr::StaticGet, Expr::This, Expr::Cast, Expr::Throw> _data;

    static Expr make_None() { return {"None", std::monostate{}}; }
    static Expr make_Literal(std::string kind, std::string value) { return {"Literal", Literal{kind, value}}; }
    static Expr make_Unary(Token op, Expr right) { return {"Unary", Unary{op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Binary(Expr left, Token op, Expr right) { return {"Binary", Binary{std::make_shared<Expr>(std::move(left)), op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Grouping(Expr inner) { return {"Grouping", Grouping{std::make_shared<Expr>(std::move(inner))}}; }
    static Expr make_Variable(Token name) { return {"Variable", Variable{name}}; }
    static Expr make_Assign(Token name, Expr value) { return {"Assign", Assign{name, std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_Logical(Expr left, Token op, Expr right) { return {"Logical", Logical{std::make_shared<Expr>(std::move(left)), op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Call(Expr callee, Token paren, std::vector<Expr> args) { return {"Call", Call{std::make_shared<Expr>(std::move(callee)), paren, args}}; }
    static Expr make_Index(Expr object, Token bracket, Expr index) { return {"Index", Index{std::make_shared<Expr>(std::move(object)), bracket, std::make_shared<Expr>(std::move(index))}}; }
    static Expr make_Vector(std::vector<Expr> elements) { return {"Vector", Vector{elements}}; }
    static Expr make_Map(std::vector<Expr> keys, std::vector<Expr> values) { return {"Map", Map{keys, values}}; }
    static Expr make_Get(Expr object, Token name) { return {"Get", Get{std::make_shared<Expr>(std::move(object)), name}}; }
    static Expr make_Set(Expr object, Token name, Expr value) { return {"Set", Set{std::make_shared<Expr>(std::move(object)), name, std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_StaticGet(Expr object, Token name) { return {"StaticGet", StaticGet{std::make_shared<Expr>(std::move(object)), name}}; }
    static Expr make_This(Token keyword) { return {"This", This{keyword}}; }
    static Expr make_Cast(Expr expr, TypeNode target_type) { return {"Cast", Cast{std::make_shared<Expr>(std::move(expr)), target_type}}; }
    static Expr make_Throw(Expr expr) { return {"Throw", Throw{std::make_shared<Expr>(std::move(expr))}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const Expr& e) { std::cout << "Expr(" << e._tag << ")" << std::endl; }
std::string operator+(const std::string& s, const Expr& e) { return s + e._tag; }
std::string operator+(const Expr& e, const std::string& s) { return e._tag + s; }

struct MatchArm {
    std::string pattern_name;
    std::vector<std::string> bindings;

    MatchArm(std::string pattern_name, std::vector<std::string> bindings)
        : pattern_name(pattern_name), bindings(bindings) {
    }

};

struct Stmt;
struct Stmt {
    struct ExprStmt { Expr expr; };
    struct Let { Token name; TypeNode var_type; Expr initializer; std::string visibility; };
    struct Const { Token name; TypeNode const_type; Expr value; std::string visibility; };
    struct Return { Token keyword; Expr value; };
    struct If { Expr condition; std::shared_ptr<Stmt> then_branch; std::shared_ptr<Stmt> else_branch; };
    struct While { Expr condition; std::shared_ptr<Stmt> body; };
    struct For { Token item_name; Expr collection; std::shared_ptr<Stmt> body; };
    struct Block { std::vector<Stmt> statements; };
    struct Try { std::shared_ptr<Stmt> try_body; std::shared_ptr<Stmt> catch_body; std::string exception_name; };
    struct Function { Token name; std::vector<Param> params; TypeNode return_type; std::vector<Stmt> body; bool is_inline; bool is_comptime; bool is_static; std::string visibility; };
    struct Class { Token name; std::vector<Stmt> body; std::string visibility; };
    struct Struct { Token name; std::vector<Stmt> body; std::string visibility; };
    struct Enum { Token name; std::vector<EnumVariantNode> variants; std::string visibility; };
    struct Match { Expr expr; std::vector<MatchArm> arm_patterns; std::vector<Stmt> arm_bodies; };
    struct Namespace { Token name; std::vector<Stmt> body; std::string visibility; };
    struct Import { std::vector<Token> path; std::string alias; };
    struct Break { Token keyword; };
    struct Continue { Token keyword; };

    std::string _tag;
    std::variant<std::monostate, Stmt::ExprStmt, Stmt::Let, Stmt::Const, Stmt::Return, Stmt::If, Stmt::While, Stmt::For, Stmt::Block, Stmt::Try, Stmt::Function, Stmt::Class, Stmt::Struct, Stmt::Enum, Stmt::Match, Stmt::Namespace, Stmt::Import, Stmt::Break, Stmt::Continue> _data;

    static Stmt make_None() { return {"None", std::monostate{}}; }
    static Stmt make_ExprStmt(Expr expr) { return {"ExprStmt", ExprStmt{expr}}; }
    static Stmt make_Let(Token name, TypeNode var_type, Expr initializer, std::string visibility) { return {"Let", Let{name, var_type, initializer, visibility}}; }
    static Stmt make_Const(Token name, TypeNode const_type, Expr value, std::string visibility) { return {"Const", Const{name, const_type, value, visibility}}; }
    static Stmt make_Return(Token keyword, Expr value) { return {"Return", Return{keyword, value}}; }
    static Stmt make_If(Expr condition, Stmt then_branch, Stmt else_branch) { return {"If", If{condition, std::make_shared<Stmt>(std::move(then_branch)), std::make_shared<Stmt>(std::move(else_branch))}}; }
    static Stmt make_While(Expr condition, Stmt body) { return {"While", While{condition, std::make_shared<Stmt>(std::move(body))}}; }
    static Stmt make_For(Token item_name, Expr collection, Stmt body) { return {"For", For{item_name, collection, std::make_shared<Stmt>(std::move(body))}}; }
    static Stmt make_Block(std::vector<Stmt> statements) { return {"Block", Block{statements}}; }
    static Stmt make_Try(Stmt try_body, Stmt catch_body, std::string exception_name) { return {"Try", Try{std::make_shared<Stmt>(std::move(try_body)), std::make_shared<Stmt>(std::move(catch_body)), exception_name}}; }
    static Stmt make_Function(Token name, std::vector<Param> params, TypeNode return_type, std::vector<Stmt> body, bool is_inline, bool is_comptime, bool is_static, std::string visibility) { return {"Function", Function{name, params, return_type, body, is_inline, is_comptime, is_static, visibility}}; }
    static Stmt make_Class(Token name, std::vector<Stmt> body, std::string visibility) { return {"Class", Class{name, body, visibility}}; }
    static Stmt make_Struct(Token name, std::vector<Stmt> body, std::string visibility) { return {"Struct", Struct{name, body, visibility}}; }
    static Stmt make_Enum(Token name, std::vector<EnumVariantNode> variants, std::string visibility) { return {"Enum", Enum{name, variants, visibility}}; }
    static Stmt make_Match(Expr expr, std::vector<MatchArm> arm_patterns, std::vector<Stmt> arm_bodies) { return {"Match", Match{expr, arm_patterns, arm_bodies}}; }
    static Stmt make_Namespace(Token name, std::vector<Stmt> body, std::string visibility) { return {"Namespace", Namespace{name, body, visibility}}; }
    static Stmt make_Import(std::vector<Token> path, std::string alias) { return {"Import", Import{path, alias}}; }
    static Stmt make_Break(Token keyword) { return {"Break", Break{keyword}}; }
    static Stmt make_Continue(Token keyword) { return {"Continue", Continue{keyword}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const Stmt& e) { std::cout << "Stmt(" << e._tag << ")" << std::endl; }
std::string operator+(const std::string& s, const Stmt& e) { return s + e._tag; }
std::string operator+(const Stmt& e, const std::string& s) { return e._tag + s; }

struct Parser {
    std::vector<Token> tokens;
    int64_t current;
    bool in_class_body;

    Parser(std::vector<Token> tokens)
        : tokens(tokens) {
        this->current = 0LL;
        this->in_class_body = false;
    }

    bool is_at_end() {
        return (*this).peek().token_type == TK_EOF;
    }

    Token peek() {
        return this->tokens[this->current];
    }

    Token peek_at(int64_t offset) {
        if (this->current + offset >= static_cast<int64_t>(this->tokens.size())) {
            return this->tokens[static_cast<int64_t>(this->tokens.size()) - 1LL];
        }
        return this->tokens[this->current + offset];
    }

    Token previous() {
        return this->tokens[this->current - 1LL];
    }

    Token advance() {
        if (!(*this).is_at_end()) {
            this->current = this->current + 1LL;
        }
        return (*this).previous();
    }

    Token consume(std::string tk_type, std::string message) {
        if ((*this).check(tk_type)) {
            return (*this).advance();
        }
        auto t = (*this).peek();
        throw std::runtime_error(message + std::string(" Got ") + t.token_type + std::string(" at ") + t.line + std::string(":") + t.col);
    }

    bool check(std::string tk_type) {
        if ((*this).is_at_end()) {
            return false;
        }
        return (*this).peek().token_type == tk_type;
    }

    bool match_any(std::vector<std::string> types) {
        for (auto& t : types) {
            if ((*this).check(t)) {
                (*this).advance();
                return true;
            }
        }
        return false;
    }

    void skip_formatting() {
        while ((*this).match_any(std::vector{TK_NEWLINE, TK_INDENT, TK_DEDENT})) {
            int64_t noop = 0LL;
        }
    }

    bool is_type_at_pos(int64_t pos) {
        auto t = (*this).peek_at(pos).token_type;
        if (t == TK_INT_TYPE) {
            return true;
        }
        if (t == TK_FLOAT_TYPE) {
            return true;
        }
        if (t == TK_STRING_TYPE) {
            return true;
        }
        if (t == TK_BOOL) {
            return true;
        }
        if (t == TK_VOID) {
            return true;
        }
        if (t == TK_AUTO) {
            return true;
        }
        if (t == TK_DYNAMIC) {
            return true;
        }
        if (t == TK_VECTOR) {
            return true;
        }
        if (t == TK_HASHMAP) {
            return true;
        }
        if (t == TK_HASHSET) {
            return true;
        }
        if (t == TK_IDENTIFIER) {
            return true;
        }
        return false;
    }

    bool check_function_start() {
        int64_t offset = 0LL;
        if (!(*this).is_type_at_pos(offset)) {
            return false;
        }
        auto tt = (*this).peek_at(offset).token_type;
        offset = offset + 1LL;
        if (tt == TK_VECTOR) {
            offset = offset + 3LL;
        }
         else {
            if (tt == TK_HASHMAP) {
                offset = offset + 5LL;
            }
             else {
                if (tt == TK_HASHSET) {
                    offset = offset + 3LL;
                }
            }
        }
        if ((*this).peek_at(offset).token_type == TK_QUESTION) {
            offset = offset + 1LL;
        }
        bool still_modifiers = true;
        while (offset < static_cast<int64_t>(this->tokens.size()) && still_modifiers) {
            auto mt = (*this).peek_at(offset).token_type;
            if (mt == TK_INLINE || mt == TK_COMPTIME) {
                offset = offset + 1LL;
            }
             else {
                still_modifiers = false;
            }
        }
        return (*this).peek_at(offset).token_type == TK_FN;
    }

    TypeNode parse_type() {
        TypeNode t = TypeNode::make_None();
        if ((*this).match_any(std::vector{TK_INT_TYPE})) {
            t = TypeNode::make_Int();
        }
         else {
            if ((*this).match_any(std::vector{TK_FLOAT_TYPE})) {
                t = TypeNode::make_Float();
            }
             else {
                if ((*this).match_any(std::vector{TK_STRING_TYPE})) {
                    t = TypeNode::make_Str();
                }
                 else {
                    if ((*this).match_any(std::vector{TK_BOOL})) {
                        t = TypeNode::make_Bool();
                    }
                     else {
                        if ((*this).match_any(std::vector{TK_VOID})) {
                            t = TypeNode::make_Void();
                        }
                         else {
                            if ((*this).match_any(std::vector{TK_AUTO})) {
                                t = TypeNode::make_Auto();
                            }
                             else {
                                if ((*this).match_any(std::vector{TK_DYNAMIC})) {
                                    t = TypeNode::make_Dynamic();
                                }
                                 else {
                                    if ((*this).match_any(std::vector{TK_NULL})) {
                                        t = TypeNode::make_NullType();
                                    }
                                     else {
                                        if ((*this).match_any(std::vector{TK_VECTOR})) {
                                            (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'vector'."));
                                            TypeNode inner = (*this).parse_type();
                                            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after vector type."));
                                            t = TypeNode::make_Array(inner);
                                        }
                                         else {
                                            if ((*this).match_any(std::vector{TK_HASHSET})) {
                                                (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'hashset'."));
                                                TypeNode inner = (*this).parse_type();
                                                (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after hashset type."));
                                                t = TypeNode::make_HashSet(inner);
                                            }
                                             else {
                                                if ((*this).match_any(std::vector{TK_HASHMAP})) {
                                                    (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'hashmap'."));
                                                    TypeNode k = (*this).parse_type();
                                                    (*this).consume(TK_COMMA, std::string("Expect ',' between key and value types."));
                                                    TypeNode v = (*this).parse_type();
                                                    (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after hashmap types."));
                                                    t = TypeNode::make_Dict(k, v);
                                                }
                                                 else {
                                                    if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
                                                        t = TypeNode::make_Custom((*this).previous().lexeme);
                                                    }
                                                     else {
                                                        auto tok = (*this).peek();
                                                        throw std::runtime_error(std::string("Expect type. Got ") + tok.token_type + std::string(" at ") + tok.line + std::string(":") + tok.col);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if ((*this).match_any(std::vector{TK_QUESTION})) {
            t = TypeNode::make_Nullable(t);
        }
        return t;
    }

    Expr expression() {
        return (*this).assignment();
    }

    Expr assignment() {
        Expr expr = (*this).logical_or();
        if ((*this).match_any(std::vector{TK_EQUAL})) {
            Expr value = (*this).assignment();
            {
                auto& _match_0 = expr;
                if (_match_0._tag == "Variable") {
                    auto& _v = std::get<Expr::Variable>(_match_0._data);
                    auto& name = _v.name;
                    return Expr::make_Assign(name, value);
                }
                else if (_match_0._tag == "Get") {
                    auto& _v = std::get<Expr::Get>(_match_0._data);
                    auto& object = *_v.object;
                    auto& name = _v.name;
                    return Expr::make_Set(object, name, value);
                }
                else {
                    throw std::runtime_error(std::string("Invalid assignment target."));
                }
            }
        }
        return expr;
    }

    Expr logical_or() {
        Expr expr = (*this).logical_and();
        while ((*this).match_any(std::vector{TK_OR})) {
            auto op = (*this).previous();
            Expr right = (*this).logical_and();
            expr = Expr::make_Logical(expr, op, right);
        }
        return expr;
    }

    Expr logical_and() {
        Expr expr = (*this).equality();
        while ((*this).match_any(std::vector{TK_AND})) {
            auto op = (*this).previous();
            Expr right = (*this).equality();
            expr = Expr::make_Logical(expr, op, right);
        }
        return expr;
    }

    Expr equality() {
        Expr expr = (*this).comparison();
        while ((*this).match_any(std::vector{TK_BANG_EQUAL, TK_EQUAL_EQUAL})) {
            auto op = (*this).previous();
            Expr right = (*this).comparison();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr comparison() {
        Expr expr = (*this).term();
        while ((*this).match_any(std::vector{TK_GREATER, TK_GREATER_EQUAL, TK_LESS, TK_LESS_EQUAL})) {
            auto op = (*this).previous();
            Expr right = (*this).term();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr term() {
        Expr expr = (*this).factor();
        while ((*this).match_any(std::vector{TK_MINUS, TK_PLUS})) {
            auto op = (*this).previous();
            Expr right = (*this).factor();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr factor() {
        Expr expr = (*this).unary();
        while ((*this).match_any(std::vector{TK_SLASH, TK_STAR, TK_PERCENT})) {
            auto op = (*this).previous();
            Expr right = (*this).unary();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr unary() {
        if ((*this).match_any(std::vector{TK_BANG, TK_MINUS, TK_NOT})) {
            auto op = (*this).previous();
            Expr right = (*this).unary();
            return Expr::make_Unary(op, right);
        }
        return (*this).call();
    }

    Expr call() {
        Expr expr = (*this).primary();
        bool more = true;
        while (more) {
            if ((*this).match_any(std::vector{TK_LEFT_PAREN})) {
                expr = (*this).finish_call(expr);
            }
             else {
                if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
                    Expr index = (*this).expression();
                    auto bracket = (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after index."));
                    expr = Expr::make_Index(expr, bracket, index);
                }
                 else {
                    if ((*this).match_any(std::vector{TK_DOT})) {
                        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect property name after '.'."));
                        expr = Expr::make_Get(expr, name);
                    }
                     else {
                        if ((*this).match_any(std::vector{TK_DOUBLE_COLON})) {
                            auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect member name after '::'."));
                            expr = Expr::make_StaticGet(expr, name);
                        }
                         else {
                            more = false;
                        }
                    }
                }
            }
        }
        return expr;
    }

    Expr finish_call(Expr callee) {
        {
            auto& _match_1 = callee;
            if (_match_1._tag == "Variable") {
                auto& _v = std::get<Expr::Variable>(_match_1._data);
                auto& name = _v.name;
                if (name.lexeme == std::string("cast")) {
                    Expr expr = (*this).expression();
                    (*this).consume(TK_COMMA, std::string("Expect ',' after value in cast()."));
                    TypeNode target = (*this).parse_type();
                    (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after type in cast()."));
                    return Expr::make_Cast(expr, target);
                }
            }
            else {
                int64_t noop = 0LL;
            }
        }
        std::vector<Expr> args = {};
        if (!(*this).check(TK_RIGHT_PAREN)) {
            args.push_back((*this).expression());
            while ((*this).match_any(std::vector{TK_COMMA})) {
                args.push_back((*this).expression());
            }
        }
        auto paren = (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after arguments."));
        return Expr::make_Call(callee, paren, args);
    }

    Expr primary() {
        if ((*this).match_any(std::vector{TK_FALSE})) {
            return Expr::make_Literal(std::string("bool"), std::string("false"));
        }
        if ((*this).match_any(std::vector{TK_TRUE})) {
            return Expr::make_Literal(std::string("bool"), std::string("true"));
        }
        if ((*this).match_any(std::vector{TK_NULL})) {
            return Expr::make_Literal(std::string("null"), std::string("null"));
        }
        if ((*this).match_any(std::vector{TK_INT})) {
            return Expr::make_Literal(std::string("int"), (*this).previous().lexeme);
        }
        if ((*this).match_any(std::vector{TK_FLOAT})) {
            return Expr::make_Literal(std::string("float"), (*this).previous().lexeme);
        }
        if ((*this).match_any(std::vector{TK_STRING})) {
            return Expr::make_Literal(std::string("string"), (*this).previous().lexeme);
        }
        if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
            return Expr::make_Variable((*this).previous());
        }
        if ((*this).match_any(std::vector{TK_THIS})) {
            return Expr::make_This((*this).previous());
        }
        if ((*this).match_any(std::vector{TK_LEFT_PAREN})) {
            Expr expr = (*this).expression();
            (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after expression."));
            return Expr::make_Grouping(expr);
        }
        if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
            std::vector<Expr> elements = {};
            (*this).skip_formatting();
            if (!(*this).check(TK_RIGHT_BRACKET)) {
                (*this).skip_formatting();
                elements.push_back((*this).expression());
                (*this).skip_formatting();
                while ((*this).match_any(std::vector{TK_COMMA})) {
                    (*this).skip_formatting();
                    elements.push_back((*this).expression());
                    (*this).skip_formatting();
                }
            }
            (*this).skip_formatting();
            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after vector literal."));
            return Expr::make_Vector(elements);
        }
        if ((*this).match_any(std::vector{TK_LEFT_BRACE})) {
            std::vector<Expr> keys = {};
            std::vector<Expr> values = {};
            (*this).skip_formatting();
            if (!(*this).check(TK_RIGHT_BRACE)) {
                (*this).skip_formatting();
                Expr key = (*this).expression();
                (*this).skip_formatting();
                (*this).consume(TK_COLON, std::string("Expect ':' after map key."));
                (*this).skip_formatting();
                Expr value = (*this).expression();
                (*this).skip_formatting();
                keys.push_back(key);
                values.push_back(value);
                while ((*this).match_any(std::vector{TK_COMMA})) {
                    (*this).skip_formatting();
                    key = (*this).expression();
                    (*this).skip_formatting();
                    (*this).consume(TK_COLON, std::string("Expect ':' after map key."));
                    (*this).skip_formatting();
                    value = (*this).expression();
                    (*this).skip_formatting();
                    keys.push_back(key);
                    values.push_back(value);
                }
            }
            (*this).skip_formatting();
            (*this).consume(TK_RIGHT_BRACE, std::string("Expect '}' after map literal."));
            return Expr::make_Map(keys, values);
        }
        auto t = (*this).peek();
        throw std::runtime_error(std::string("Expect expression. Got ") + t.token_type + std::string(" at ") + t.line + std::string(":") + t.col);
    }

    Stmt statement() {
        if ((*this).match_any(std::vector{TK_IF})) {
            return (*this).if_statement();
        }
        if ((*this).match_any(std::vector{TK_WHILE})) {
            return (*this).while_statement();
        }
        if ((*this).match_any(std::vector{TK_FOR})) {
            return (*this).for_statement();
        }
        if ((*this).match_any(std::vector{TK_RETURN})) {
            return (*this).return_statement();
        }
        if ((*this).match_any(std::vector{TK_THROW})) {
            return (*this).throw_statement();
        }
        if ((*this).match_any(std::vector{TK_MATCH})) {
            return (*this).match_statement();
        }
        if ((*this).match_any(std::vector{TK_BREAK})) {
            auto kw = (*this).previous();
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_Break(kw);
        }
        if ((*this).match_any(std::vector{TK_CONTINUE})) {
            auto kw = (*this).previous();
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_Continue(kw);
        }
        return (*this).expression_statement();
    }

    Stmt throw_statement() {
        Expr expr = (*this).expression();
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_ExprStmt(Expr::make_Throw(expr));
    }

    Stmt if_statement() {
        Expr condition = (*this).expression();
        (*this).consume(TK_COLON, std::string("Expect ':' after if condition."));
        Stmt then_branch = Stmt::make_Block((*this).block());
        Stmt else_branch = Stmt::make_None();
        if ((*this).match_any(std::vector{TK_ELIF})) {
            else_branch = (*this).if_statement();
        }
         else {
            if ((*this).match_any(std::vector{TK_ELSE})) {
                if ((*this).match_any(std::vector{TK_COLON})) {
                    else_branch = Stmt::make_Block((*this).block());
                }
                 else {
                    if ((*this).check(TK_IF)) {
                        else_branch = (*this).declaration();
                    }
                }
            }
        }
        return Stmt::make_If(condition, then_branch, else_branch);
    }

    Stmt while_statement() {
        Expr condition = (*this).expression();
        (*this).consume(TK_COLON, std::string("Expect ':' after while condition."));
        Stmt body = Stmt::make_Block((*this).block());
        return Stmt::make_While(condition, body);
    }

    Stmt for_statement() {
        auto item_name = (*this).consume(TK_IDENTIFIER, std::string("Expect variable name after 'for'."));
        (*this).consume(TK_IN, std::string("Expect 'in' after variable name."));
        Expr collection = (*this).expression();
        (*this).consume(TK_COLON, std::string("Expect ':' after for loop header."));
        Stmt body = Stmt::make_Block((*this).block());
        return Stmt::make_For(item_name, collection, body);
    }

    Stmt return_statement() {
        auto keyword = (*this).previous();
        Expr value = Expr::make_None();
        if (!(*this).check(TK_NEWLINE) && !(*this).check(TK_DEDENT) && !(*this).is_at_end()) {
            value = (*this).expression();
        }
        (*this).match_any(std::vector{TK_NEWLINE});
        return Stmt::make_Return(keyword, value);
    }

    Stmt match_statement() {
        Expr expr = (*this).expression();
        (*this).consume(TK_COLON, std::string("Expect ':' after match expression."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start match body."));
        std::vector<MatchArm> arm_patterns = {};
        std::vector<Stmt> arm_bodies = {};
        while (!(*this).check(TK_DEDENT) && !(*this).is_at_end()) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                int64_t noop = 0LL;
            }
             else {
                auto pattern_tok = (*this).consume(TK_IDENTIFIER, std::string("Expect pattern name in match arm."));
                std::string pattern_name = pattern_tok.lexeme;
                std::vector<std::string> bindings = {};
                if (pattern_name != std::string("_")) {
                    if ((*this).match_any(std::vector{TK_LEFT_PAREN})) {
                        if (!(*this).check(TK_RIGHT_PAREN)) {
                            bindings.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect binding name.")).lexeme);
                            while ((*this).match_any(std::vector{TK_COMMA})) {
                                bindings.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect binding name.")).lexeme);
                            }
                        }
                        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after bindings."));
                    }
                }
                (*this).consume(TK_COLON, std::string("Expect ':' after match pattern."));
                std::vector<Stmt> body = (*this).block();
                arm_patterns.push_back(MatchArm(pattern_name, bindings));
                arm_bodies.push_back(Stmt::make_Block(body));
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end match body."));
        return Stmt::make_Match(expr, arm_patterns, arm_bodies);
    }

    std::vector<Stmt> block() {
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start a block."));
        std::vector<Stmt> statements = {};
        while (!(*this).check(TK_DEDENT) && !(*this).is_at_end()) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                int64_t noop = 0LL;
            }
             else {
                statements.push_back((*this).declaration());
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end a block."));
        return statements;
    }

    Stmt expression_statement() {
        Expr expr = (*this).expression();
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_ExprStmt(expr);
    }

    Stmt var_declaration(std::string visibility) {
        TypeNode var_type = (*this).parse_type();
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect variable name after type."));
        Expr initializer = Expr::make_None();
        if ((*this).match_any(std::vector{TK_EQUAL})) {
            initializer = (*this).expression();
        }
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Let(name, var_type, initializer, visibility);
    }

    Stmt const_declaration(std::string visibility) {
        TypeNode const_type = (*this).parse_type();
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect constant name after type."));
        (*this).consume(TK_EQUAL, std::string("Const declaration must have an initializer."));
        Expr value = (*this).expression();
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Const(name, const_type, value, visibility);
    }

    Stmt function_declaration(std::string visibility, bool is_static) {
        TypeNode return_type = (*this).parse_type();
        bool is_inline = false;
        bool is_comptime = false;
        while ((*this).match_any(std::vector{TK_INLINE, TK_COMPTIME})) {
            if ((*this).previous().token_type == TK_INLINE) {
                is_inline = true;
            }
            if ((*this).previous().token_type == TK_COMPTIME) {
                is_comptime = true;
            }
        }
        (*this).consume(TK_FN, std::string("Expect 'fn' keyword after return type."));
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect function name."));
        (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after function name."));
        std::vector<Param> params = {};
        if (!(*this).check(TK_RIGHT_PAREN)) {
            TypeNode param_type = (*this).parse_type();
            auto param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
            params.push_back(Param(param_name, param_type));
            while ((*this).match_any(std::vector{TK_COMMA})) {
                param_type = (*this).parse_type();
                param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
                params.push_back(Param(param_name, param_type));
            }
        }
        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after parameters."));
        (*this).consume(TK_COLON, std::string("Expect ':' before function body."));
        std::vector<Stmt> body = (*this).block();
        return Stmt::make_Function(name, params, return_type, body, is_inline, is_comptime, is_static, visibility);
    }

    Stmt class_declaration(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect class name."));
        (*this).consume(TK_COLON, std::string("Expect ':' after class name."));
        auto old = this->in_class_body;
        this->in_class_body = true;
        auto body = (*this).block();
        this->in_class_body = old;
        return Stmt::make_Class(name, body, visibility);
    }

    Stmt struct_declaration(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect struct name."));
        (*this).consume(TK_COLON, std::string("Expect ':' after struct name."));
        auto old = this->in_class_body;
        this->in_class_body = true;
        auto body = (*this).block();
        this->in_class_body = old;
        return Stmt::make_Struct(name, body, visibility);
    }

    Stmt enum_declaration(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect enum name."));
        (*this).consume(TK_COLON, std::string("Expect ':' after enum name."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start enum body."));
        std::vector<EnumVariantNode> variants = {};
        while (!(*this).check(TK_DEDENT) && !(*this).is_at_end()) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                int64_t noop = 0LL;
            }
             else {
                if ((*this).check(TK_IDENTIFIER) && (*this).peek_at(1LL).token_type == TK_LEFT_PAREN) {
                    auto vname = (*this).consume(TK_IDENTIFIER, std::string("Expect variant name."));
                    (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after variant name."));
                    std::vector<TypeNode> fields = {};
                    std::vector<std::string> fnames = {};
                    if (!(*this).check(TK_RIGHT_PAREN)) {
                        fields.push_back((*this).parse_type());
                        fnames.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect field name.")).lexeme);
                        while ((*this).match_any(std::vector{TK_COMMA})) {
                            fields.push_back((*this).parse_type());
                            fnames.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect field name.")).lexeme);
                        }
                    }
                    (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after variant fields."));
                    variants.push_back(EnumVariantNode(vname, fields, fnames));
                    (*this).match_any(std::vector{TK_NEWLINE});
                }
                 else {
                    TypeNode vtype = (*this).parse_type();
                    auto vname = (*this).consume(TK_IDENTIFIER, std::string("Expect variant name."));
                    std::vector<TypeNode> types = {};
                    std::vector<std::string> old_fnames = {};
                    if (vtype._tag != std::string("NullType") && vtype._tag != std::string("Void")) {
                        types.push_back(vtype);
                        old_fnames.push_back(std::string("value"));
                    }
                    variants.push_back(EnumVariantNode(vname, types, old_fnames));
                    (*this).match_any(std::vector{TK_NEWLINE});
                }
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end enum body."));
        return Stmt::make_Enum(name, variants, visibility);
    }

    Stmt import_statement() {
        std::vector<Token> path = {};
        path.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect module name.")));
        while ((*this).match_any(std::vector{TK_DOUBLE_COLON})) {
            path.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect module name.")));
        }
        std::string alias = std::string("");
        if ((*this).match_any(std::vector{TK_AS})) {
            alias = (*this).consume(TK_IDENTIFIER, std::string("Expect alias name after 'as'.")).lexeme;
        }
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Import(path, alias);
    }

    Stmt namespace_statement(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect namespace name."));
        (*this).consume(TK_COLON, std::string("Expect ':' after namespace name."));
        auto body = (*this).block();
        return Stmt::make_Namespace(name, body, visibility);
    }

    Stmt try_statement() {
        (*this).consume(TK_COLON, std::string("Expect ':' after 'try'."));
        Stmt try_body = Stmt::make_Block((*this).block());
        (*this).consume(TK_CATCH, std::string("Expect 'catch' after 'try' block."));
        std::string exception_name = std::string("");
        if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
            exception_name = (*this).previous().lexeme;
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after 'catch'."));
        Stmt catch_body = Stmt::make_Block((*this).block());
        return Stmt::make_Try(try_body, catch_body, exception_name);
    }

    Stmt declaration() {
        std::string visibility = std::string("public");
        if ((*this).match_any(std::vector{TK_PRIVATE})) {
            visibility = std::string("private");
        }
         else {
            if ((*this).match_any(std::vector{TK_PUBLIC})) {
                visibility = std::string("public");
            }
        }
        bool is_static = (*this).match_any(std::vector{TK_STATIC});
        while ((*this).check(TK_HASH)) {
            while (!(*this).check(TK_NEWLINE) && !(*this).is_at_end()) {
                (*this).advance();
            }
            (*this).match_any(std::vector{TK_NEWLINE});
        }
        if ((*this).match_any(std::vector{TK_IMPORT})) {
            return (*this).import_statement();
        }
        if ((*this).match_any(std::vector{TK_NAMESPACE})) {
            return (*this).namespace_statement(visibility);
        }
        if ((*this).match_any(std::vector{TK_CLASS})) {
            return (*this).class_declaration(visibility);
        }
        if ((*this).match_any(std::vector{TK_STRUCT})) {
            return (*this).struct_declaration(visibility);
        }
        if ((*this).match_any(std::vector{TK_ENUM})) {
            return (*this).enum_declaration(visibility);
        }
        if ((*this).match_any(std::vector{TK_TRY})) {
            return (*this).try_statement();
        }
        if ((*this).match_any(std::vector{TK_CONST})) {
            return (*this).const_declaration(visibility);
        }
        if (this->in_class_body && (*this).check(TK_IDENTIFIER)) {
            auto ctor_name = (*this).peek().lexeme;
            if ((ctor_name == std::string("constructor") || ctor_name == std::string("destructor")) && (*this).peek_at(1LL).token_type == TK_LEFT_PAREN) {
                auto name_tok = (*this).advance();
                (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after ") + ctor_name + std::string("."));
                std::vector<Param> params = {};
                if (!(*this).check(TK_RIGHT_PAREN)) {
                    TypeNode param_type = (*this).parse_type();
                    auto param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
                    params.push_back(Param(param_name, param_type));
                    while ((*this).match_any(std::vector{TK_COMMA})) {
                        param_type = (*this).parse_type();
                        param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
                        params.push_back(Param(param_name, param_type));
                    }
                }
                (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after parameters."));
                (*this).consume(TK_COLON, std::string("Expect ':' before body."));
                std::vector<Stmt> body = (*this).block();
                return Stmt::make_Function(name_tok, params, TypeNode::make_Void(), body, false, false, is_static, visibility);
            }
        }
        if ((*this).is_type_at_pos(0LL)) {
            int64_t next_pos = 1LL;
            auto current_type = (*this).peek().token_type;
            if (current_type == TK_VECTOR) {
                next_pos = 4LL;
            }
             else {
                if (current_type == TK_HASHMAP) {
                    next_pos = 6LL;
                }
                 else {
                    if (current_type == TK_HASHSET) {
                        next_pos = 4LL;
                    }
                }
            }
            if ((*this).peek_at(next_pos).token_type == TK_QUESTION) {
                next_pos = next_pos + 1LL;
            }
            auto next_token = (*this).peek_at(next_pos).token_type;
            if (next_token == TK_IDENTIFIER || next_token == TK_FN || next_token == TK_INLINE || next_token == TK_COMPTIME) {
                if ((*this).check_function_start()) {
                    return (*this).function_declaration(visibility, is_static);
                }
                if (!this->in_class_body) {
                    int64_t eq_pos = next_pos + 1LL;
                    if ((*this).peek_at(eq_pos).token_type != TK_EQUAL) {
                        auto tok = (*this).peek_at(next_pos);
                        throw std::runtime_error(std::string("Bare field declarations are only allowed inside class/struct at ") + tok.line + std::string(":") + tok.col);
                    }
                }
                return (*this).var_declaration(visibility);
            }
        }
        return (*this).statement();
    }

    std::vector<Stmt> parse_program() {
        std::vector<Stmt> statements = {};
        while (!(*this).is_at_end()) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                int64_t noop = 0LL;
            }
             else {
                statements.push_back((*this).declaration());
            }
        }
        return statements;
    }

};

void test_parser() {
    auto parser_test_source = std::string("1 + 2 * 3\n");
    auto parser_test_scanner = Scanner(parser_test_source);
    parser_test_scanner.scan_tokens();
    auto parser = Parser(parser_test_scanner.tokens);
    std::vector<Stmt> stmts = parser.parse_program();
    print(std::string("Parsed statements: ") + static_cast<int64_t>(stmts.size()));
    for (auto& stmt : stmts) {
        {
            auto& _match_2 = stmt;
            if (_match_2._tag == "ExprStmt") {
                auto& _v = std::get<Stmt::ExprStmt>(_match_2._data);
                auto& expr = _v.expr;
                {
                    auto& _match_3 = expr;
                    if (_match_3._tag == "Binary") {
                        auto& _v = std::get<Expr::Binary>(_match_3._data);
                        auto& left = *_v.left;
                        auto& op = _v.op;
                        auto& right = *_v.right;
                        print(std::string("Binary: ") + op.lexeme);
                        {
                            auto& _match_4 = right;
                            if (_match_4._tag == "Binary") {
                                auto& _v = std::get<Expr::Binary>(_match_4._data);
                                auto& rl = *_v.left;
                                auto& rop = _v.op;
                                auto& rr = *_v.right;
                                print(std::string("  Right is Binary: ") + rop.lexeme);
                                print(std::string("OK: precedence correct (1 + (2 * 3))"));
                            }
                            else {
                                int64_t noop = 0LL;
                            }
                        }
                    }
                    else {
                        int64_t noop = 0LL;
                    }
                }
            }
            else {
                int64_t noop = 0LL;
            }
        }
    }
}

struct CppCodegen {
    std::string output;
    std::string declarations;
    int64_t indent_level;
    int64_t temp_counter;
    bool has_main;
    std::vector<std::string> known_enum_names;
    std::vector<std::vector<EnumVariantNode>> known_enum_variants;
    std::vector<std::string> dynamic_vars;

    CppCodegen() {
        this->output = std::string("");
        this->declarations = std::string("");
        this->indent_level = 0LL;
        this->temp_counter = 0LL;
        this->has_main = false;
        this->known_enum_names = {};
        this->known_enum_variants = {};
        this->dynamic_vars = {};
    }

    std::string indent() {
        std::string result = std::string("");
        int64_t i = 0LL;
        while (i < this->indent_level) {
            result = result + std::string("    ");
            i = i + 1LL;
        }
        return result;
    }

    bool is_dynamic_var(std::string name) {
        for (auto& v : this->dynamic_vars) {
            if (v == name) {
                return true;
            }
        }
        return false;
    }

    void add_dynamic_var(std::string name) {
        if (!(*this).is_dynamic_var(name)) {
            this->dynamic_vars.push_back(name);
        }
    }

    std::string find_enum_for_variant(std::string variant_name) {
        int64_t i = 0LL;
        while (i < static_cast<int64_t>(this->known_enum_names.size())) {
            std::vector<EnumVariantNode> variants = this->known_enum_variants[i];
            for (auto& v : variants) {
                if (v.name.lexeme == variant_name) {
                    return this->known_enum_names[i];
                }
            }
            i = i + 1LL;
        }
        return std::string("");
    }

    bool variant_has_fields(std::string enum_name, std::string variant_name) {
        int64_t i = 0LL;
        while (i < static_cast<int64_t>(this->known_enum_names.size())) {
            if (this->known_enum_names[i] == enum_name) {
                std::vector<EnumVariantNode> variants = this->known_enum_variants[i];
                for (auto& v : variants) {
                    if (v.name.lexeme == variant_name) {
                        return static_cast<int64_t>(v.types.size()) > 0LL;
                    }
                }
                return false;
            }
            i = i + 1LL;
        }
        return false;
    }

    EnumVariantNode get_variant_info(std::string enum_name, std::string variant_name) {
        int64_t i = 0LL;
        while (i < static_cast<int64_t>(this->known_enum_names.size())) {
            if (this->known_enum_names[i] == enum_name) {
                std::vector<EnumVariantNode> variants = this->known_enum_variants[i];
                for (auto& v : variants) {
                    if (v.name.lexeme == variant_name) {
                        return v;
                    }
                }
            }
            i = i + 1LL;
        }
        std::vector<TypeNode> empty_types = {};
        std::vector<std::string> empty_names = {};
        return EnumVariantNode(Token(std::string(""), std::string(""), 0LL, 0LL), empty_types, empty_names);
    }

    bool enum_has_data_variants(std::string enum_name) {
        int64_t i = 0LL;
        while (i < static_cast<int64_t>(this->known_enum_names.size())) {
            if (this->known_enum_names[i] == enum_name) {
                std::vector<EnumVariantNode> variants = this->known_enum_variants[i];
                for (auto& v : variants) {
                    if (static_cast<int64_t>(v.types.size()) > 0LL) {
                        return true;
                    }
                }
                return false;
            }
            i = i + 1LL;
        }
        return false;
    }

    bool is_known_enum(std::string name) {
        for (auto& n : this->known_enum_names) {
            if (n == name) {
                return true;
            }
        }
        return false;
    }

    bool type_contains_dynamic(TypeNode t) {
        {
            auto& _match_5 = t;
            if (_match_5._tag == "Dynamic") {
                return true;
            }
            else if (_match_5._tag == "Array") {
                auto& _v = std::get<TypeNode::Array>(_match_5._data);
                auto& inner = *_v.inner;
                return (*this).type_contains_dynamic(inner);
            }
            else {
                return false;
            }
        }
    }

    std::string emit_type(TypeNode t) {
        {
            auto& _match_6 = t;
            if (_match_6._tag == "Int") {
                return std::string("int64_t");
            }
            else if (_match_6._tag == "Float") {
                return std::string("double");
            }
            else if (_match_6._tag == "Str") {
                return std::string("std::string");
            }
            else if (_match_6._tag == "Bool") {
                return std::string("bool");
            }
            else if (_match_6._tag == "Void") {
                return std::string("void");
            }
            else if (_match_6._tag == "Auto") {
                return std::string("auto");
            }
            else if (_match_6._tag == "Dynamic") {
                return std::string("std::any");
            }
            else if (_match_6._tag == "NullType") {
                return std::string("std::nullptr_t");
            }
            else if (_match_6._tag == "Custom") {
                auto& _v = std::get<TypeNode::Custom>(_match_6._data);
                auto& name = _v.name;
                return name;
            }
            else if (_match_6._tag == "Array") {
                auto& _v = std::get<TypeNode::Array>(_match_6._data);
                auto& inner = *_v.inner;
                return std::string("std::vector<") + (*this).emit_type(inner) + std::string(">");
            }
            else if (_match_6._tag == "HashSet") {
                auto& _v = std::get<TypeNode::HashSet>(_match_6._data);
                auto& inner = *_v.inner;
                return std::string("std::unordered_set<") + (*this).emit_type(inner) + std::string(">");
            }
            else if (_match_6._tag == "Dict") {
                auto& _v = std::get<TypeNode::Dict>(_match_6._data);
                auto& key_type = *_v.key_type;
                auto& value_type = *_v.value_type;
                return std::string("std::unordered_map<") + (*this).emit_type(key_type) + std::string(", ") + (*this).emit_type(value_type) + std::string(">");
            }
            else if (_match_6._tag == "Nullable") {
                auto& _v = std::get<TypeNode::Nullable>(_match_6._data);
                auto& inner = *_v.inner;
                return std::string("std::optional<") + (*this).emit_type(inner) + std::string(">");
            }
            else {
                return std::string("auto");
            }
        }
    }

    std::string token_to_cpp_op(Token t) {
        if (t.token_type == TK_PLUS) {
            return std::string("+");
        }
         else {
            if (t.token_type == TK_MINUS) {
                return std::string("-");
            }
             else {
                if (t.token_type == TK_STAR) {
                    return std::string("*");
                }
                 else {
                    if (t.token_type == TK_SLASH) {
                        return std::string("/");
                    }
                     else {
                        if (t.token_type == TK_PERCENT) {
                            return std::string("%");
                        }
                         else {
                            if (t.token_type == TK_EQUAL_EQUAL) {
                                return std::string("==");
                            }
                             else {
                                if (t.token_type == TK_BANG_EQUAL) {
                                    return std::string("!=");
                                }
                                 else {
                                    if (t.token_type == TK_LESS) {
                                        return std::string("<");
                                    }
                                     else {
                                        if (t.token_type == TK_LESS_EQUAL) {
                                            return std::string("<=");
                                        }
                                         else {
                                            if (t.token_type == TK_GREATER) {
                                                return std::string(">");
                                            }
                                             else {
                                                if (t.token_type == TK_GREATER_EQUAL) {
                                                    return std::string(">=");
                                                }
                                                 else {
                                                    if (t.token_type == TK_BANG || t.token_type == TK_NOT) {
                                                        return std::string("!");
                                                    }
                                                     else {
                                                        if (t.token_type == TK_AND) {
                                                            return std::string("&&");
                                                        }
                                                         else {
                                                            if (t.token_type == TK_OR) {
                                                                return std::string("||");
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::string("/* unknown op */");
    }

    std::string default_init(std::string cpp_type) {
        if (cpp_type == std::string("int64_t")) {
            return std::string(" = 0");
        }
         else {
            if (cpp_type == std::string("double")) {
                return std::string(" = 0.0");
            }
             else {
                if (cpp_type == std::string("bool")) {
                    return std::string(" = false");
                }
                 else {
                    if (cpp_type == std::string("std::string")) {
                        return std::string(" = \"\"");
                    }
                }
            }
        }
        return std::string("");
    }

    std::string emit_expr(Expr e) {
        {
            auto& _match_7 = e;
            if (_match_7._tag == "None") {
                return std::string("");
            }
            else if (_match_7._tag == "Literal") {
                auto& _v = std::get<Expr::Literal>(_match_7._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                return (*this).emit_literal(kind, value);
            }
            else if (_match_7._tag == "Unary") {
                auto& _v = std::get<Expr::Unary>(_match_7._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string r = (*this).emit_expr(right);
                std::string op_str = (*this).token_to_cpp_op(op);
                return op_str + r;
            }
            else if (_match_7._tag == "Binary") {
                auto& _v = std::get<Expr::Binary>(_match_7._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_expr(left);
                std::string r = (*this).emit_expr(right);
                std::string op_str = (*this).token_to_cpp_op(op);
                return l + std::string(" ") + op_str + std::string(" ") + r;
            }
            else if (_match_7._tag == "Grouping") {
                auto& _v = std::get<Expr::Grouping>(_match_7._data);
                auto& inner = *_v.inner;
                std::string i = (*this).emit_expr(inner);
                return std::string("(") + i + std::string(")");
            }
            else if (_match_7._tag == "Variable") {
                auto& _v = std::get<Expr::Variable>(_match_7._data);
                auto& name = _v.name;
                return name.lexeme;
            }
            else if (_match_7._tag == "Assign") {
                auto& _v = std::get<Expr::Assign>(_match_7._data);
                auto& name = _v.name;
                auto& value = *_v.value;
                std::string v = (*this).emit_expr(value);
                return name.lexeme + std::string(" = ") + v;
            }
            else if (_match_7._tag == "Logical") {
                auto& _v = std::get<Expr::Logical>(_match_7._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_expr(left);
                std::string r = (*this).emit_expr(right);
                std::string op_str = std::string("&&");
                if (op.token_type == TK_OR) {
                    op_str = std::string("||");
                }
                return l + std::string(" ") + op_str + std::string(" ") + r;
            }
            else if (_match_7._tag == "Call") {
                auto& _v = std::get<Expr::Call>(_match_7._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                return (*this).emit_call_expr(callee, args, false);
            }
            else if (_match_7._tag == "Index") {
                auto& _v = std::get<Expr::Index>(_match_7._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                std::string obj = (*this).emit_expr(object);
                std::string idx = (*this).emit_expr(index);
                return obj + std::string("[") + idx + std::string("]");
            }
            else if (_match_7._tag == "Vector") {
                auto& _v = std::get<Expr::Vector>(_match_7._data);
                auto& elements = _v.elements;
                if (static_cast<int64_t>(elements.size()) == 0LL) {
                    return std::string("{}");
                }
                std::vector<std::string> elems = {};
                for (auto& el : elements) {
                    elems.push_back((*this).emit_expr(el));
                }
                return std::string("std::vector{") + lv_join(elems, std::string(", ")) + std::string("}");
            }
            else if (_match_7._tag == "Map") {
                auto& _v = std::get<Expr::Map>(_match_7._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                std::vector<std::string> entries = {};
                int64_t i = 0LL;
                while (i < static_cast<int64_t>(keys.size())) {
                    std::string k = (*this).emit_expr(keys[i]);
                    std::string v = (*this).emit_expr(values[i]);
                    entries.push_back(std::string("{") + k + std::string(", ") + v + std::string("}"));
                    i = i + 1LL;
                }
                return std::string("std::unordered_map{") + std::string("{") + lv_join(entries, std::string(", ")) + std::string("}") + std::string("}");
            }
            else if (_match_7._tag == "Get") {
                auto& _v = std::get<Expr::Get>(_match_7._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = (*this).emit_expr(object);
                return obj + std::string(".") + name.lexeme;
            }
            else if (_match_7._tag == "Set") {
                auto& _v = std::get<Expr::Set>(_match_7._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                auto& value = *_v.value;
                std::string obj = (*this).emit_expr(object);
                std::string val = (*this).emit_expr(value);
                return obj + std::string(".") + name.lexeme + std::string(" = ") + val;
            }
            else if (_match_7._tag == "StaticGet") {
                auto& _v = std::get<Expr::StaticGet>(_match_7._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = (*this).emit_expr(object);
                return obj + std::string("::") + name.lexeme;
            }
            else if (_match_7._tag == "This") {
                auto& _v = std::get<Expr::This>(_match_7._data);
                auto& keyword = _v.keyword;
                return std::string("(*this)");
            }
            else if (_match_7._tag == "Cast") {
                auto& _v = std::get<Expr::Cast>(_match_7._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                std::string ex = (*this).emit_expr(expr);
                std::string t = (*this).emit_type(target_type);
                if ((*this).is_dynamic_expression(expr)) {
                    return std::string("std::any_cast<") + t + std::string(">(") + ex + std::string(")");
                }
                return std::string("static_cast<") + t + std::string(">(") + ex + std::string(")");
            }
            else if (_match_7._tag == "Throw") {
                auto& _v = std::get<Expr::Throw>(_match_7._data);
                auto& expr = *_v.expr;
                std::string ex = (*this).emit_expr(expr);
                return std::string("throw std::runtime_error(") + ex + std::string(")");
            }
            else {
                return std::string("");
            }
        }
    }

    std::string emit_literal(std::string kind, std::string value) {
        if (kind == std::string("int")) {
            return value + std::string("LL");
        }
         else {
            if (kind == std::string("float")) {
                return value;
            }
             else {
                if (kind == std::string("string")) {
                    return std::string("std::string(\"") + value + std::string("\")");
                }
                 else {
                    if (kind == std::string("bool")) {
                        return value;
                    }
                     else {
                        if (kind == std::string("null")) {
                            return std::string("std::nullopt");
                        }
                    }
                }
            }
        }
        return value;
    }

    bool is_dynamic_expression(Expr e) {
        {
            auto& _match_8 = e;
            if (_match_8._tag == "Variable") {
                auto& _v = std::get<Expr::Variable>(_match_8._data);
                auto& name = _v.name;
                return (*this).is_dynamic_var(name.lexeme);
            }
            else if (_match_8._tag == "Index") {
                auto& _v = std::get<Expr::Index>(_match_8._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                return (*this).is_dynamic_expression(object);
            }
            else {
                return false;
            }
        }
    }

    std::string emit_call_expr(Expr callee, std::vector<Expr> args, bool in_method) {
        {
            auto& _match_9 = callee;
            if (_match_9._tag == "Get") {
                auto& _v = std::get<Expr::Get>(_match_9._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = std::string("");
                if (in_method) {
                    obj = (*this).emit_method_expr(object);
                }
                 else {
                    obj = (*this).emit_expr(object);
                }
                std::vector<std::string> arg_strs = {};
                for (auto& a : args) {
                    if (in_method) {
                        arg_strs.push_back((*this).emit_method_expr(a));
                    }
                     else {
                        arg_strs.push_back((*this).emit_expr(a));
                    }
                }
                std::string remapped = (*this).try_remap_method(obj, name.lexeme, arg_strs);
                if (remapped != std::string("")) {
                    return remapped;
                }
                return obj + std::string(".") + name.lexeme + std::string("(") + lv_join(arg_strs, std::string(", ")) + std::string(")");
            }
            else if (_match_9._tag == "StaticGet") {
                auto& _v = std::get<Expr::StaticGet>(_match_9._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = std::string("");
                if (in_method) {
                    obj = (*this).emit_method_expr(object);
                }
                 else {
                    obj = (*this).emit_expr(object);
                }
                std::vector<std::string> arg_strs = {};
                for (auto& a : args) {
                    if (in_method) {
                        arg_strs.push_back((*this).emit_method_expr(a));
                    }
                     else {
                        arg_strs.push_back((*this).emit_expr(a));
                    }
                }
                {
                    auto& _match_10 = object;
                    if (_match_10._tag == "Variable") {
                        auto& _v = std::get<Expr::Variable>(_match_10._data);
                        auto& tok = _v.name;
                        if ((*this).is_known_enum(tok.lexeme)) {
                            return obj + std::string("::make_") + name.lexeme + std::string("(") + lv_join(arg_strs, std::string(", ")) + std::string(")");
                        }
                    }
                    else {
                        int64_t noop = 0LL;
                    }
                }
                return obj + std::string("::") + name.lexeme + std::string("(") + lv_join(arg_strs, std::string(", ")) + std::string(")");
            }
            else if (_match_9._tag == "Variable") {
                auto& _v = std::get<Expr::Variable>(_match_9._data);
                auto& tok = _v.name;
                std::vector<std::string> arg_strs = {};
                for (auto& a : args) {
                    if (in_method) {
                        arg_strs.push_back((*this).emit_method_expr(a));
                    }
                     else {
                        arg_strs.push_back((*this).emit_expr(a));
                    }
                }
                if (tok.lexeme == std::string("exit")) {
                    return std::string("lv_exit(") + lv_join(arg_strs, std::string(", ")) + std::string(")");
                }
                return tok.lexeme + std::string("(") + lv_join(arg_strs, std::string(", ")) + std::string(")");
            }
            else {
                std::string func = std::string("");
                if (in_method) {
                    func = (*this).emit_method_expr(callee);
                }
                 else {
                    func = (*this).emit_expr(callee);
                }
                std::vector<std::string> arg_strs = {};
                for (auto& a : args) {
                    if (in_method) {
                        arg_strs.push_back((*this).emit_method_expr(a));
                    }
                     else {
                        arg_strs.push_back((*this).emit_expr(a));
                    }
                }
                return func + std::string("(") + lv_join(arg_strs, std::string(", ")) + std::string(")");
            }
        }
    }

    std::string try_remap_method(std::string obj, std::string method, std::vector<std::string> args) {
        if (method == std::string("len")) {
            return std::string("static_cast<int64_t>(") + obj + std::string(".size())");
        }
         else {
            if (method == std::string("is_empty")) {
                return obj + std::string(".empty()");
            }
             else {
                if (method == std::string("contains")) {
                    return std::string("lv_contains(") + obj + std::string(", ") + lv_join(args, std::string(", ")) + std::string(")");
                }
                 else {
                    if (method == std::string("add")) {
                        return obj + std::string(".insert(") + lv_join(args, std::string(", ")) + std::string(")");
                    }
                     else {
                        if (method == std::string("upper")) {
                            return std::string("lv_upper(") + obj + std::string(")");
                        }
                         else {
                            if (method == std::string("lower")) {
                                return std::string("lv_lower(") + obj + std::string(")");
                            }
                             else {
                                if (method == std::string("trim")) {
                                    return std::string("lv_trim(") + obj + std::string(")");
                                }
                                 else {
                                    if (method == std::string("replace")) {
                                        return std::string("lv_replace(") + obj + std::string(", ") + lv_join(args, std::string(", ")) + std::string(")");
                                    }
                                     else {
                                        if (method == std::string("split")) {
                                            if (static_cast<int64_t>(args.size()) > 0LL) {
                                                return std::string("lv_split(") + obj + std::string(", ") + args[0LL] + std::string(")");
                                            }
                                            return std::string("lv_split(") + obj + std::string(", std::string(\" \"))");
                                        }
                                         else {
                                            if (method == std::string("starts_with")) {
                                                return obj + std::string(".starts_with(") + lv_join(args, std::string(", ")) + std::string(")");
                                            }
                                             else {
                                                if (method == std::string("ends_with")) {
                                                    return obj + std::string(".ends_with(") + lv_join(args, std::string(", ")) + std::string(")");
                                                }
                                                 else {
                                                    if (method == std::string("indexOf")) {
                                                        return std::string("lv_index_of(") + obj + std::string(", ") + lv_join(args, std::string(", ")) + std::string(")");
                                                    }
                                                     else {
                                                        if (method == std::string("charAt")) {
                                                            return std::string("std::string(1, ") + obj + std::string("[") + lv_join(args, std::string(", ")) + std::string("])");
                                                        }
                                                         else {
                                                            if (method == std::string("substring")) {
                                                                if (static_cast<int64_t>(args.size()) >= 2LL) {
                                                                    return obj + std::string(".substr(") + args[0LL] + std::string(", (") + args[1LL] + std::string(") - (") + args[0LL] + std::string("))");
                                                                }
                                                                return obj + std::string(".substr(") + lv_join(args, std::string(", ")) + std::string(")");
                                                            }
                                                             else {
                                                                if (method == std::string("push")) {
                                                                    return obj + std::string(".push_back(") + lv_join(args, std::string(", ")) + std::string(")");
                                                                }
                                                                 else {
                                                                    if (method == std::string("pop")) {
                                                                        return std::string("lv_pop(") + obj + std::string(")");
                                                                    }
                                                                     else {
                                                                        if (method == std::string("clear")) {
                                                                            return obj + std::string(".clear()");
                                                                        }
                                                                         else {
                                                                            if (method == std::string("remove")) {
                                                                                return std::string("lv_remove(") + obj + std::string(", ") + lv_join(args, std::string(", ")) + std::string(")");
                                                                            }
                                                                             else {
                                                                                if (method == std::string("join")) {
                                                                                    if (static_cast<int64_t>(args.size()) > 0LL) {
                                                                                        return std::string("lv_join(") + obj + std::string(", ") + args[0LL] + std::string(")");
                                                                                    }
                                                                                    return std::string("lv_join(") + obj + std::string(", std::string(\"\"))");
                                                                                }
                                                                                 else {
                                                                                    if (method == std::string("reverse")) {
                                                                                        return std::string("lv_reverse(") + obj + std::string(")");
                                                                                    }
                                                                                     else {
                                                                                        if (method == std::string("keys")) {
                                                                                            return std::string("lv_keys(") + obj + std::string(")");
                                                                                        }
                                                                                         else {
                                                                                            if (method == std::string("values")) {
                                                                                                return std::string("lv_values(") + obj + std::string(")");
                                                                                            }
                                                                                             else {
                                                                                                if (method == std::string("has")) {
                                                                                                    return std::string("(") + obj + std::string(".count(") + lv_join(args, std::string(", ")) + std::string(") > 0)");
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::string("");
    }

    std::string emit_method_expr(Expr e) {
        {
            auto& _match_11 = e;
            if (_match_11._tag == "Set") {
                auto& _v = std::get<Expr::Set>(_match_11._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                auto& value = *_v.value;
                {
                    auto& _match_12 = object;
                    if (_match_12._tag == "This") {
                        auto& _v = std::get<Expr::This>(_match_12._data);
                        auto& kw = _v.keyword;
                        std::string val = (*this).emit_method_expr(value);
                        return std::string("this->") + name.lexeme + std::string(" = ") + val;
                    }
                    else {
                        std::string obj = (*this).emit_method_expr(object);
                        std::string val = (*this).emit_method_expr(value);
                        return obj + std::string(".") + name.lexeme + std::string(" = ") + val;
                    }
                }
            }
            else if (_match_11._tag == "Get") {
                auto& _v = std::get<Expr::Get>(_match_11._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                {
                    auto& _match_13 = object;
                    if (_match_13._tag == "This") {
                        auto& _v = std::get<Expr::This>(_match_13._data);
                        auto& kw = _v.keyword;
                        return std::string("this->") + name.lexeme;
                    }
                    else {
                        std::string obj = (*this).emit_method_expr(object);
                        return obj + std::string(".") + name.lexeme;
                    }
                }
            }
            else if (_match_11._tag == "This") {
                auto& _v = std::get<Expr::This>(_match_11._data);
                auto& keyword = _v.keyword;
                return std::string("(*this)");
            }
            else if (_match_11._tag == "Grouping") {
                auto& _v = std::get<Expr::Grouping>(_match_11._data);
                auto& inner = *_v.inner;
                std::string i = (*this).emit_method_expr(inner);
                return std::string("(") + i + std::string(")");
            }
            else if (_match_11._tag == "Index") {
                auto& _v = std::get<Expr::Index>(_match_11._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                std::string obj = (*this).emit_method_expr(object);
                std::string idx = (*this).emit_method_expr(index);
                return obj + std::string("[") + idx + std::string("]");
            }
            else if (_match_11._tag == "Vector") {
                auto& _v = std::get<Expr::Vector>(_match_11._data);
                auto& elements = _v.elements;
                if (static_cast<int64_t>(elements.size()) == 0LL) {
                    return std::string("{}");
                }
                std::vector<std::string> elems = {};
                for (auto& el : elements) {
                    elems.push_back((*this).emit_method_expr(el));
                }
                return std::string("std::vector{") + lv_join(elems, std::string(", ")) + std::string("}");
            }
            else if (_match_11._tag == "Binary") {
                auto& _v = std::get<Expr::Binary>(_match_11._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_method_expr(left);
                std::string r = (*this).emit_method_expr(right);
                std::string op_str = (*this).token_to_cpp_op(op);
                return l + std::string(" ") + op_str + std::string(" ") + r;
            }
            else if (_match_11._tag == "Unary") {
                auto& _v = std::get<Expr::Unary>(_match_11._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string r = (*this).emit_method_expr(right);
                std::string op_str = (*this).token_to_cpp_op(op);
                return op_str + r;
            }
            else if (_match_11._tag == "Logical") {
                auto& _v = std::get<Expr::Logical>(_match_11._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_method_expr(left);
                std::string r = (*this).emit_method_expr(right);
                std::string op_str = std::string("&&");
                if (op.token_type == TK_OR) {
                    op_str = std::string("||");
                }
                return l + std::string(" ") + op_str + std::string(" ") + r;
            }
            else if (_match_11._tag == "Assign") {
                auto& _v = std::get<Expr::Assign>(_match_11._data);
                auto& name = _v.name;
                auto& value = *_v.value;
                std::string v = (*this).emit_method_expr(value);
                return name.lexeme + std::string(" = ") + v;
            }
            else if (_match_11._tag == "Call") {
                auto& _v = std::get<Expr::Call>(_match_11._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                return (*this).emit_call_expr(callee, args, true);
            }
            else {
                return (*this).emit_expr(e);
            }
        }
    }

    void emit_stmt(Stmt s) {
        {
            auto& _match_14 = s;
            if (_match_14._tag == "None") {
                int64_t noop = 0LL;
            }
            else if (_match_14._tag == "ExprStmt") {
                auto& _v = std::get<Stmt::ExprStmt>(_match_14._data);
                auto& expr = _v.expr;
                std::string e = (*this).emit_expr(expr);
                this->output = this->output + (*this).indent() + e + std::string(";\n");
            }
            else if (_match_14._tag == "Let") {
                auto& _v = std::get<Stmt::Let>(_match_14._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                (*this).emit_let(name, var_type, initializer, false);
            }
            else if (_match_14._tag == "Const") {
                auto& _v = std::get<Stmt::Const>(_match_14._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                std::string cpp_type = (*this).emit_type(const_type);
                std::string val = (*this).emit_expr(value);
                this->output = this->output + (*this).indent() + std::string("const ") + cpp_type + std::string(" ") + name.lexeme + std::string(" = ") + val + std::string(";\n");
            }
            else if (_match_14._tag == "Return") {
                auto& _v = std::get<Stmt::Return>(_match_14._data);
                auto& keyword = _v.keyword;
                auto& value = _v.value;
                {
                    auto& _match_15 = value;
                    if (_match_15._tag == "None") {
                        this->output = this->output + (*this).indent() + std::string("return;\n");
                    }
                    else {
                        std::string val = (*this).emit_expr(value);
                        this->output = this->output + (*this).indent() + std::string("return ") + val + std::string(";\n");
                    }
                }
            }
            else if (_match_14._tag == "If") {
                auto& _v = std::get<Stmt::If>(_match_14._data);
                auto& condition = _v.condition;
                auto& then_branch = *_v.then_branch;
                auto& else_branch = *_v.else_branch;
                std::string c = (*this).emit_expr(condition);
                this->output = this->output + (*this).indent() + std::string("if (") + c + std::string(") ");
                (*this).emit_block_or_stmt(then_branch);
                {
                    auto& _match_16 = else_branch;
                    if (_match_16._tag == "None") {
                        int64_t noop = 0LL;
                    }
                    else {
                        this->output = this->output + (*this).indent() + std::string(" else ");
                        (*this).emit_block_or_stmt(else_branch);
                    }
                }
            }
            else if (_match_14._tag == "While") {
                auto& _v = std::get<Stmt::While>(_match_14._data);
                auto& condition = _v.condition;
                auto& body = *_v.body;
                std::string c = (*this).emit_expr(condition);
                this->output = this->output + (*this).indent() + std::string("while (") + c + std::string(") ");
                (*this).emit_block_or_stmt(body);
            }
            else if (_match_14._tag == "For") {
                auto& _v = std::get<Stmt::For>(_match_14._data);
                auto& item_name = _v.item_name;
                auto& collection = _v.collection;
                auto& body = *_v.body;
                {
                    auto& _match_17 = collection;
                    if (_match_17._tag == "Variable") {
                        auto& _v = std::get<Expr::Variable>(_match_17._data);
                        auto& tok = _v.name;
                        if ((*this).is_dynamic_var(tok.lexeme)) {
                            (*this).add_dynamic_var(item_name.lexeme);
                        }
                    }
                    else {
                        int64_t noop = 0LL;
                    }
                }
                std::string iter = (*this).emit_expr(collection);
                this->output = this->output + (*this).indent() + std::string("for (auto& ") + item_name.lexeme + std::string(" : ") + iter + std::string(") ");
                (*this).emit_block_or_stmt(body);
            }
            else if (_match_14._tag == "Block") {
                auto& _v = std::get<Stmt::Block>(_match_14._data);
                auto& statements = _v.statements;
                this->output = this->output + (*this).indent() + std::string("{\n");
                this->indent_level = this->indent_level + 1LL;
                for (auto& st : statements) {
                    (*this).emit_stmt(st);
                }
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
            else if (_match_14._tag == "Try") {
                auto& _v = std::get<Stmt::Try>(_match_14._data);
                auto& try_body = *_v.try_body;
                auto& catch_body = *_v.catch_body;
                auto& exception_name = _v.exception_name;
                this->output = this->output + (*this).indent() + std::string("try ");
                (*this).emit_block_or_stmt(try_body);
                std::string exc_name = std::string("e");
                if (exception_name != std::string("")) {
                    exc_name = exception_name;
                }
                this->output = this->output + (*this).indent() + std::string(" catch (const std::exception& ") + exc_name + std::string(") ");
                (*this).emit_block_or_stmt(catch_body);
            }
            else if (_match_14._tag == "Function") {
                auto& _v = std::get<Stmt::Function>(_match_14._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& is_comptime = _v.is_comptime;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                (*this).emit_function(name, params, return_type, body);
            }
            else if (_match_14._tag == "Class") {
                auto& _v = std::get<Stmt::Class>(_match_14._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                (*this).emit_class(name, body);
            }
            else if (_match_14._tag == "Struct") {
                auto& _v = std::get<Stmt::Struct>(_match_14._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                (*this).emit_class(name, body);
            }
            else if (_match_14._tag == "Enum") {
                auto& _v = std::get<Stmt::Enum>(_match_14._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& visibility = _v.visibility;
                (*this).emit_enum(name, variants);
            }
            else if (_match_14._tag == "Match") {
                auto& _v = std::get<Stmt::Match>(_match_14._data);
                auto& expr = _v.expr;
                auto& arm_patterns = _v.arm_patterns;
                auto& arm_bodies = _v.arm_bodies;
                (*this).emit_match(expr, arm_patterns, arm_bodies);
            }
            else if (_match_14._tag == "Namespace") {
                auto& _v = std::get<Stmt::Namespace>(_match_14._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                this->output = this->output + (*this).indent() + std::string("// TODO: unsupported namespace\n");
            }
            else if (_match_14._tag == "Import") {
                auto& _v = std::get<Stmt::Import>(_match_14._data);
                auto& path = _v.path;
                auto& alias = _v.alias;
                this->output = this->output + (*this).indent() + std::string("// TODO: unsupported import\n");
            }
            else if (_match_14._tag == "Break") {
                auto& _v = std::get<Stmt::Break>(_match_14._data);
                auto& keyword = _v.keyword;
                this->output = this->output + (*this).indent() + std::string("break;\n");
            }
            else if (_match_14._tag == "Continue") {
                auto& _v = std::get<Stmt::Continue>(_match_14._data);
                auto& keyword = _v.keyword;
                this->output = this->output + (*this).indent() + std::string("continue;\n");
            }
            else {
                int64_t noop = 0LL;
            }
        }
    }

    void emit_let(Token name, TypeNode var_type, Expr initializer, bool in_method) {
        std::string cpp_type = (*this).emit_type(var_type);
        if ((*this).type_contains_dynamic(var_type)) {
            (*this).add_dynamic_var(name.lexeme);
        }
        std::string init_str = std::string("");
        {
            auto& _match_18 = initializer;
            if (_match_18._tag == "None") {
                init_str = (*this).default_init(cpp_type);
            }
            else {
                std::string val = std::string("");
                if (in_method) {
                    val = (*this).emit_method_expr(initializer);
                }
                 else {
                    val = (*this).emit_expr(initializer);
                }
                init_str = std::string(" = ") + val;
            }
        }
        this->output = this->output + (*this).indent() + cpp_type + std::string(" ") + name.lexeme + init_str + std::string(";\n");
    }

    void emit_block_or_stmt(Stmt s) {
        {
            auto& _match_19 = s;
            if (_match_19._tag == "Block") {
                auto& _v = std::get<Stmt::Block>(_match_19._data);
                auto& stmts = _v.statements;
                this->output = this->output + std::string("{\n");
                this->indent_level = this->indent_level + 1LL;
                for (auto& st : stmts) {
                    (*this).emit_stmt(st);
                }
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
            else {
                this->output = this->output + std::string("{\n");
                this->indent_level = this->indent_level + 1LL;
                (*this).emit_stmt(s);
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
        }
    }

    void emit_method_block_or_stmt(Stmt s) {
        {
            auto& _match_20 = s;
            if (_match_20._tag == "Block") {
                auto& _v = std::get<Stmt::Block>(_match_20._data);
                auto& stmts = _v.statements;
                this->output = this->output + std::string("{\n");
                this->indent_level = this->indent_level + 1LL;
                for (auto& st : stmts) {
                    (*this).emit_method_stmt(st);
                }
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
            else {
                this->output = this->output + std::string("{\n");
                this->indent_level = this->indent_level + 1LL;
                (*this).emit_method_stmt(s);
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
        }
    }

    void emit_function(Token name, std::vector<Param> params, TypeNode return_type, std::vector<Stmt> body) {
        std::string ret_type = (*this).emit_type(return_type);
        std::vector<std::string> param_strs = {};
        for (auto& p : params) {
            std::string pt = (*this).emit_type(p.param_type);
            param_strs.push_back(pt + std::string(" ") + p.name.lexeme);
            {
                auto& _match_21 = p.param_type;
                if (_match_21._tag == "Dynamic") {
                    (*this).add_dynamic_var(p.name.lexeme);
                }
                else {
                    int64_t noop = 0LL;
                }
            }
        }
        this->output = this->output + (*this).indent() + ret_type + std::string(" ") + name.lexeme + std::string("(") + lv_join(param_strs, std::string(", ")) + std::string(") {\n");
        this->indent_level = this->indent_level + 1LL;
        for (auto& st : body) {
            (*this).emit_stmt(st);
        }
        this->indent_level = this->indent_level - 1LL;
        this->output = this->output + (*this).indent() + std::string("}\n\n");
    }

    void emit_method_stmt(Stmt s) {
        {
            auto& _match_22 = s;
            if (_match_22._tag == "ExprStmt") {
                auto& _v = std::get<Stmt::ExprStmt>(_match_22._data);
                auto& expr = _v.expr;
                std::string e = (*this).emit_method_expr(expr);
                this->output = this->output + (*this).indent() + e + std::string(";\n");
            }
            else if (_match_22._tag == "Return") {
                auto& _v = std::get<Stmt::Return>(_match_22._data);
                auto& keyword = _v.keyword;
                auto& value = _v.value;
                {
                    auto& _match_23 = value;
                    if (_match_23._tag == "None") {
                        this->output = this->output + (*this).indent() + std::string("return;\n");
                    }
                    else {
                        std::string val = (*this).emit_method_expr(value);
                        this->output = this->output + (*this).indent() + std::string("return ") + val + std::string(";\n");
                    }
                }
            }
            else if (_match_22._tag == "If") {
                auto& _v = std::get<Stmt::If>(_match_22._data);
                auto& condition = _v.condition;
                auto& then_branch = *_v.then_branch;
                auto& else_branch = *_v.else_branch;
                std::string c = (*this).emit_method_expr(condition);
                this->output = this->output + (*this).indent() + std::string("if (") + c + std::string(") ");
                (*this).emit_method_block_or_stmt(then_branch);
                {
                    auto& _match_24 = else_branch;
                    if (_match_24._tag == "None") {
                        int64_t noop = 0LL;
                    }
                    else {
                        this->output = this->output + (*this).indent() + std::string(" else ");
                        (*this).emit_method_block_or_stmt(else_branch);
                    }
                }
            }
            else if (_match_22._tag == "While") {
                auto& _v = std::get<Stmt::While>(_match_22._data);
                auto& condition = _v.condition;
                auto& body = *_v.body;
                std::string c = (*this).emit_method_expr(condition);
                this->output = this->output + (*this).indent() + std::string("while (") + c + std::string(") ");
                (*this).emit_method_block_or_stmt(body);
            }
            else if (_match_22._tag == "For") {
                auto& _v = std::get<Stmt::For>(_match_22._data);
                auto& item_name = _v.item_name;
                auto& collection = _v.collection;
                auto& body = *_v.body;
                {
                    auto& _match_25 = collection;
                    if (_match_25._tag == "Variable") {
                        auto& _v = std::get<Expr::Variable>(_match_25._data);
                        auto& tok = _v.name;
                        if ((*this).is_dynamic_var(tok.lexeme)) {
                            (*this).add_dynamic_var(item_name.lexeme);
                        }
                    }
                    else {
                        int64_t noop = 0LL;
                    }
                }
                std::string iter = (*this).emit_method_expr(collection);
                this->output = this->output + (*this).indent() + std::string("for (auto& ") + item_name.lexeme + std::string(" : ") + iter + std::string(") ");
                (*this).emit_method_block_or_stmt(body);
            }
            else if (_match_22._tag == "Let") {
                auto& _v = std::get<Stmt::Let>(_match_22._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                (*this).emit_let(name, var_type, initializer, true);
            }
            else if (_match_22._tag == "Block") {
                auto& _v = std::get<Stmt::Block>(_match_22._data);
                auto& stmts = _v.statements;
                this->output = this->output + (*this).indent() + std::string("{\n");
                this->indent_level = this->indent_level + 1LL;
                for (auto& st : stmts) {
                    (*this).emit_method_stmt(st);
                }
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
            else if (_match_22._tag == "Try") {
                auto& _v = std::get<Stmt::Try>(_match_22._data);
                auto& try_body = *_v.try_body;
                auto& catch_body = *_v.catch_body;
                auto& exception_name = _v.exception_name;
                this->output = this->output + (*this).indent() + std::string("try ");
                (*this).emit_method_block_or_stmt(try_body);
                std::string exc_name = std::string("e");
                if (exception_name != std::string("")) {
                    exc_name = exception_name;
                }
                this->output = this->output + (*this).indent() + std::string(" catch (const std::exception& ") + exc_name + std::string(") ");
                (*this).emit_method_block_or_stmt(catch_body);
            }
            else if (_match_22._tag == "Match") {
                auto& _v = std::get<Stmt::Match>(_match_22._data);
                auto& expr = _v.expr;
                auto& arm_patterns = _v.arm_patterns;
                auto& arm_bodies = _v.arm_bodies;
                (*this).emit_match_in_method(expr, arm_patterns, arm_bodies);
            }
            else {
                (*this).emit_stmt(s);
            }
        }
    }

    void emit_class(Token name, std::vector<Stmt> body) {
        std::vector<Stmt> init_body = {};
        std::vector<Param> init_params = {};
        bool has_init = false;
        std::vector<Stmt> methods = {};
        std::vector<std::string> let_field_names = {};
        std::vector<std::string> let_field_types = {};
        for (auto& st : body) {
            {
                auto& _match_26 = st;
                if (_match_26._tag == "Function") {
                    auto& _v = std::get<Stmt::Function>(_match_26._data);
                    auto& fname = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& fbody = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& is_comptime = _v.is_comptime;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    if (fname.lexeme == std::string("constructor")) {
                        has_init = true;
                        init_body = fbody;
                        init_params = params;
                    }
                     else {
                        methods.push_back(st);
                    }
                }
                else if (_match_26._tag == "Let") {
                    auto& _v = std::get<Stmt::Let>(_match_26._data);
                    auto& fname = _v.name;
                    auto& var_type = _v.var_type;
                    auto& init = _v.initializer;
                    auto& visibility = _v.visibility;
                    let_field_names.push_back(fname.lexeme);
                    let_field_types.push_back((*this).emit_type(var_type));
                }
                else {
                    int64_t noop = 0LL;
                }
            }
        }
        std::vector<std::string> init_field_names = {};
        std::vector<std::string> init_field_types = {};
        if (has_init) {
            std::vector<std::string> param_names = {};
            std::vector<std::string> param_types = {};
            for (auto& p : init_params) {
                param_names.push_back(p.name.lexeme);
                param_types.push_back((*this).emit_type(p.param_type));
            }
            std::vector<std::string> seen = {};
            for (auto& st : init_body) {
                {
                    auto& _match_27 = st;
                    if (_match_27._tag == "ExprStmt") {
                        auto& _v = std::get<Stmt::ExprStmt>(_match_27._data);
                        auto& expr = _v.expr;
                        {
                            auto& _match_28 = expr;
                            if (_match_28._tag == "Set") {
                                auto& _v = std::get<Expr::Set>(_match_28._data);
                                auto& object = *_v.object;
                                auto& prop = _v.name;
                                auto& value = *_v.value;
                                {
                                    auto& _match_29 = object;
                                    if (_match_29._tag == "This") {
                                        auto& _v = std::get<Expr::This>(_match_29._data);
                                        auto& kw = _v.keyword;
                                        bool already = false;
                                        for (auto& s : seen) {
                                            if (s == prop.lexeme) {
                                                already = true;
                                            }
                                        }
                                        if (!already) {
                                            seen.push_back(prop.lexeme);
                                            std::string cpp_type = (*this).infer_expr_type(value, param_names, param_types);
                                            init_field_names.push_back(prop.lexeme);
                                            init_field_types.push_back(cpp_type);
                                        }
                                    }
                                    else {
                                        int64_t noop = 0LL;
                                    }
                                }
                            }
                            else {
                                int64_t noop = 0LL;
                            }
                        }
                    }
                    else {
                        int64_t noop = 0LL;
                    }
                }
            }
        }
        this->output = this->output + (*this).indent() + std::string("struct ") + name.lexeme + std::string(" {\n");
        this->indent_level = this->indent_level + 1LL;
        int64_t fi = 0LL;
        while (fi < static_cast<int64_t>(init_field_names.size())) {
            bool is_let = false;
            for (auto& lfn : let_field_names) {
                if (lfn == init_field_names[fi]) {
                    is_let = true;
                }
            }
            if (!is_let) {
                this->output = this->output + (*this).indent() + init_field_types[fi] + std::string(" ") + init_field_names[fi] + std::string(";\n");
            }
            fi = fi + 1LL;
        }
        int64_t li = 0LL;
        while (li < static_cast<int64_t>(let_field_names.size())) {
            this->output = this->output + (*this).indent() + let_field_types[li] + std::string(" ") + let_field_names[li] + std::string(";\n");
            li = li + 1LL;
        }
        if (static_cast<int64_t>(init_field_names.size()) > 0LL || static_cast<int64_t>(let_field_names.size()) > 0LL) {
            this->output = this->output + std::string("\n");
        }
        if (has_init) {
            std::vector<std::string> param_strs = {};
            for (auto& p : init_params) {
                param_strs.push_back((*this).emit_type(p.param_type) + std::string(" ") + p.name.lexeme);
            }
            std::vector<std::string> init_list = {};
            std::vector<Stmt> remaining_body = {};
            for (auto& st : init_body) {
                {
                    auto& _match_30 = st;
                    if (_match_30._tag == "ExprStmt") {
                        auto& _v = std::get<Stmt::ExprStmt>(_match_30._data);
                        auto& expr = _v.expr;
                        bool handled = false;
                        {
                            auto& _match_31 = expr;
                            if (_match_31._tag == "Set") {
                                auto& _v = std::get<Expr::Set>(_match_31._data);
                                auto& object = *_v.object;
                                auto& prop = _v.name;
                                auto& value = *_v.value;
                                {
                                    auto& _match_32 = object;
                                    if (_match_32._tag == "This") {
                                        auto& _v = std::get<Expr::This>(_match_32._data);
                                        auto& kw = _v.keyword;
                                        {
                                            auto& _match_33 = value;
                                            if (_match_33._tag == "Variable") {
                                                auto& _v = std::get<Expr::Variable>(_match_33._data);
                                                auto& tok = _v.name;
                                                init_list.push_back(prop.lexeme + std::string("(") + tok.lexeme + std::string(")"));
                                                handled = true;
                                            }
                                            else {
                                                int64_t noop = 0LL;
                                            }
                                        }
                                    }
                                    else {
                                        int64_t noop = 0LL;
                                    }
                                }
                            }
                            else {
                                int64_t noop = 0LL;
                            }
                        }
                        if (!handled) {
                            remaining_body.push_back(st);
                        }
                    }
                    else {
                        remaining_body.push_back(st);
                    }
                }
            }
            if (static_cast<int64_t>(init_list.size()) == 0LL) {
                this->output = this->output + (*this).indent() + name.lexeme + std::string("(") + lv_join(param_strs, std::string(", ")) + std::string(") {\n");
            }
             else {
                this->output = this->output + (*this).indent() + name.lexeme + std::string("(") + lv_join(param_strs, std::string(", ")) + std::string(")\n");
                this->output = this->output + (*this).indent() + std::string("    : ") + lv_join(init_list, std::string(", ")) + std::string(" {\n");
            }
            this->indent_level = this->indent_level + 1LL;
            for (auto& st : remaining_body) {
                (*this).emit_method_stmt(st);
            }
            this->indent_level = this->indent_level - 1LL;
            this->output = this->output + (*this).indent() + std::string("}\n\n");
        }
        for (auto& m : methods) {
            {
                auto& _match_34 = m;
                if (_match_34._tag == "Function") {
                    auto& _v = std::get<Stmt::Function>(_match_34._data);
                    auto& mname = _v.name;
                    auto& mparams = _v.params;
                    auto& mret = _v.return_type;
                    auto& mbody = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& is_comptime = _v.is_comptime;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    std::string ret_type = (*this).emit_type(mret);
                    std::vector<std::string> mparam_strs = {};
                    for (auto& p : mparams) {
                        mparam_strs.push_back((*this).emit_type(p.param_type) + std::string(" ") + p.name.lexeme);
                        {
                            auto& _match_35 = p.param_type;
                            if (_match_35._tag == "Dynamic") {
                                (*this).add_dynamic_var(p.name.lexeme);
                            }
                            else {
                                int64_t noop = 0LL;
                            }
                        }
                    }
                    if (mname.lexeme == std::string("destructor")) {
                        this->output = this->output + (*this).indent() + std::string("~") + name.lexeme + std::string("() {\n");
                    }
                     else {
                        this->output = this->output + (*this).indent() + ret_type + std::string(" ") + mname.lexeme + std::string("(") + lv_join(mparam_strs, std::string(", ")) + std::string(") {\n");
                    }
                    this->indent_level = this->indent_level + 1LL;
                    for (auto& st : mbody) {
                        (*this).emit_method_stmt(st);
                    }
                    this->indent_level = this->indent_level - 1LL;
                    this->output = this->output + (*this).indent() + std::string("}\n\n");
                }
                else {
                    int64_t noop = 0LL;
                }
            }
        }
        this->indent_level = this->indent_level - 1LL;
        this->output = this->output + (*this).indent() + std::string("};\n\n");
    }

    std::string infer_expr_type(Expr e, std::vector<std::string> param_names, std::vector<std::string> param_types) {
        {
            auto& _match_36 = e;
            if (_match_36._tag == "Literal") {
                auto& _v = std::get<Expr::Literal>(_match_36._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if (kind == std::string("int")) {
                    return std::string("int64_t");
                }
                 else {
                    if (kind == std::string("float")) {
                        return std::string("double");
                    }
                     else {
                        if (kind == std::string("string")) {
                            return std::string("std::string");
                        }
                         else {
                            if (kind == std::string("bool")) {
                                return std::string("bool");
                            }
                        }
                    }
                }
                return std::string("std::any");
            }
            else if (_match_36._tag == "Variable") {
                auto& _v = std::get<Expr::Variable>(_match_36._data);
                auto& tok = _v.name;
                int64_t i = 0LL;
                while (i < static_cast<int64_t>(param_names.size())) {
                    if (param_names[i] == tok.lexeme) {
                        if (param_types[i] == std::string("auto")) {
                            return std::string("std::any");
                        }
                        return param_types[i];
                    }
                    i = i + 1LL;
                }
                return std::string("std::any");
            }
            else if (_match_36._tag == "Vector") {
                auto& _v = std::get<Expr::Vector>(_match_36._data);
                auto& elements = _v.elements;
                return std::string("std::vector<std::any>");
            }
            else {
                return std::string("std::any");
            }
        }
    }

    void emit_enum(Token name, std::vector<EnumVariantNode> variants) {
        std::string enum_name = name.lexeme;
        this->known_enum_names.push_back(enum_name);
        this->known_enum_variants.push_back(variants);
        bool has_self_ref = false;
        for (auto& v : variants) {
            for (auto& ft : v.types) {
                {
                    auto& _match_37 = ft;
                    if (_match_37._tag == "Custom") {
                        auto& _v = std::get<TypeNode::Custom>(_match_37._data);
                        auto& n = _v.name;
                        if (n == enum_name) {
                            has_self_ref = true;
                        }
                    }
                    else {
                        int64_t noop = 0LL;
                    }
                }
            }
        }
        if (has_self_ref) {
            this->output = this->output + (*this).indent() + std::string("struct ") + enum_name + std::string(";\n");
        }
        this->output = this->output + (*this).indent() + std::string("struct ") + enum_name + std::string(" {\n");
        this->indent_level = this->indent_level + 1LL;
        for (auto& v : variants) {
            if (static_cast<int64_t>(v.types.size()) > 0LL) {
                this->output = this->output + (*this).indent() + std::string("struct ") + v.name.lexeme + std::string(" { ");
                int64_t fi = 0LL;
                while (fi < static_cast<int64_t>(v.types.size())) {
                    std::string cpp_type = (*this).emit_type(v.types[fi]);
                    std::string fname = v.field_names[fi];
                    {
                        auto& _match_38 = v.types[fi];
                        if (_match_38._tag == "Custom") {
                            auto& _v = std::get<TypeNode::Custom>(_match_38._data);
                            auto& n = _v.name;
                            if (n == enum_name) {
                                cpp_type = std::string("std::shared_ptr<") + enum_name + std::string(">");
                            }
                        }
                        else {
                            int64_t noop = 0LL;
                        }
                    }
                    this->output = this->output + cpp_type + std::string(" ") + fname + std::string("; ");
                    fi = fi + 1LL;
                }
                this->output = this->output + std::string("};\n");
            }
        }
        this->output = this->output + std::string("\n");
        this->output = this->output + (*this).indent() + std::string("std::string _tag;\n");
        std::vector<std::string> variant_inner_types = {};
        for (auto& v : variants) {
            if (static_cast<int64_t>(v.types.size()) > 0LL) {
                variant_inner_types.push_back(enum_name + std::string("::") + v.name.lexeme);
            }
        }
        if (static_cast<int64_t>(variant_inner_types.size()) > 0LL) {
            std::string all_types = std::string("std::monostate, ") + lv_join(variant_inner_types, std::string(", "));
            this->output = this->output + (*this).indent() + std::string("std::variant<") + all_types + std::string("> _data;\n");
        }
        this->output = this->output + std::string("\n");
        for (auto& v : variants) {
            std::string vname = v.name.lexeme;
            if (static_cast<int64_t>(v.types.size()) == 0LL) {
                std::string mono = std::string("");
                if (static_cast<int64_t>(variant_inner_types.size()) > 0LL) {
                    mono = std::string(", std::monostate{}");
                }
                this->output = this->output + (*this).indent() + std::string("static ") + enum_name + std::string(" make_") + vname + std::string("() { return {\"") + vname + std::string("\"") + mono + std::string("}; }\n");
            }
             else {
                std::vector<std::string> params = {};
                std::vector<std::string> field_inits = {};
                int64_t fi = 0LL;
                while (fi < static_cast<int64_t>(v.types.size())) {
                    std::string cpp_type = (*this).emit_type(v.types[fi]);
                    std::string fname = v.field_names[fi];
                    bool is_self_ref = false;
                    {
                        auto& _match_39 = v.types[fi];
                        if (_match_39._tag == "Custom") {
                            auto& _v = std::get<TypeNode::Custom>(_match_39._data);
                            auto& n = _v.name;
                            if (n == enum_name) {
                                is_self_ref = true;
                            }
                        }
                        else {
                            int64_t noop = 0LL;
                        }
                    }
                    if (is_self_ref) {
                        params.push_back(enum_name + std::string(" ") + fname);
                        field_inits.push_back(std::string("std::make_shared<") + enum_name + std::string(">(std::move(") + fname + std::string("))"));
                    }
                     else {
                        params.push_back(cpp_type + std::string(" ") + fname);
                        field_inits.push_back(fname);
                    }
                    fi = fi + 1LL;
                }
                this->output = this->output + (*this).indent() + std::string("static ") + enum_name + std::string(" make_") + vname + std::string("(") + lv_join(params, std::string(", ")) + std::string(") { return {\"") + vname + std::string("\", ") + vname + std::string("{") + lv_join(field_inits, std::string(", ")) + std::string("}}; }\n");
            }
        }
        this->output = this->output + std::string("\n");
        this->output = this->output + (*this).indent() + std::string("std::string operator[](const std::string& key) const {\n");
        this->indent_level = this->indent_level + 1LL;
        this->output = this->output + (*this).indent() + std::string("if (key == \"_tag\") return _tag;\n");
        this->output = this->output + (*this).indent() + std::string("return \"\";\n");
        this->indent_level = this->indent_level - 1LL;
        this->output = this->output + (*this).indent() + std::string("}\n");
        this->indent_level = this->indent_level - 1LL;
        this->output = this->output + (*this).indent() + std::string("};\n\n");
        this->output = this->output + std::string("void print(const ") + enum_name + std::string("& e) { std::cout << \"") + enum_name + std::string("(\" << e._tag << \")\" << std::endl; }\n");
        this->output = this->output + std::string("std::string operator+(const std::string& s, const ") + enum_name + std::string("& e) { return s + e._tag; }\n");
        this->output = this->output + std::string("std::string operator+(const ") + enum_name + std::string("& e, const std::string& s) { return e._tag + s; }\n\n");
    }

    void emit_match(Expr expr, std::vector<MatchArm> arm_patterns, std::vector<Stmt> arm_bodies) {
        (*this).emit_match_impl(expr, arm_patterns, arm_bodies, false);
    }

    void emit_match_in_method(Expr expr, std::vector<MatchArm> arm_patterns, std::vector<Stmt> arm_bodies) {
        (*this).emit_match_impl(expr, arm_patterns, arm_bodies, true);
    }

    void emit_match_impl(Expr expr, std::vector<MatchArm> arm_patterns, std::vector<Stmt> arm_bodies, bool in_method) {
        std::string match_val = std::string("");
        if (in_method) {
            match_val = (*this).emit_method_expr(expr);
        }
         else {
            match_val = (*this).emit_expr(expr);
        }
        std::string temp = std::string("_match_") + this->temp_counter;
        this->temp_counter = this->temp_counter + 1LL;
        this->output = this->output + (*this).indent() + std::string("{\n");
        this->indent_level = this->indent_level + 1LL;
        this->output = this->output + (*this).indent() + std::string("auto& ") + temp + std::string(" = ") + match_val + std::string(";\n");
        bool first = true;
        int64_t ai = 0LL;
        while (ai < static_cast<int64_t>(arm_patterns.size())) {
            auto arm = arm_patterns[ai];
            Stmt arm_body = arm_bodies[ai];
            if (arm.pattern_name == std::string("_")) {
                if (first) {
                    this->output = this->output + (*this).indent() + std::string("{\n");
                }
                 else {
                    this->output = this->output + (*this).indent() + std::string("else {\n");
                }
                this->indent_level = this->indent_level + 1LL;
                (*this).emit_arm_body(arm_body, in_method);
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
             else {
                std::string keyword = std::string("if");
                if (!first) {
                    keyword = std::string("else if");
                }
                first = false;
                this->output = this->output + (*this).indent() + keyword + std::string(" (") + temp + std::string("._tag == \"") + arm.pattern_name + std::string("\") {\n");
                this->indent_level = this->indent_level + 1LL;
                if (static_cast<int64_t>(arm.bindings.size()) > 0LL) {
                    std::string ename = (*this).find_enum_for_variant(arm.pattern_name);
                    if (ename != std::string("")) {
                        this->output = this->output + (*this).indent() + std::string("auto& _v = std::get<") + ename + std::string("::") + arm.pattern_name + std::string(">(") + temp + std::string("._data);\n");
                        auto vinfo = (*this).get_variant_info(ename, arm.pattern_name);
                        int64_t bi = 0LL;
                        while (bi < static_cast<int64_t>(arm.bindings.size())) {
                            if (bi < static_cast<int64_t>(vinfo.field_names.size())) {
                                std::string field_name = vinfo.field_names[bi];
                                bool is_self_ref = false;
                                if (bi < static_cast<int64_t>(vinfo.types.size())) {
                                    {
                                        auto& _match_40 = vinfo.types[bi];
                                        if (_match_40._tag == "Custom") {
                                            auto& _v = std::get<TypeNode::Custom>(_match_40._data);
                                            auto& n = _v.name;
                                            if (n == ename) {
                                                is_self_ref = true;
                                            }
                                        }
                                        else {
                                            int64_t noop = 0LL;
                                        }
                                    }
                                }
                                if (is_self_ref) {
                                    this->output = this->output + (*this).indent() + std::string("auto& ") + arm.bindings[bi] + std::string(" = *_v.") + field_name + std::string(";\n");
                                }
                                 else {
                                    this->output = this->output + (*this).indent() + std::string("auto& ") + arm.bindings[bi] + std::string(" = _v.") + field_name + std::string(";\n");
                                }
                            }
                            bi = bi + 1LL;
                        }
                    }
                }
                (*this).emit_arm_body(arm_body, in_method);
                this->indent_level = this->indent_level - 1LL;
                this->output = this->output + (*this).indent() + std::string("}\n");
            }
            ai = ai + 1LL;
        }
        this->indent_level = this->indent_level - 1LL;
        this->output = this->output + (*this).indent() + std::string("}\n");
    }

    void emit_arm_body(Stmt arm_body, bool in_method) {
        {
            auto& _match_41 = arm_body;
            if (_match_41._tag == "Block") {
                auto& _v = std::get<Stmt::Block>(_match_41._data);
                auto& stmts = _v.statements;
                for (auto& st : stmts) {
                    if (in_method) {
                        (*this).emit_method_stmt(st);
                    }
                     else {
                        (*this).emit_stmt(st);
                    }
                }
            }
            else {
                if (in_method) {
                    (*this).emit_method_stmt(arm_body);
                }
                 else {
                    (*this).emit_stmt(arm_body);
                }
            }
        }
    }

    std::string generate(std::vector<Stmt> stmts) {
        this->declarations = std::string("#include \"lavina.h\"\n\n");
        for (auto& stmt : stmts) {
            {
                auto& _match_42 = stmt;
                if (_match_42._tag == "Function") {
                    auto& _v = std::get<Stmt::Function>(_match_42._data);
                    auto& name = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& body = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& is_comptime = _v.is_comptime;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    if (name.lexeme == std::string("main")) {
                        this->has_main = true;
                        this->output = this->output + std::string("int main(int argc, char* argv[]) {\n");
                        this->output = this->output + std::string("    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);\n");
                        this->indent_level = 1LL;
                        for (auto& s : body) {
                            (*this).emit_stmt(s);
                        }
                        this->indent_level = 0LL;
                        this->output = this->output + std::string("}\n");
                        this->declarations = this->declarations + this->output;
                        this->output = std::string("");
                    }
                     else {
                        (*this).emit_stmt(stmt);
                        this->declarations = this->declarations + this->output;
                        this->output = std::string("");
                    }
                }
                else {
                    (*this).emit_stmt(stmt);
                    this->declarations = this->declarations + this->output;
                    this->output = std::string("");
                }
            }
        }
        return this->declarations;
    }

};

struct ImportResolver {
    std::vector<std::string> resolved_paths;

    ImportResolver() {
        this->resolved_paths = {};
    }

    bool already_resolved(std::string path) {
        for (auto& p : this->resolved_paths) {
            if (p == path) {
                return true;
            }
        }
        return false;
    }

    std::string get_directory(std::string path) {
        int64_t i = static_cast<int64_t>(path.size()) - 1LL;
        while (i >= 0LL) {
            if (std::string(1, path[i]) == std::string("/")) {
                return path.substr(0LL, (i + 1LL) - (0LL));
            }
            i = i - 1LL;
        }
        return std::string("./");
    }

    std::string resolve(std::string file_path) {
        if ((*this).already_resolved(file_path)) {
            return std::string("");
        }
        this->resolved_paths.push_back(file_path);
        std::string source = fs_read(file_path);
        std::string dir = (*this).get_directory(file_path);
        std::string result = std::string("");
        std::vector<std::string> lines = lv_split(source, std::string("\n"));
        for (auto& line : lines) {
            std::string trimmed = lv_trim(line);
            if (trimmed.starts_with(std::string("import "))) {
                std::string module_name = lv_trim(trimmed.substr(7LL, (static_cast<int64_t>(trimmed.size())) - (7LL)));
                std::string module_path = dir + module_name + std::string(".lv");
                std::string resolved = (*this).resolve(module_path);
                if (resolved != std::string("")) {
                    result = result + resolved + std::string("\n");
                }
            }
             else {
                result = result + line + std::string("\n");
            }
        }
        return result;
    }

};

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);
    auto args = os_args();
    if (static_cast<int64_t>(args.size()) < 2LL) {
        print(std::string("Usage: bootstrap [--emit-cpp | compile] <file.lv>"));
        return 1LL;
    }
    std::string mode = std::string("run");
    std::string path = std::string("");
    if (static_cast<int64_t>(args.size()) == 2LL) {
        path = args[1LL];
    }
     else {
        if (static_cast<int64_t>(args.size()) == 3LL) {
            if (args[1LL] == std::string("--emit-cpp")) {
                mode = std::string("emit-cpp");
                path = args[2LL];
            }
             else {
                if (args[1LL] == std::string("compile")) {
                    mode = std::string("compile");
                    path = args[2LL];
                }
                 else {
                    print(std::string("Unknown option: ") + args[1LL]);
                    return 1LL;
                }
            }
        }
    }
    auto resolver = ImportResolver();
    std::string source = resolver.resolve(path);
    auto scanner = Scanner(source);
    scanner.scan_tokens();
    if (static_cast<int64_t>(scanner.errors.size()) > 0LL) {
        print(std::string("Scanner errors:"));
        for (auto& err : scanner.errors) {
            print(err);
        }
        return 1LL;
    }
    auto parser = Parser(scanner.tokens);
    std::vector<Stmt> stmts = parser.parse_program();
    auto codegen = CppCodegen();
    std::string cpp = codegen.generate(stmts);
    if (mode == std::string("emit-cpp")) {
        print(cpp);
        return 0LL;
    }
    if (!codegen.has_main) {
        print(std::string("Error: no main() function defined."));
        return 1LL;
    }
    std::string dir = resolver.get_directory(path);
    std::string base = path;
    int64_t si = static_cast<int64_t>(path.size()) - 1LL;
    while (si >= 0LL) {
        if (std::string(1, path[si]) == std::string("/")) {
            base = path.substr(si + 1LL, (static_cast<int64_t>(path.size())) - (si + 1LL));
            break;
        }
        si = si - 1LL;
    }
    if (base.ends_with(std::string(".lv"))) {
        base = base.substr(0LL, (static_cast<int64_t>(base.size()) - 3LL) - (0LL));
    }
    std::string cpp_path = dir + base + std::string(".cpp");
    std::string bin_path = dir + base;
    std::string header_path = dir + std::string("lavina.h");
    fs_write(cpp_path, cpp);
    bool wrote_header = false;
    if (!fs_exists(header_path)) {
        try {
            std::string header_content = fs_read(std::string("runtime/lavina.h"));
            fs_write(header_path, header_content);
            wrote_header = true;
        }
         catch (const std::exception& e) {
            print(std::string("Warning: could not find runtime/lavina.h"));
        }
    }
    std::string compile_cmd = std::string("g++ -std=c++23 -o ") + bin_path + std::string(" ") + cpp_path;
    int64_t compile_result = os_exec(compile_cmd);
    if (compile_result != 0LL) {
        print(std::string("Compilation failed"));
        os_exec(std::string("rm -f ") + cpp_path);
        if (wrote_header) {
            os_exec(std::string("rm -f ") + header_path);
        }
        return 1LL;
    }
    if (mode == std::string("compile")) {
        os_exec(std::string("rm -f ") + cpp_path);
        if (wrote_header) {
            os_exec(std::string("rm -f ") + header_path);
        }
        print(std::string("Compiled: ") + bin_path);
        return 0LL;
    }
    int64_t run_result = os_exec(bin_path);
    os_exec(std::string("rm -f ") + cpp_path);
    os_exec(std::string("rm -f ") + bin_path);
    if (wrote_header) {
        os_exec(std::string("rm -f ") + header_path);
    }
    if (run_result != 0LL) {
        return 1LL;
    }
    return 0LL;
}

