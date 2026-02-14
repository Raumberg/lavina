// Stage Latest
#include "lavina.h"

const std::string TK_LEFT_PAREN = std::string("LeftParen");
const std::string TK_RIGHT_PAREN = std::string("RightParen");
const std::string TK_LEFT_BRACKET = std::string("LeftBracket");
const std::string TK_RIGHT_BRACKET = std::string("RightBracket");
const std::string TK_LEFT_BRACE = std::string("LeftBrace");
const std::string TK_RIGHT_BRACE = std::string("RightBrace");
const std::string TK_COMMA = std::string("Comma");
const std::string TK_DOT = std::string("Dot");
const std::string TK_DOT_DOT = std::string("DotDot");
const std::string TK_MINUS = std::string("Minus");
const std::string TK_PLUS = std::string("Plus");
const std::string TK_SEMICOLON = std::string("Semicolon");
const std::string TK_SLASH = std::string("Slash");
const std::string TK_STAR = std::string("Star");
const std::string TK_COLON = std::string("Colon");
const std::string TK_HASH = std::string("Hash");
const std::string TK_QUESTION = std::string("Question");
const std::string TK_PERCENT = std::string("Percent");
const std::string TK_PLUS_EQUAL = std::string("PlusEqual");
const std::string TK_MINUS_EQUAL = std::string("MinusEqual");
const std::string TK_STAR_EQUAL = std::string("StarEqual");
const std::string TK_BANG = std::string("Bang");
const std::string TK_BANG_EQUAL = std::string("BangEqual");
const std::string TK_EQUAL = std::string("Equal");
const std::string TK_EQUAL_EQUAL = std::string("EqualEqual");
const std::string TK_GREATER = std::string("Greater");
const std::string TK_GREATER_EQUAL = std::string("GreaterEqual");
const std::string TK_LESS = std::string("Less");
const std::string TK_LESS_EQUAL = std::string("LessEqual");
const std::string TK_ARROW = std::string("Arrow");
const std::string TK_FAT_ARROW = std::string("FatArrow");
const std::string TK_DOUBLE_COLON = std::string("DoubleColon");
const std::string TK_IDENTIFIER = std::string("Identifier");
const std::string TK_STRING = std::string("String");
const std::string TK_INT = std::string("Int");
const std::string TK_FLOAT = std::string("Float");
const std::string TK_AND = std::string("And");
const std::string TK_AUTO = std::string("Auto");
const std::string TK_BOOL = std::string("Bool");
const std::string TK_COMPTIME = std::string("Comptime");
const std::string TK_COMPTIME_STRICT = std::string("ComptimeStrict");
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
const std::string TK_REF = std::string("Ref");
const std::string TK_REF_MUT = std::string("RefMut");
const std::string TK_OWN = std::string("Own");
const std::string TK_PASS = std::string("Pass");
const std::string TK_INT8 = std::string("Int8");
const std::string TK_INT16 = std::string("Int16");
const std::string TK_INT32 = std::string("Int32");
const std::string TK_INT64 = std::string("Int64");
const std::string TK_FLOAT32 = std::string("Float32");
const std::string TK_FLOAT64 = std::string("Float64");
const std::string TK_USIZE = std::string("USize");
const std::string TK_CSTRING = std::string("CString");
const std::string TK_PTR = std::string("Ptr");
const std::string TK_CPP = std::string("Cpp");
const std::string TK_AMPERSAND = std::string("Ampersand");
const std::string TK_EXTERN = std::string("Extern");
const std::string TK_LINK = std::string("Link");
const std::string TK_OPERATOR = std::string("Operator");
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

    std::string to_string() const {
        return ((((((((std::string("") + (this->token_type)) + std::string("(")) + (this->lexeme)) + std::string(") at ")) + (this->line)) + std::string(":")) + (this->col)) + std::string(""));
    }

};

void print(const Token& _v) { std::cout << _v.to_string() << std::endl; }
std::string operator+(const std::string& _s, const Token& _v) { return _s + _v.to_string(); }
std::string operator+(const Token& _v, const std::string& _s) { return _v.to_string() + _s; }

std::string lookup_keyword(const std::string& w) {
    if ((w == std::string("and"))) {
        return TK_AND;
    }
    else {
        if ((w == std::string("auto"))) {
            return TK_AUTO;
        }
        else {
            if ((w == std::string("bool"))) {
                return TK_BOOL;
            }
            else {
                if ((w == std::string("comptime"))) {
                    return TK_COMPTIME;
                }
                else {
                    if ((w == std::string("dynamic"))) {
                        return TK_DYNAMIC;
                    }
                    else {
                        if ((w == std::string("else"))) {
                            return TK_ELSE;
                        }
                        else {
                            if ((w == std::string("elif"))) {
                                return TK_ELIF;
                            }
                            else {
                                if ((w == std::string("false"))) {
                                    return TK_FALSE;
                                }
                                else {
                                    if ((w == std::string("float"))) {
                                        return TK_FLOAT_TYPE;
                                    }
                                    else {
                                        if ((w == std::string("fn"))) {
                                            return TK_FN;
                                        }
                                        else {
                                            if ((w == std::string("if"))) {
                                                return TK_IF;
                                            }
                                            else {
                                                if ((w == std::string("inline"))) {
                                                    return TK_INLINE;
                                                }
                                                else {
                                                    if ((w == std::string("int"))) {
                                                        return TK_INT_TYPE;
                                                    }
                                                    else {
                                                        if ((w == std::string("null"))) {
                                                            return TK_NULL;
                                                        }
                                                        else {
                                                            if ((w == std::string("or"))) {
                                                                return TK_OR;
                                                            }
                                                            else {
                                                                if ((w == std::string("return"))) {
                                                                    return TK_RETURN;
                                                                }
                                                                else {
                                                                    if ((w == std::string("string"))) {
                                                                        return TK_STRING_TYPE;
                                                                    }
                                                                    else {
                                                                        if ((w == std::string("true"))) {
                                                                            return TK_TRUE;
                                                                        }
                                                                        else {
                                                                            if ((w == std::string("void"))) {
                                                                                return TK_VOID;
                                                                            }
                                                                            else {
                                                                                if ((w == std::string("while"))) {
                                                                                    return TK_WHILE;
                                                                                }
                                                                                else {
                                                                                    if ((w == std::string("for"))) {
                                                                                        return TK_FOR;
                                                                                    }
                                                                                    else {
                                                                                        if ((w == std::string("in"))) {
                                                                                            return TK_IN;
                                                                                        }
                                                                                        else {
                                                                                            if ((w == std::string("vector"))) {
                                                                                                return TK_VECTOR;
                                                                                            }
                                                                                            else {
                                                                                                if ((w == std::string("hashmap"))) {
                                                                                                    return TK_HASHMAP;
                                                                                                }
                                                                                                else {
                                                                                                    if ((w == std::string("hashset"))) {
                                                                                                        return TK_HASHSET;
                                                                                                    }
                                                                                                    else {
                                                                                                        if ((w == std::string("import"))) {
                                                                                                            return TK_IMPORT;
                                                                                                        }
                                                                                                        else {
                                                                                                            if ((w == std::string("as"))) {
                                                                                                                return TK_AS;
                                                                                                            }
                                                                                                            else {
                                                                                                                if ((w == std::string("namespace"))) {
                                                                                                                    return TK_NAMESPACE;
                                                                                                                }
                                                                                                                else {
                                                                                                                    if ((w == std::string("public"))) {
                                                                                                                        return TK_PUBLIC;
                                                                                                                    }
                                                                                                                    else {
                                                                                                                        if ((w == std::string("private"))) {
                                                                                                                            return TK_PRIVATE;
                                                                                                                        }
                                                                                                                        else {
                                                                                                                            if ((w == std::string("static"))) {
                                                                                                                                return TK_STATIC;
                                                                                                                            }
                                                                                                                            else {
                                                                                                                                if ((w == std::string("class"))) {
                                                                                                                                    return TK_CLASS;
                                                                                                                                }
                                                                                                                                else {
                                                                                                                                    if ((w == std::string("struct"))) {
                                                                                                                                        return TK_STRUCT;
                                                                                                                                    }
                                                                                                                                    else {
                                                                                                                                        if ((w == std::string("enum"))) {
                                                                                                                                            return TK_ENUM;
                                                                                                                                        }
                                                                                                                                        else {
                                                                                                                                            if ((w == std::string("this"))) {
                                                                                                                                                return TK_THIS;
                                                                                                                                            }
                                                                                                                                            else {
                                                                                                                                                if ((w == std::string("try"))) {
                                                                                                                                                    return TK_TRY;
                                                                                                                                                }
                                                                                                                                                else {
                                                                                                                                                    if ((w == std::string("catch"))) {
                                                                                                                                                        return TK_CATCH;
                                                                                                                                                    }
                                                                                                                                                    else {
                                                                                                                                                        if ((w == std::string("throw"))) {
                                                                                                                                                            return TK_THROW;
                                                                                                                                                        }
                                                                                                                                                        else {
                                                                                                                                                            if ((w == std::string("const"))) {
                                                                                                                                                                return TK_CONST;
                                                                                                                                                            }
                                                                                                                                                            else {
                                                                                                                                                                if ((w == std::string("let"))) {
                                                                                                                                                                    return TK_LET;
                                                                                                                                                                }
                                                                                                                                                                else {
                                                                                                                                                                    if ((w == std::string("match"))) {
                                                                                                                                                                        return TK_MATCH;
                                                                                                                                                                    }
                                                                                                                                                                    else {
                                                                                                                                                                        if ((w == std::string("break"))) {
                                                                                                                                                                            return TK_BREAK;
                                                                                                                                                                        }
                                                                                                                                                                        else {
                                                                                                                                                                            if ((w == std::string("continue"))) {
                                                                                                                                                                                return TK_CONTINUE;
                                                                                                                                                                            }
                                                                                                                                                                            else {
                                                                                                                                                                                if ((w == std::string("not"))) {
                                                                                                                                                                                    return TK_NOT;
                                                                                                                                                                                }
                                                                                                                                                                                else {
                                                                                                                                                                                    if ((w == std::string("ref"))) {
                                                                                                                                                                                        return TK_REF;
                                                                                                                                                                                    }
                                                                                                                                                                                    else {
                                                                                                                                                                                        if ((w == std::string("own"))) {
                                                                                                                                                                                            return TK_OWN;
                                                                                                                                                                                        }
                                                                                                                                                                                        else {
                                                                                                                                                                                            if ((w == std::string("pass"))) {
                                                                                                                                                                                                return TK_PASS;
                                                                                                                                                                                            }
                                                                                                                                                                                            else {
                                                                                                                                                                                                if ((w == std::string("int8"))) {
                                                                                                                                                                                                    return TK_INT8;
                                                                                                                                                                                                }
                                                                                                                                                                                                else {
                                                                                                                                                                                                    if ((w == std::string("int16"))) {
                                                                                                                                                                                                        return TK_INT16;
                                                                                                                                                                                                    }
                                                                                                                                                                                                    else {
                                                                                                                                                                                                        if ((w == std::string("int32"))) {
                                                                                                                                                                                                            return TK_INT32;
                                                                                                                                                                                                        }
                                                                                                                                                                                                        else {
                                                                                                                                                                                                            if ((w == std::string("int64"))) {
                                                                                                                                                                                                                return TK_INT64;
                                                                                                                                                                                                            }
                                                                                                                                                                                                            else {
                                                                                                                                                                                                                if ((w == std::string("float32"))) {
                                                                                                                                                                                                                    return TK_FLOAT32;
                                                                                                                                                                                                                }
                                                                                                                                                                                                                else {
                                                                                                                                                                                                                    if ((w == std::string("float64"))) {
                                                                                                                                                                                                                        return TK_FLOAT64;
                                                                                                                                                                                                                    }
                                                                                                                                                                                                                    else {
                                                                                                                                                                                                                        if ((w == std::string("usize"))) {
                                                                                                                                                                                                                            return TK_USIZE;
                                                                                                                                                                                                                        }
                                                                                                                                                                                                                        else {
                                                                                                                                                                                                                            if ((w == std::string("cstring"))) {
                                                                                                                                                                                                                                return TK_CSTRING;
                                                                                                                                                                                                                            }
                                                                                                                                                                                                                            else {
                                                                                                                                                                                                                                if ((w == std::string("ptr"))) {
                                                                                                                                                                                                                                    return TK_PTR;
                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                else {
                                                                                                                                                                                                                                    if ((w == std::string("cpp"))) {
                                                                                                                                                                                                                                        return TK_CPP;
                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                    else {
                                                                                                                                                                                                                                        if ((w == std::string("extern"))) {
                                                                                                                                                                                                                                            return TK_EXTERN;
                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                        else {
                                                                                                                                                                                                                                            if ((w == std::string("link"))) {
                                                                                                                                                                                                                                                return TK_LINK;
                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                            else {
                                                                                                                                                                                                                                                if ((w == std::string("operator"))) {
                                                                                                                                                                                                                                                    return TK_OPERATOR;
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
    if ((c >= std::string("a")) && (c <= std::string("z"))) {
        return true;
    }
    else {
        if ((c >= std::string("A")) && (c <= std::string("Z"))) {
            return true;
        }
        else {
            if ((c == std::string("_"))) {
                return true;
            }
        }
    }
    return false;
}

bool is_digit(auto c) {
    if ((c >= std::string("0")) && (c <= std::string("9"))) {
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
        return (this->current >= static_cast<int64_t>(this->source.size()));
    }

    auto peek() {
        if ((*this).is_at_end()) {
            return std::string("");
        }
        return std::string(1, this->source[this->current]);
    }

    auto peek_next() {
        if (((this->current + 1LL) >= static_cast<int64_t>(this->source.size()))) {
            return std::string("");
        }
        return std::string(1, this->source[(this->current + 1LL)]);
    }

    auto advance() {
        auto c = std::string(1, this->source[this->current]);
        this->current = (this->current + 1LL);
        this->column = (this->column + 1LL);
        return c;
    }

    bool match_char(std::string expected) {
        if ((*this).is_at_end()) {
            return false;
        }
        if ((std::string(1, this->source[this->current]) != expected)) {
            return false;
        }
        this->current = (this->current + 1LL);
        this->column = (this->column + 1LL);
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
        auto err = ((((((std::string("Error at line ") + (this->line)) + std::string(":")) + (this->column)) + std::string(": ")) + (message)) + std::string(""));
        this->errors.push_back(err);
    }

    void handle_indentation() {
        int64_t indent = 0LL;
        while ((!(*this).is_at_end()) && (((*this).peek() == std::string(" ")) || ((*this).peek() == std::string("\t")))) {
            auto c = (*this).advance();
            if ((c == std::string(" "))) {
                indent = (indent + 1LL);
            }
            else {
                indent = (indent + 4LL);
            }
        }
        if ((*this).is_at_end() || ((*this).peek() == std::string("\n"))) {
            if (((*this).peek() == std::string("\n"))) {
                this->at_line_start = true;
            }
            return;
        }
        auto current_indent = this->indent_stack[(static_cast<int64_t>(this->indent_stack.size()) - 1LL)];
        if ((indent > current_indent)) {
            this->indent_stack.push_back(indent);
            (*this).add_token(TK_INDENT, std::string(""));
        }
        else {
            if ((indent < current_indent)) {
                while ((indent < this->indent_stack[(static_cast<int64_t>(this->indent_stack.size()) - 1LL)])) {
                    lv_pop(this->indent_stack);
                    (*this).add_token(TK_DEDENT, std::string(""));
                }
            }
        }
        this->at_line_start = false;
    }

    void scan_string() {
        int64_t str_start = this->current;
        while ((!(*this).is_at_end()) && ((*this).peek() != std::string("\""))) {
            if (((*this).peek() == std::string("\\"))) {
                (*this).advance();
                (*this).advance();
            }
            else {
                if (((*this).peek() == std::string("$")) && ((*this).peek_next() == std::string("{"))) {
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
                    if (((*this).peek() == std::string("\n"))) {
                        this->line = (this->line + 1LL);
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
        if (((*this).peek() == std::string(".")) && is_digit((*this).peek_next())) {
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
        if ((kw == TK_COMPTIME) && ((*this).peek() == std::string("!"))) {
            (*this).advance();
            (*this).add_token(TK_COMPTIME_STRICT, std::string("comptime!"));
        }
        else {
            if ((kw == TK_REF) && ((*this).peek() == std::string("!"))) {
                (*this).advance();
                (*this).add_token(TK_REF_MUT, std::string("ref!"));
            }
            else {
                if ((kw == TK_CPP)) {
                    int64_t save = this->current;
                    int64_t save_col = this->column;
                    while ((!(*this).is_at_end()) && (((*this).peek() == std::string(" ")) || ((*this).peek() == std::string("\t")))) {
                        (*this).advance();
                    }
                    if ((!(*this).is_at_end()) && ((*this).peek() == std::string("{"))) {
                        this->current = save;
                        this->column = save_col;
                        (*this).scan_cpp_block();
                    }
                    else {
                        this->current = save;
                        this->column = save_col;
                        (*this).add_token(TK_IDENTIFIER, text);
                    }
                }
                else {
                    if ((kw != std::string(""))) {
                        (*this).add_token(kw, text);
                    }
                    else {
                        (*this).add_token(TK_IDENTIFIER, text);
                    }
                }
            }
        }
    }

    void scan_cpp_block() {
        while ((!(*this).is_at_end()) && (((*this).peek() == std::string(" ")) || ((*this).peek() == std::string("\t")))) {
            (*this).advance();
        }
        if ((*this).is_at_end() || ((*this).peek() != std::string("{"))) {
            (*this).add_error(std::string("Expect '{' after 'cpp'"));
            return;
        }
        (*this).advance();
        int64_t depth = 1LL;
        int64_t content_start = this->current;
        while ((!(*this).is_at_end()) && (depth > 0LL)) {
            auto c = (*this).advance();
            if ((c == std::string("{"))) {
                depth = (depth + 1LL);
            }
            else {
                if ((c == std::string("}"))) {
                    depth = (depth - 1LL);
                }
                else {
                    if ((c == std::string("\n"))) {
                        this->line = (this->line + 1LL);
                        this->column = 1LL;
                    }
                }
            }
        }
        if ((depth != 0LL)) {
            (*this).add_error(std::string("Unterminated cpp block"));
            return;
        }
        auto content = this->source.substr(content_start, ((this->current - 1LL)) - (content_start));
        (*this).add_token(TK_CPP, content);
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
        if ((c == std::string("("))) {
            (*this).add_simple_token(TK_LEFT_PAREN);
        }
        else {
            if ((c == std::string(")"))) {
                (*this).add_simple_token(TK_RIGHT_PAREN);
            }
            else {
                if ((c == std::string("["))) {
                    (*this).add_simple_token(TK_LEFT_BRACKET);
                }
                else {
                    if ((c == std::string("]"))) {
                        (*this).add_simple_token(TK_RIGHT_BRACKET);
                    }
                    else {
                        if ((c == std::string("{"))) {
                            if (this->in_string_interp) {
                                this->interp_brace_depth = (this->interp_brace_depth + 1LL);
                            }
                            (*this).add_simple_token(TK_LEFT_BRACE);
                        }
                        else {
                            if ((c == std::string("}"))) {
                                if (this->in_string_interp) {
                                    this->interp_brace_depth = (this->interp_brace_depth - 1LL);
                                    if ((this->interp_brace_depth == 0LL)) {
                                        this->in_string_interp = false;
                                        (*this).add_token(TK_RIGHT_PAREN, std::string(")"));
                                        (*this).add_token(TK_PLUS, std::string("+"));
                                        (*this).scan_string();
                                        return;
                                    }
                                }
                                (*this).add_simple_token(TK_RIGHT_BRACE);
                            }
                            else {
                                if ((c == std::string(","))) {
                                    (*this).add_simple_token(TK_COMMA);
                                }
                                else {
                                    if ((c == std::string("."))) {
                                        if ((*this).match_char(std::string("."))) {
                                            (*this).add_simple_token(TK_DOT_DOT);
                                        }
                                        else {
                                            (*this).add_simple_token(TK_DOT);
                                        }
                                    }
                                    else {
                                        if ((c == std::string("?"))) {
                                            (*this).add_simple_token(TK_QUESTION);
                                        }
                                        else {
                                            if ((c == std::string("+"))) {
                                                if ((*this).match_char(std::string("="))) {
                                                    (*this).add_simple_token(TK_PLUS_EQUAL);
                                                }
                                                else {
                                                    (*this).add_simple_token(TK_PLUS);
                                                }
                                            }
                                            else {
                                                if ((c == std::string(";"))) {
                                                    (*this).add_simple_token(TK_SEMICOLON);
                                                }
                                                else {
                                                    if ((c == std::string("*"))) {
                                                        if ((*this).match_char(std::string("="))) {
                                                            (*this).add_simple_token(TK_STAR_EQUAL);
                                                        }
                                                        else {
                                                            (*this).add_simple_token(TK_STAR);
                                                        }
                                                    }
                                                    else {
                                                        if ((c == std::string("%"))) {
                                                            (*this).add_simple_token(TK_PERCENT);
                                                        }
                                                        else {
                                                            if ((c == std::string("&"))) {
                                                                if ((*this).match_char(std::string("&"))) {
                                                                    (*this).add_simple_token(TK_AND);
                                                                }
                                                                else {
                                                                    (*this).add_simple_token(TK_AMPERSAND);
                                                                }
                                                            }
                                                            else {
                                                                if ((c == std::string("|"))) {
                                                                    if ((*this).match_char(std::string("|"))) {
                                                                        (*this).add_simple_token(TK_OR);
                                                                    }
                                                                    else {
                                                                        (*this).add_error(std::string("Unexpected character: |"));
                                                                    }
                                                                }
                                                                else {
                                                                    if ((c == std::string("-"))) {
                                                                        if ((*this).match_char(std::string(">"))) {
                                                                            (*this).add_simple_token(TK_ARROW);
                                                                        }
                                                                        else {
                                                                            if ((*this).match_char(std::string("="))) {
                                                                                (*this).add_simple_token(TK_MINUS_EQUAL);
                                                                            }
                                                                            else {
                                                                                (*this).add_simple_token(TK_MINUS);
                                                                            }
                                                                        }
                                                                    }
                                                                    else {
                                                                        if ((c == std::string(":"))) {
                                                                            if ((*this).match_char(std::string(":"))) {
                                                                                (*this).add_simple_token(TK_DOUBLE_COLON);
                                                                            }
                                                                            else {
                                                                                (*this).add_simple_token(TK_COLON);
                                                                            }
                                                                        }
                                                                        else {
                                                                            if ((c == std::string("#"))) {
                                                                                (*this).add_simple_token(TK_HASH);
                                                                            }
                                                                            else {
                                                                                if ((c == std::string("!"))) {
                                                                                    if ((*this).match_char(std::string("="))) {
                                                                                        (*this).add_simple_token(TK_BANG_EQUAL);
                                                                                    }
                                                                                    else {
                                                                                        (*this).add_simple_token(TK_BANG);
                                                                                    }
                                                                                }
                                                                                else {
                                                                                    if ((c == std::string("="))) {
                                                                                        if ((*this).match_char(std::string("="))) {
                                                                                            (*this).add_simple_token(TK_EQUAL_EQUAL);
                                                                                        }
                                                                                        else {
                                                                                            if ((*this).match_char(std::string(">"))) {
                                                                                                (*this).add_simple_token(TK_FAT_ARROW);
                                                                                            }
                                                                                            else {
                                                                                                (*this).add_simple_token(TK_EQUAL);
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                    else {
                                                                                        if ((c == std::string("<"))) {
                                                                                            if ((*this).match_char(std::string("="))) {
                                                                                                (*this).add_simple_token(TK_LESS_EQUAL);
                                                                                            }
                                                                                            else {
                                                                                                (*this).add_simple_token(TK_LESS);
                                                                                            }
                                                                                        }
                                                                                        else {
                                                                                            if ((c == std::string(">"))) {
                                                                                                if ((*this).match_char(std::string("="))) {
                                                                                                    (*this).add_simple_token(TK_GREATER_EQUAL);
                                                                                                }
                                                                                                else {
                                                                                                    (*this).add_simple_token(TK_GREATER);
                                                                                                }
                                                                                            }
                                                                                            else {
                                                                                                if ((c == std::string("/"))) {
                                                                                                    if ((*this).match_char(std::string("/"))) {
                                                                                                        while (((*this).peek() != std::string("\n")) && (!(*this).is_at_end())) {
                                                                                                            (*this).advance();
                                                                                                        }
                                                                                                    }
                                                                                                    else {
                                                                                                        (*this).add_simple_token(TK_SLASH);
                                                                                                    }
                                                                                                }
                                                                                                else {
                                                                                                    if ((c == std::string(" ")) || (c == std::string("\r")) || (c == std::string("\t"))) {
                                                                                                        /* pass */
                                                                                                    }
                                                                                                    else {
                                                                                                        if ((c == std::string("\n"))) {
                                                                                                            (*this).add_token(TK_NEWLINE, std::string("\n"));
                                                                                                            this->line = (this->line + 1LL);
                                                                                                            this->column = 1LL;
                                                                                                            this->at_line_start = true;
                                                                                                        }
                                                                                                        else {
                                                                                                            if ((c == std::string("\""))) {
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
                                                                                                                        (*this).add_error(((std::string("Unexpected character: ") + (c)) + std::string("")));
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

    void scan_tokens() {
        while ((!(*this).is_at_end())) {
            this->start = this->current;
            this->start_line = this->line;
            this->start_column = this->column;
            (*this).scan_token();
        }
        while ((static_cast<int64_t>(this->indent_stack.size()) > 1LL)) {
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
    print((std::string("Tokens: ") + static_cast<int64_t>(scanner.tokens.size())));
    if ((static_cast<int64_t>(scanner.errors.size()) > 0LL)) {
        print(std::string("ERRORS:"));
        for (const auto& err : scanner.errors) {
            print(err);
        }
    }
    else {
        for (auto tok : scanner.tokens) {
            print(tok.to_string());
        }
    }
}

struct TypeNode;
struct TypeNode {
    struct None {};
    struct Int {};
    struct Float {};
    struct Str {};
    struct Bool {};
    struct Void {};
    struct Auto {};
    struct Dynamic {};
    struct NullType {};
    struct Custom { std::string name; std::vector<TypeNode> type_args; };
    struct Array { std::shared_ptr<TypeNode> inner; };
    struct HashSet { std::shared_ptr<TypeNode> inner; };
    struct HashMap { std::shared_ptr<TypeNode> key_type; std::shared_ptr<TypeNode> value_type; };
    struct Nullable { std::shared_ptr<TypeNode> inner; };
    struct Int8 {};
    struct Int16 {};
    struct Int32 {};
    struct Float32 {};
    struct USize {};
    struct CString {};
    struct Ptr { std::shared_ptr<TypeNode> inner; };

    std::string _tag;
    std::variant<TypeNode::None, TypeNode::Int, TypeNode::Float, TypeNode::Str, TypeNode::Bool, TypeNode::Void, TypeNode::Auto, TypeNode::Dynamic, TypeNode::NullType, TypeNode::Custom, TypeNode::Array, TypeNode::HashSet, TypeNode::HashMap, TypeNode::Nullable, TypeNode::Int8, TypeNode::Int16, TypeNode::Int32, TypeNode::Float32, TypeNode::USize, TypeNode::CString, TypeNode::Ptr> _data;

    static TypeNode make_None() { return {"None", None{}}; }
    static TypeNode make_Int() { return {"Int", Int{}}; }
    static TypeNode make_Float() { return {"Float", Float{}}; }
    static TypeNode make_Str() { return {"Str", Str{}}; }
    static TypeNode make_Bool() { return {"Bool", Bool{}}; }
    static TypeNode make_Void() { return {"Void", Void{}}; }
    static TypeNode make_Auto() { return {"Auto", Auto{}}; }
    static TypeNode make_Dynamic() { return {"Dynamic", Dynamic{}}; }
    static TypeNode make_NullType() { return {"NullType", NullType{}}; }
    static TypeNode make_Custom(std::string name, std::vector<TypeNode> type_args) { return {"Custom", Custom{name, type_args}}; }
    static TypeNode make_Array(TypeNode inner) { return {"Array", Array{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_HashSet(TypeNode inner) { return {"HashSet", HashSet{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_HashMap(TypeNode key_type, TypeNode value_type) { return {"HashMap", HashMap{std::make_shared<TypeNode>(std::move(key_type)), std::make_shared<TypeNode>(std::move(value_type))}}; }
    static TypeNode make_Nullable(TypeNode inner) { return {"Nullable", Nullable{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_Int8() { return {"Int8", Int8{}}; }
    static TypeNode make_Int16() { return {"Int16", Int16{}}; }
    static TypeNode make_Int32() { return {"Int32", Int32{}}; }
    static TypeNode make_Float32() { return {"Float32", Float32{}}; }
    static TypeNode make_USize() { return {"USize", USize{}}; }
    static TypeNode make_CString() { return {"CString", CString{}}; }
    static TypeNode make_Ptr(TypeNode inner) { return {"Ptr", Ptr{std::make_shared<TypeNode>(std::move(inner))}}; }

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
    bool is_ref;
    bool is_mut;

};

struct EnumVariantNode {
    Token name;
    std::vector<TypeNode> types;
    std::vector<std::string> field_names;

};

struct Expr;
struct Expr {
    struct None {};
    struct Literal { std::string kind; std::string value; };
    struct Unary { Token op; std::shared_ptr<Expr> right; };
    struct Binary { std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right; };
    struct Grouping { std::shared_ptr<Expr> inner; };
    struct Variable { Token name; };
    struct Assign { Token name; std::shared_ptr<Expr> value; };
    struct Logical { std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right; };
    struct Call { std::shared_ptr<Expr> callee; Token paren; std::vector<Expr> args; };
    struct Index { std::shared_ptr<Expr> object; Token bracket; std::shared_ptr<Expr> index; };
    struct IndexSet { std::shared_ptr<Expr> object; Token bracket; std::shared_ptr<Expr> index; std::shared_ptr<Expr> value; };
    struct Vector { std::vector<Expr> elements; };
    struct Map { std::vector<Expr> keys; std::vector<Expr> values; };
    struct Get { std::shared_ptr<Expr> object; Token name; };
    struct Set { std::shared_ptr<Expr> object; Token name; std::shared_ptr<Expr> value; };
    struct StaticGet { std::shared_ptr<Expr> object; Token name; };
    struct This { Token keyword; };
    struct Cast { std::shared_ptr<Expr> expr; TypeNode target_type; };
    struct Throw { std::shared_ptr<Expr> expr; };
    struct Range { std::shared_ptr<Expr> start; std::shared_ptr<Expr> end; };
    struct Lambda { std::vector<Param> params; std::shared_ptr<Expr> body; };
    struct Own { std::shared_ptr<Expr> expr; };
    struct AddressOf { std::shared_ptr<Expr> expr; };
    struct BlockLambda { std::vector<Param> params; int64_t body_id; };

    std::string _tag;
    std::variant<Expr::None, Expr::Literal, Expr::Unary, Expr::Binary, Expr::Grouping, Expr::Variable, Expr::Assign, Expr::Logical, Expr::Call, Expr::Index, Expr::IndexSet, Expr::Vector, Expr::Map, Expr::Get, Expr::Set, Expr::StaticGet, Expr::This, Expr::Cast, Expr::Throw, Expr::Range, Expr::Lambda, Expr::Own, Expr::AddressOf, Expr::BlockLambda> _data;

    static Expr make_None() { return {"None", None{}}; }
    static Expr make_Literal(std::string kind, std::string value) { return {"Literal", Literal{kind, value}}; }
    static Expr make_Unary(Token op, Expr right) { return {"Unary", Unary{op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Binary(Expr left, Token op, Expr right) { return {"Binary", Binary{std::make_shared<Expr>(std::move(left)), op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Grouping(Expr inner) { return {"Grouping", Grouping{std::make_shared<Expr>(std::move(inner))}}; }
    static Expr make_Variable(Token name) { return {"Variable", Variable{name}}; }
    static Expr make_Assign(Token name, Expr value) { return {"Assign", Assign{name, std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_Logical(Expr left, Token op, Expr right) { return {"Logical", Logical{std::make_shared<Expr>(std::move(left)), op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Call(Expr callee, Token paren, std::vector<Expr> args) { return {"Call", Call{std::make_shared<Expr>(std::move(callee)), paren, args}}; }
    static Expr make_Index(Expr object, Token bracket, Expr index) { return {"Index", Index{std::make_shared<Expr>(std::move(object)), bracket, std::make_shared<Expr>(std::move(index))}}; }
    static Expr make_IndexSet(Expr object, Token bracket, Expr index, Expr value) { return {"IndexSet", IndexSet{std::make_shared<Expr>(std::move(object)), bracket, std::make_shared<Expr>(std::move(index)), std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_Vector(std::vector<Expr> elements) { return {"Vector", Vector{elements}}; }
    static Expr make_Map(std::vector<Expr> keys, std::vector<Expr> values) { return {"Map", Map{keys, values}}; }
    static Expr make_Get(Expr object, Token name) { return {"Get", Get{std::make_shared<Expr>(std::move(object)), name}}; }
    static Expr make_Set(Expr object, Token name, Expr value) { return {"Set", Set{std::make_shared<Expr>(std::move(object)), name, std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_StaticGet(Expr object, Token name) { return {"StaticGet", StaticGet{std::make_shared<Expr>(std::move(object)), name}}; }
    static Expr make_This(Token keyword) { return {"This", This{keyword}}; }
    static Expr make_Cast(Expr expr, TypeNode target_type) { return {"Cast", Cast{std::make_shared<Expr>(std::move(expr)), target_type}}; }
    static Expr make_Throw(Expr expr) { return {"Throw", Throw{std::make_shared<Expr>(std::move(expr))}}; }
    static Expr make_Range(Expr start, Expr end) { return {"Range", Range{std::make_shared<Expr>(std::move(start)), std::make_shared<Expr>(std::move(end))}}; }
    static Expr make_Lambda(std::vector<Param> params, Expr body) { return {"Lambda", Lambda{params, std::make_shared<Expr>(std::move(body))}}; }
    static Expr make_Own(Expr expr) { return {"Own", Own{std::make_shared<Expr>(std::move(expr))}}; }
    static Expr make_AddressOf(Expr expr) { return {"AddressOf", AddressOf{std::make_shared<Expr>(std::move(expr))}}; }
    static Expr make_BlockLambda(std::vector<Param> params, int64_t body_id) { return {"BlockLambda", BlockLambda{params, body_id}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const Expr& e) { std::cout << "Expr(" << e._tag << ")" << std::endl; }
std::string operator+(const std::string& s, const Expr& e) { return s + e._tag; }
std::string operator+(const Expr& e, const std::string& s) { return e._tag + s; }

struct ExternType {
    std::string lavina_name;
    std::string cpp_name;

};

struct ExternFn {
    std::string name;
    std::string cpp_name;
    TypeNode return_type;
    std::vector<Param> params;

};

struct MatchArm {
    std::string pattern_name;
    std::vector<std::string> bindings;

};

struct Stmt;
struct Stmt {
    struct None {};
    struct ExprStmt { Expr expr; };
    struct Let { Token name; TypeNode var_type; Expr initializer; std::string visibility; bool is_ref; bool is_mut; };
    struct Const { Token name; TypeNode const_type; Expr value; std::string visibility; };
    struct Return { Token keyword; Expr value; };
    struct If { Expr condition; std::shared_ptr<Stmt> then_branch; std::shared_ptr<Stmt> else_branch; };
    struct While { Expr condition; std::shared_ptr<Stmt> body; };
    struct For { Token item_name; Expr collection; std::shared_ptr<Stmt> body; bool is_ref; bool is_mut; };
    struct Block { std::vector<Stmt> statements; };
    struct Try { std::shared_ptr<Stmt> try_body; std::shared_ptr<Stmt> catch_body; std::string exception_name; };
    struct Function { Token name; std::vector<Param> params; TypeNode return_type; std::vector<Stmt> body; bool is_inline; int64_t comptime_mode; bool is_static; std::string visibility; std::vector<std::string> type_params; };
    struct Class { Token name; std::vector<Stmt> body; std::string visibility; };
    struct Struct { Token name; std::vector<Stmt> body; std::string visibility; std::vector<std::string> type_params; };
    struct Enum { Token name; std::vector<EnumVariantNode> variants; std::vector<Stmt> methods; std::string visibility; std::vector<std::string> type_params; };
    struct Match { Expr expr; std::vector<MatchArm> arm_patterns; std::vector<Stmt> arm_bodies; };
    struct Namespace { Token name; std::vector<Stmt> body; std::string visibility; };
    struct Import { std::vector<Token> path; std::string alias; };
    struct Break { Token keyword; };
    struct Continue { Token keyword; };
    struct Pass { Token keyword; };
    struct CppBlock { std::string code; };
    struct Extern { std::string header; std::string link_lib; std::vector<ExternType> types; std::vector<ExternFn> functions; };

    std::string _tag;
    std::variant<Stmt::None, Stmt::ExprStmt, Stmt::Let, Stmt::Const, Stmt::Return, Stmt::If, Stmt::While, Stmt::For, Stmt::Block, Stmt::Try, Stmt::Function, Stmt::Class, Stmt::Struct, Stmt::Enum, Stmt::Match, Stmt::Namespace, Stmt::Import, Stmt::Break, Stmt::Continue, Stmt::Pass, Stmt::CppBlock, Stmt::Extern> _data;

    static Stmt make_None() { return {"None", None{}}; }
    static Stmt make_ExprStmt(Expr expr) { return {"ExprStmt", ExprStmt{expr}}; }
    static Stmt make_Let(Token name, TypeNode var_type, Expr initializer, std::string visibility, bool is_ref, bool is_mut) { return {"Let", Let{name, var_type, initializer, visibility, is_ref, is_mut}}; }
    static Stmt make_Const(Token name, TypeNode const_type, Expr value, std::string visibility) { return {"Const", Const{name, const_type, value, visibility}}; }
    static Stmt make_Return(Token keyword, Expr value) { return {"Return", Return{keyword, value}}; }
    static Stmt make_If(Expr condition, Stmt then_branch, Stmt else_branch) { return {"If", If{condition, std::make_shared<Stmt>(std::move(then_branch)), std::make_shared<Stmt>(std::move(else_branch))}}; }
    static Stmt make_While(Expr condition, Stmt body) { return {"While", While{condition, std::make_shared<Stmt>(std::move(body))}}; }
    static Stmt make_For(Token item_name, Expr collection, Stmt body, bool is_ref, bool is_mut) { return {"For", For{item_name, collection, std::make_shared<Stmt>(std::move(body)), is_ref, is_mut}}; }
    static Stmt make_Block(std::vector<Stmt> statements) { return {"Block", Block{statements}}; }
    static Stmt make_Try(Stmt try_body, Stmt catch_body, std::string exception_name) { return {"Try", Try{std::make_shared<Stmt>(std::move(try_body)), std::make_shared<Stmt>(std::move(catch_body)), exception_name}}; }
    static Stmt make_Function(Token name, std::vector<Param> params, TypeNode return_type, std::vector<Stmt> body, bool is_inline, int64_t comptime_mode, bool is_static, std::string visibility, std::vector<std::string> type_params) { return {"Function", Function{name, params, return_type, body, is_inline, comptime_mode, is_static, visibility, type_params}}; }
    static Stmt make_Class(Token name, std::vector<Stmt> body, std::string visibility) { return {"Class", Class{name, body, visibility}}; }
    static Stmt make_Struct(Token name, std::vector<Stmt> body, std::string visibility, std::vector<std::string> type_params) { return {"Struct", Struct{name, body, visibility, type_params}}; }
    static Stmt make_Enum(Token name, std::vector<EnumVariantNode> variants, std::vector<Stmt> methods, std::string visibility, std::vector<std::string> type_params) { return {"Enum", Enum{name, variants, methods, visibility, type_params}}; }
    static Stmt make_Match(Expr expr, std::vector<MatchArm> arm_patterns, std::vector<Stmt> arm_bodies) { return {"Match", Match{expr, arm_patterns, arm_bodies}}; }
    static Stmt make_Namespace(Token name, std::vector<Stmt> body, std::string visibility) { return {"Namespace", Namespace{name, body, visibility}}; }
    static Stmt make_Import(std::vector<Token> path, std::string alias) { return {"Import", Import{path, alias}}; }
    static Stmt make_Break(Token keyword) { return {"Break", Break{keyword}}; }
    static Stmt make_Continue(Token keyword) { return {"Continue", Continue{keyword}}; }
    static Stmt make_Pass(Token keyword) { return {"Pass", Pass{keyword}}; }
    static Stmt make_CppBlock(std::string code) { return {"CppBlock", CppBlock{code}}; }
    static Stmt make_Extern(std::string header, std::string link_lib, std::vector<ExternType> types, std::vector<ExternFn> functions) { return {"Extern", Extern{header, link_lib, types, functions}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const Stmt& e) { std::cout << "Stmt(" << e._tag << ")" << std::endl; }
std::string operator+(const std::string& s, const Stmt& e) { return s + e._tag; }
std::string operator+(const Stmt& e, const std::string& s) { return e._tag + s; }

struct CppCodegen {
    std::string output;
    std::string declarations;
    int64_t indent_level;
    int64_t temp_counter;
    bool has_main;
    std::unordered_map<std::string, std::vector<EnumVariantNode>> known_enums;
    std::vector<std::string> dynamic_vars;
    std::vector<std::string> extern_includes;
    std::vector<std::string> extern_link_libs;
    std::unordered_map<std::string, std::string> extern_fn_names;
    std::unordered_map<std::string, std::string> extern_type_names;
    std::unordered_map<std::string, std::vector<Param>> extern_fn_params;
    std::unordered_map<std::string, TypeNode> var_types;
    std::vector<std::string> module_short_names;
    std::vector<std::string> module_full_names;
    std::vector<std::string> module_aliases;
    std::vector<std::vector<Stmt>> module_stmts;
    std::vector<std::vector<Stmt>> lambda_blocks;

    CppCodegen() {
        this->output = std::string("");
        this->declarations = std::string("");
        this->indent_level = 0LL;
        this->temp_counter = 0LL;
        this->has_main = false;
        this->known_enums = {{}};
        this->dynamic_vars = {};
        this->extern_includes = {};
        this->extern_link_libs = {};
        this->extern_fn_names = {{}};
        this->extern_type_names = {{}};
        this->var_types = {{}};
        this->extern_fn_params = {{}};
        this->module_short_names = {};
        this->module_full_names = {};
        this->module_aliases = {};
        this->module_stmts = {};
        this->lambda_blocks = {};
    }

    void set_modules(const std::vector<std::string>& short_names, const std::vector<std::string>& full_names, const std::vector<std::string>& aliases, const std::vector<std::vector<Stmt>>& stmts) {
        this->module_short_names = short_names;
        this->module_full_names = full_names;
        this->module_aliases = aliases;
        this->module_stmts = stmts;
    }

    std::string indent() {
        std::string result = std::string("");
        for (int64_t i = 0LL; i < this->indent_level; i++) {
            result = (result + std::string("    "));
        }
        return result;
    }

    bool is_dynamic_var(const std::string& name) {
        for (const auto& v : this->dynamic_vars) {
            if ((v == name)) {
                return true;
            }
        }
        return false;
    }

    void add_dynamic_var(std::string name) {
        if ((!(*this).is_dynamic_var(name))) {
            this->dynamic_vars.push_back(name);
        }
    }

    std::string find_enum_for_variant(const std::string& variant_name) {
        std::string found = std::string("");
        int64_t count = 0LL;
        std::vector<std::string> keys = lv_keys(this->known_enums);
        for (const auto& key : keys) {
            std::vector<EnumVariantNode> variants = this->known_enums[key];
            for (const auto& v : variants) {
                if ((v.name.lexeme == variant_name)) {
                    if ((count == 0LL)) {
                        found = key;
                    }
                    count = (count + 1LL);
                }
            }
        }
        if ((count == 1LL)) {
            return found;
        }
        return std::string("");
    }

    EnumVariantNode get_variant_info(const std::string& enum_name, const std::string& variant_name) {
        if ((this->known_enums.count(enum_name) > 0)) {
            std::vector<EnumVariantNode> variants = this->known_enums[enum_name];
            for (const auto& v : variants) {
                if ((v.name.lexeme == variant_name)) {
                    return v;
                }
            }
        }
        std::vector<TypeNode> empty_types = {};
        std::vector<std::string> empty_names = {};
        return EnumVariantNode(Token(std::string(""), std::string(""), 0LL, 0LL), empty_types, empty_names);
    }

    bool is_known_enum(const std::string& name) {
        if ((this->known_enums.count(name) > 0)) {
            return true;
        }
        int64_t angle = lv_index_of(name, std::string("<"));
        if ((angle >= 0LL)) {
            std::string base = name.substr(0LL, (angle) - (0LL));
            return (this->known_enums.count(base) > 0);
        }
        return false;
    }

    bool type_contains_dynamic(const TypeNode& t) {
        {
            const auto& _match_0 = t;
            if (std::holds_alternative<std::decay_t<decltype(_match_0)>::Dynamic>(_match_0._data)) {
                return true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_0)>::Array>(_match_0._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_0)>::Array>(_match_0._data);
                auto& inner = *_v.inner;
                return (*this).type_contains_dynamic(inner);
            }
            else {
                return false;
            }
        }
    }

    std::string template_prefix(const std::vector<std::string>& type_params) {
        if ((static_cast<int64_t>(type_params.size()) == 0LL)) {
            return std::string("");
        }
        std::vector<std::string> tps = {};
        for (const auto& tp : type_params) {
            tps.push_back(((std::string("typename ") + (tp)) + std::string("")));
        }
        return ((((std::string("") + ((*this).indent())) + std::string("template<")) + (lv_join(tps, std::string(", ")))) + std::string(">\n"));
    }

    std::string wrap_convert(std::string expr, const TypeNode& from, const TypeNode& expected) {
        {
            const auto& _match_1 = from;
            if (std::holds_alternative<std::decay_t<decltype(_match_1)>::Str>(_match_1._data)) {
                {
                    const auto& _match_2 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_2)>::CString>(_match_2._data)) {
                        return ((std::string("(") + (expr)) + std::string(").c_str()"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_1)>::CString>(_match_1._data)) {
                {
                    const auto& _match_3 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_3)>::Str>(_match_3._data)) {
                        return ((std::string("std::string(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_1)>::Int>(_match_1._data)) {
                {
                    const auto& _match_4 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Int32>(_match_4._data)) {
                        return ((std::string("static_cast<int32_t>(") + (expr)) + std::string(")"));
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::USize>(_match_4._data)) {
                        return ((std::string("static_cast<size_t>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_1)>::Int32>(_match_1._data)) {
                {
                    const auto& _match_5 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Int>(_match_5._data)) {
                        return ((std::string("static_cast<int64_t>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_1)>::USize>(_match_1._data)) {
                {
                    const auto& _match_6 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_6)>::Int>(_match_6._data)) {
                        return ((std::string("static_cast<int64_t>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_1)>::Float>(_match_1._data)) {
                {
                    const auto& _match_7 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_7)>::Float32>(_match_7._data)) {
                        return ((std::string("static_cast<float>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_1)>::Float32>(_match_1._data)) {
                {
                    const auto& _match_8 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_8)>::Float>(_match_8._data)) {
                        return ((std::string("static_cast<double>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        return expr;
    }

    std::string wrap_extern_arg(std::string expr, const Expr& original, const TypeNode& expected) {
        {
            const auto& _match_9 = original;
            if (std::holds_alternative<std::decay_t<decltype(_match_9)>::Literal>(_match_9._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_9)>::Literal>(_match_9._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("null"))) {
                    {
                        const auto& _match_10 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_10)>::Ptr>(_match_10._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_10)>::Ptr>(_match_10._data);
                            auto& inner = *_v.inner;
                            return std::string("nullptr");
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                if ((kind == std::string("string"))) {
                    {
                        const auto& _match_11 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_11)>::CString>(_match_11._data)) {
                            return ((std::string("(") + (expr)) + std::string(").c_str()"));
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                if ((kind == std::string("int"))) {
                    {
                        const auto& _match_12 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_12)>::Int32>(_match_12._data)) {
                            return ((std::string("static_cast<int32_t>(") + (expr)) + std::string(")"));
                        }
                        else if (std::holds_alternative<std::decay_t<decltype(_match_12)>::USize>(_match_12._data)) {
                            return ((std::string("static_cast<size_t>(") + (expr)) + std::string(")"));
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                if ((kind == std::string("float"))) {
                    {
                        const auto& _match_13 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_13)>::Float32>(_match_13._data)) {
                            return ((std::string("static_cast<float>(") + (expr)) + std::string(")"));
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                return expr;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_9)>::Variable>(_match_9._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_9)>::Variable>(_match_9._data);
                auto& tok = _v.name;
                if ((this->var_types.count(tok.lexeme) > 0)) {
                    TypeNode vt = this->var_types[tok.lexeme];
                    return (*this).wrap_convert(expr, vt, expected);
                }
            }
            else {
                /* pass */
            }
        }
        return expr;
    }

    std::string emit_type(const TypeNode& t) {
        {
            const auto& _match_14 = t;
            if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Int>(_match_14._data)) {
                return std::string("int64_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Float>(_match_14._data)) {
                return std::string("double");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Str>(_match_14._data)) {
                return std::string("std::string");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Bool>(_match_14._data)) {
                return std::string("bool");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Void>(_match_14._data)) {
                return std::string("void");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Auto>(_match_14._data)) {
                return std::string("auto");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Dynamic>(_match_14._data)) {
                return std::string("std::any");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::NullType>(_match_14._data)) {
                return std::string("std::nullptr_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Custom>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::Custom>(_match_14._data);
                auto& name = _v.name;
                auto& type_args = _v.type_args;
                if ((static_cast<int64_t>(type_args.size()) > 0LL)) {
                    std::vector<std::string> ta = {};
                    for (const auto& a : type_args) {
                        ta.push_back((*this).emit_type(a));
                    }
                    return ((((std::string("") + (name)) + std::string("<")) + (lv_join(ta, std::string(", ")))) + std::string(">"));
                }
                if ((this->extern_type_names.count(name) > 0)) {
                    return this->extern_type_names[name];
                }
                return name;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Array>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::Array>(_match_14._data);
                auto& inner = *_v.inner;
                return ((std::string("std::vector<") + ((*this).emit_type(inner))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::HashSet>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::HashSet>(_match_14._data);
                auto& inner = *_v.inner;
                return ((std::string("std::unordered_set<") + ((*this).emit_type(inner))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::HashMap>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::HashMap>(_match_14._data);
                auto& key_type = *_v.key_type;
                auto& value_type = *_v.value_type;
                return ((((std::string("std::unordered_map<") + ((*this).emit_type(key_type))) + std::string(", ")) + ((*this).emit_type(value_type))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Nullable>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::Nullable>(_match_14._data);
                auto& inner = *_v.inner;
                return ((std::string("std::optional<") + ((*this).emit_type(inner))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Int8>(_match_14._data)) {
                return std::string("int8_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Int16>(_match_14._data)) {
                return std::string("int16_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Int32>(_match_14._data)) {
                return std::string("int32_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Float32>(_match_14._data)) {
                return std::string("float");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::USize>(_match_14._data)) {
                return std::string("size_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::CString>(_match_14._data)) {
                return std::string("const char*");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Ptr>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::Ptr>(_match_14._data);
                auto& inner = *_v.inner;
                return ((std::string("") + ((*this).emit_type(inner))) + std::string("*"));
            }
            else {
                return std::string("auto");
            }
        }
    }

    std::string token_to_cpp_op(Token t) {
        if ((t.token_type == TK_PLUS)) {
            return std::string("+");
        }
        else {
            if ((t.token_type == TK_MINUS)) {
                return std::string("-");
            }
            else {
                if ((t.token_type == TK_STAR)) {
                    return std::string("*");
                }
                else {
                    if ((t.token_type == TK_SLASH)) {
                        return std::string("/");
                    }
                    else {
                        if ((t.token_type == TK_PERCENT)) {
                            return std::string("%");
                        }
                        else {
                            if ((t.token_type == TK_EQUAL_EQUAL)) {
                                return std::string("==");
                            }
                            else {
                                if ((t.token_type == TK_BANG_EQUAL)) {
                                    return std::string("!=");
                                }
                                else {
                                    if ((t.token_type == TK_LESS)) {
                                        return std::string("<");
                                    }
                                    else {
                                        if ((t.token_type == TK_LESS_EQUAL)) {
                                            return std::string("<=");
                                        }
                                        else {
                                            if ((t.token_type == TK_GREATER)) {
                                                return std::string(">");
                                            }
                                            else {
                                                if ((t.token_type == TK_GREATER_EQUAL)) {
                                                    return std::string(">=");
                                                }
                                                else {
                                                    if ((t.token_type == TK_BANG) || (t.token_type == TK_NOT)) {
                                                        return std::string("!");
                                                    }
                                                    else {
                                                        if ((t.token_type == TK_AND)) {
                                                            return std::string("&&");
                                                        }
                                                        else {
                                                            if ((t.token_type == TK_OR)) {
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
        if ((cpp_type == std::string("int64_t"))) {
            return std::string(" = 0");
        }
        else {
            if ((cpp_type == std::string("double"))) {
                return std::string(" = 0.0");
            }
            else {
                if ((cpp_type == std::string("bool"))) {
                    return std::string(" = false");
                }
                else {
                    if ((cpp_type == std::string("std::string"))) {
                        return std::string(" = \"\"");
                    }
                    else {
                        if ((cpp_type == std::string("int8_t"))) {
                            return std::string(" = 0");
                        }
                        else {
                            if ((cpp_type == std::string("int16_t"))) {
                                return std::string(" = 0");
                            }
                            else {
                                if ((cpp_type == std::string("int32_t"))) {
                                    return std::string(" = 0");
                                }
                                else {
                                    if ((cpp_type == std::string("size_t"))) {
                                        return std::string(" = 0");
                                    }
                                    else {
                                        if ((cpp_type == std::string("float"))) {
                                            return std::string(" = 0.0f");
                                        }
                                        else {
                                            if ((cpp_type == std::string("const char*"))) {
                                                return std::string(" = nullptr");
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

    std::string emit_expr(const Expr& e, bool m) {
        {
            const auto& _match_15 = e;
            if (_match_15._tag == "None") {
                return std::string("");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Literal>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Literal>(_match_15._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                return (*this).emit_literal(kind, value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Unary>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Unary>(_match_15._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string r = (*this).emit_expr(right, m);
                std::string op_str = (*this).token_to_cpp_op(op);
                return ((((std::string("(") + (op_str)) + std::string("")) + (r)) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Binary>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Binary>(_match_15._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_expr(left, m);
                std::string r = (*this).emit_expr(right, m);
                std::string op_str = (*this).token_to_cpp_op(op);
                return ((((((std::string("(") + (l)) + std::string(" ")) + (op_str)) + std::string(" ")) + (r)) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Grouping>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Grouping>(_match_15._data);
                auto& inner = *_v.inner;
                return ((std::string("(") + ((*this).emit_expr(inner, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Variable>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Variable>(_match_15._data);
                auto& name = _v.name;
                return name.lexeme;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Assign>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Assign>(_match_15._data);
                auto& name = _v.name;
                auto& value = *_v.value;
                return ((((std::string("") + (name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(value, m))) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Logical>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Logical>(_match_15._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_expr(left, m);
                std::string r = (*this).emit_expr(right, m);
                std::string op_str = std::string("&&");
                if ((op.token_type == TK_OR)) {
                    op_str = std::string("||");
                }
                return ((((((std::string("") + (l)) + std::string(" ")) + (op_str)) + std::string(" ")) + (r)) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Call>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Call>(_match_15._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                return (*this).emit_call_expr(callee, args, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Index>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Index>(_match_15._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string("[")) + ((*this).emit_expr(index, m))) + std::string("]"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::IndexSet>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::IndexSet>(_match_15._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                auto& value = *_v.value;
                return ((((((std::string("") + ((*this).emit_expr(object, m))) + std::string("[")) + ((*this).emit_expr(index, m))) + std::string("] = ")) + ((*this).emit_expr(value, m))) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Vector>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Vector>(_match_15._data);
                auto& elements = _v.elements;
                if ((static_cast<int64_t>(elements.size()) == 0LL)) {
                    return std::string("{}");
                }
                std::vector<std::string> elems = {};
                for (const auto& el : elements) {
                    elems.push_back((*this).emit_expr(el, m));
                }
                return ((std::string("std::vector{") + (lv_join(elems, std::string(", ")))) + std::string("}"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Map>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Map>(_match_15._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                std::vector<std::string> entries = {};
                for (int64_t i = 0LL; i < static_cast<int64_t>(keys.size()); i++) {
                    entries.push_back(((((std::string("{") + ((*this).emit_expr(keys[i], m))) + std::string(", ")) + ((*this).emit_expr(values[i], m))) + std::string("}")));
                }
                return ((std::string("{{") + (lv_join(entries, std::string(", ")))) + std::string("}}"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Get>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Get>(_match_15._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                if (m) {
                    {
                        const auto& _match_16 = object;
                        if (std::holds_alternative<std::decay_t<decltype(_match_16)>::This>(_match_16._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_16)>::This>(_match_16._data);
                            auto& kw = _v.keyword;
                            return ((std::string("this->") + (name.lexeme)) + std::string(""));
                        }
                        else {
                            return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string(".")) + (name.lexeme)) + std::string(""));
                        }
                    }
                }
                return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string(".")) + (name.lexeme)) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Set>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Set>(_match_15._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                auto& value = *_v.value;
                if (m) {
                    {
                        const auto& _match_17 = object;
                        if (std::holds_alternative<std::decay_t<decltype(_match_17)>::This>(_match_17._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_17)>::This>(_match_17._data);
                            auto& kw = _v.keyword;
                            return ((((std::string("this->") + (name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(value, m))) + std::string(""));
                        }
                        else {
                            return ((((((std::string("") + ((*this).emit_expr(object, m))) + std::string(".")) + (name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(value, m))) + std::string(""));
                        }
                    }
                }
                return ((((((std::string("") + ((*this).emit_expr(object, m))) + std::string(".")) + (name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(value, m))) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::StaticGet>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::StaticGet>(_match_15._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                if (m) {
                    {
                        const auto& _match_18 = object;
                        if (std::holds_alternative<std::decay_t<decltype(_match_18)>::This>(_match_18._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_18)>::This>(_match_18._data);
                            auto& kw = _v.keyword;
                            return ((std::string("(*this)::") + (name.lexeme)) + std::string(""));
                        }
                        else {
                            return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string("::")) + (name.lexeme)) + std::string(""));
                        }
                    }
                }
                return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string("::")) + (name.lexeme)) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::This>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::This>(_match_15._data);
                auto& keyword = _v.keyword;
                return std::string("(*this)");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Cast>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Cast>(_match_15._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                std::string ex = (*this).emit_expr(expr, m);
                std::string t = (*this).emit_type(target_type);
                if ((*this).is_dynamic_expression(expr)) {
                    return ((((std::string("std::any_cast<") + (t)) + std::string(">(")) + (ex)) + std::string(")"));
                }
                return ((((std::string("static_cast<") + (t)) + std::string(">(")) + (ex)) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Throw>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Throw>(_match_15._data);
                auto& expr = *_v.expr;
                return ((std::string("throw std::runtime_error(") + ((*this).emit_expr(expr, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Lambda>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Lambda>(_match_15._data);
                auto& params = _v.params;
                auto& body = *_v.body;
                return (*this).emit_lambda(params, body, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Own>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::Own>(_match_15._data);
                auto& expr = *_v.expr;
                return ((std::string("std::move(") + ((*this).emit_expr(expr, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::AddressOf>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::AddressOf>(_match_15._data);
                auto& expr = *_v.expr;
                return ((std::string("&(") + ((*this).emit_expr(expr, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_15)>::BlockLambda>(_match_15._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_15)>::BlockLambda>(_match_15._data);
                auto& params = _v.params;
                auto& body_id = _v.body_id;
                return (*this).emit_block_lambda(params, body_id, m);
            }
            else {
                return std::string("");
            }
        }
    }

    std::string emit_literal(std::string kind, std::string value) {
        if ((kind == std::string("int"))) {
            return ((std::string("") + (value)) + std::string("LL"));
        }
        else {
            if ((kind == std::string("float"))) {
                return value;
            }
            else {
                if ((kind == std::string("string"))) {
                    return ((std::string("std::string(\"") + (value)) + std::string("\")"));
                }
                else {
                    if ((kind == std::string("bool"))) {
                        return value;
                    }
                    else {
                        if ((kind == std::string("null"))) {
                            return std::string("std::nullopt");
                        }
                    }
                }
            }
        }
        return value;
    }

    bool is_dynamic_expression(const Expr& e) {
        {
            const auto& _match_19 = e;
            if (std::holds_alternative<std::decay_t<decltype(_match_19)>::Variable>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::Variable>(_match_19._data);
                auto& name = _v.name;
                return (*this).is_dynamic_var(name.lexeme);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_19)>::Index>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::Index>(_match_19._data);
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

    std::string emit_call_expr(const Expr& callee, const std::vector<Expr>& args, bool in_method) {
        {
            const auto& _match_20 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_20)>::Get>(_match_20._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_20)>::Get>(_match_20._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = (*this).emit_expr(object, in_method);
                std::vector<std::string> arg_strs = {};
                for (const auto& a : args) {
                    arg_strs.push_back((*this).emit_expr(a, in_method));
                }
                std::string remapped = (*this).try_remap_method(obj, name.lexeme, arg_strs);
                if ((remapped != std::string(""))) {
                    return remapped;
                }
                return ((((((std::string("") + (obj)) + std::string(".")) + (name.lexeme)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_20)>::StaticGet>(_match_20._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_20)>::StaticGet>(_match_20._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = (*this).emit_expr(object, in_method);
                std::vector<std::string> arg_strs = {};
                for (const auto& a : args) {
                    arg_strs.push_back((*this).emit_expr(a, in_method));
                }
                {
                    const auto& _match_21 = object;
                    if (std::holds_alternative<std::decay_t<decltype(_match_21)>::Variable>(_match_21._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_21)>::Variable>(_match_21._data);
                        auto& tok = _v.name;
                        if ((*this).is_known_enum(tok.lexeme)) {
                            return ((((((std::string("") + (obj)) + std::string("::make_")) + (name.lexeme)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
                        }
                    }
                    else {
                        /* pass */
                    }
                }
                return ((((((std::string("") + (obj)) + std::string("::")) + (name.lexeme)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_20)>::Variable>(_match_20._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_20)>::Variable>(_match_20._data);
                auto& tok = _v.name;
                std::vector<std::string> arg_strs = {};
                for (const auto& a : args) {
                    arg_strs.push_back((*this).emit_expr(a, in_method));
                }
                if ((tok.lexeme == std::string("exit"))) {
                    return ((std::string("lv_exit(") + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
                }
                if ((this->extern_fn_params.count(tok.lexeme) > 0)) {
                    std::vector<Param> eparams = this->extern_fn_params[tok.lexeme];
                    for (int64_t i = 0LL; i < static_cast<int64_t>(arg_strs.size()); i++) {
                        if ((i < static_cast<int64_t>(eparams.size()))) {
                            arg_strs[i] = (*this).wrap_extern_arg(arg_strs[i], args[i], eparams[i].param_type);
                        }
                    }
                    std::string fn_name = tok.lexeme;
                    if ((this->extern_fn_names.count(tok.lexeme) > 0)) {
                        fn_name = this->extern_fn_names[tok.lexeme];
                    }
                    return ((((std::string("") + (fn_name)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
                }
                if ((this->extern_fn_names.count(tok.lexeme) > 0)) {
                    return ((((std::string("") + (this->extern_fn_names[tok.lexeme])) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
                }
                return ((((std::string("") + (tok.lexeme)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
            else {
                std::string func = (*this).emit_expr(callee, in_method);
                std::vector<std::string> arg_strs = {};
                for (const auto& a : args) {
                    arg_strs.push_back((*this).emit_expr(a, in_method));
                }
                return ((((std::string("") + (func)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
        }
    }

    std::string try_remap_method(const std::string& obj, const std::string& method, const std::vector<std::string>& args) {
        if ((method == std::string("len"))) {
            return ((std::string("static_cast<int64_t>(") + (obj)) + std::string(".size())"));
        }
        else {
            if ((method == std::string("is_empty"))) {
                return ((std::string("") + (obj)) + std::string(".empty()"));
            }
            else {
                if ((method == std::string("contains"))) {
                    return ((((std::string("lv_contains(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                }
                else {
                    if ((method == std::string("add"))) {
                        return ((((std::string("") + (obj)) + std::string(".insert(")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                    }
                    else {
                        if ((method == std::string("upper"))) {
                            return ((std::string("lv_upper(") + (obj)) + std::string(")"));
                        }
                        else {
                            if ((method == std::string("lower"))) {
                                return ((std::string("lv_lower(") + (obj)) + std::string(")"));
                            }
                            else {
                                if ((method == std::string("trim"))) {
                                    return ((std::string("lv_trim(") + (obj)) + std::string(")"));
                                }
                                else {
                                    if ((method == std::string("replace"))) {
                                        return ((((std::string("lv_replace(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                    }
                                    else {
                                        if ((method == std::string("split"))) {
                                            if ((static_cast<int64_t>(args.size()) > 0LL)) {
                                                return ((((std::string("lv_split(") + (obj)) + std::string(", ")) + (args[0LL])) + std::string(")"));
                                            }
                                            return ((std::string("lv_split(") + (obj)) + std::string(", std::string(\" \"))"));
                                        }
                                        else {
                                            if ((method == std::string("starts_with"))) {
                                                return ((((std::string("") + (obj)) + std::string(".starts_with(")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                            }
                                            else {
                                                if ((method == std::string("ends_with"))) {
                                                    return ((((std::string("") + (obj)) + std::string(".ends_with(")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                }
                                                else {
                                                    if ((method == std::string("indexOf"))) {
                                                        if ((static_cast<int64_t>(args.size()) >= 2LL)) {
                                                            return ((((((std::string("lv_index_of(") + (obj)) + std::string(", ")) + (args[0LL])) + std::string(", ")) + (args[1LL])) + std::string(")"));
                                                        }
                                                        return ((((std::string("lv_index_of(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                    }
                                                    else {
                                                        if ((method == std::string("charAt"))) {
                                                            return ((((std::string("std::string(1, ") + (obj)) + std::string("[")) + (lv_join(args, std::string(", ")))) + std::string("])"));
                                                        }
                                                        else {
                                                            if ((method == std::string("substring"))) {
                                                                if ((static_cast<int64_t>(args.size()) >= 2LL)) {
                                                                    return ((((((((std::string("") + (obj)) + std::string(".substr(")) + (args[0LL])) + std::string(", (")) + (args[1LL])) + std::string(") - (")) + (args[0LL])) + std::string("))"));
                                                                }
                                                                return ((((std::string("") + (obj)) + std::string(".substr(")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                            }
                                                            else {
                                                                if ((method == std::string("push"))) {
                                                                    return ((((std::string("") + (obj)) + std::string(".push_back(")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                                }
                                                                else {
                                                                    if ((method == std::string("pop"))) {
                                                                        return ((std::string("lv_pop(") + (obj)) + std::string(")"));
                                                                    }
                                                                    else {
                                                                        if ((method == std::string("clear"))) {
                                                                            return ((std::string("") + (obj)) + std::string(".clear()"));
                                                                        }
                                                                        else {
                                                                            if ((method == std::string("remove"))) {
                                                                                return ((((std::string("lv_remove(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                                            }
                                                                            else {
                                                                                if ((method == std::string("join"))) {
                                                                                    if ((static_cast<int64_t>(args.size()) > 0LL)) {
                                                                                        return ((((std::string("lv_join(") + (obj)) + std::string(", ")) + (args[0LL])) + std::string(")"));
                                                                                    }
                                                                                    return ((std::string("lv_join(") + (obj)) + std::string(", std::string(\"\"))"));
                                                                                }
                                                                                else {
                                                                                    if ((method == std::string("reverse"))) {
                                                                                        return ((std::string("lv_reverse(") + (obj)) + std::string(")"));
                                                                                    }
                                                                                    else {
                                                                                        if ((method == std::string("keys"))) {
                                                                                            return ((std::string("lv_keys(") + (obj)) + std::string(")"));
                                                                                        }
                                                                                        else {
                                                                                            if ((method == std::string("values"))) {
                                                                                                return ((std::string("lv_values(") + (obj)) + std::string(")"));
                                                                                            }
                                                                                            else {
                                                                                                if ((method == std::string("has"))) {
                                                                                                    return ((((std::string("(") + (obj)) + std::string(".count(")) + (lv_join(args, std::string(", ")))) + std::string(") > 0)"));
                                                                                                }
                                                                                                else {
                                                                                                    if ((method == std::string("repeat"))) {
                                                                                                        return ((((std::string("lv_repeat(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                                                                    }
                                                                                                    else {
                                                                                                        if ((method == std::string("pad_left"))) {
                                                                                                            return ((((std::string("lv_pad_left(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                                                                        }
                                                                                                        else {
                                                                                                            if ((method == std::string("pad_right"))) {
                                                                                                                return ((((std::string("lv_pad_right(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                                                                            }
                                                                                                            else {
                                                                                                                if ((method == std::string("sort"))) {
                                                                                                                    return ((std::string("lv_sort(") + (obj)) + std::string(")"));
                                                                                                                }
                                                                                                                else {
                                                                                                                    if ((method == std::string("unique"))) {
                                                                                                                        return ((std::string("lv_unique(") + (obj)) + std::string(")"));
                                                                                                                    }
                                                                                                                    else {
                                                                                                                        if ((method == std::string("slice"))) {
                                                                                                                            return ((((std::string("lv_slice(") + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                                                                                                                        }
                                                                                                                        else {
                                                                                                                            if ((method == std::string("flatten"))) {
                                                                                                                                return ((std::string("lv_flatten(") + (obj)) + std::string(")"));
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

    std::string emit_params(const std::vector<Param>& params, bool track_dynamic) {
        std::vector<std::string> strs = {};
        for (const auto& p : params) {
            if (p.is_ref && p.is_mut) {
                strs.push_back(((((std::string("") + ((*this).emit_type(p.param_type))) + std::string("& ")) + (p.name.lexeme)) + std::string("")));
            }
            else {
                if (p.is_ref) {
                    strs.push_back(((((std::string("const ") + ((*this).emit_type(p.param_type))) + std::string("& ")) + (p.name.lexeme)) + std::string("")));
                }
                else {
                    strs.push_back(((((std::string("") + ((*this).emit_type(p.param_type))) + std::string(" ")) + (p.name.lexeme)) + std::string("")));
                }
            }
            this->var_types[p.name.lexeme] = p.param_type;
            if (track_dynamic) {
                {
                    const auto& _match_22 = p.param_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_22)>::Dynamic>(_match_22._data)) {
                        (*this).add_dynamic_var(p.name.lexeme);
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        return lv_join(strs, std::string(", "));
    }

    std::string emit_lambda(const std::vector<Param>& params, const Expr& body, bool in_method) {
        std::string body_str = (*this).emit_expr(body, in_method);
        return ((((std::string("[&](") + ((*this).emit_params(params, false))) + std::string(") { return ")) + (body_str)) + std::string("; }"));
    }

    std::string emit_block_lambda(const std::vector<Param>& params, int64_t body_id, bool in_method) {
        std::string param_str = (*this).emit_params(params, false);
        std::string saved_output = this->output;
        this->output = std::string("");
        int64_t saved_indent = this->indent_level;
        this->indent_level = 2LL;
        std::vector<Stmt> body = this->lambda_blocks[body_id];
        for (const auto& st : body) {
            (*this).emit_stmt(st, in_method);
        }
        std::string body_str = this->output;
        this->output = saved_output;
        this->indent_level = saved_indent;
        return ((((((std::string("[&](") + (param_str)) + std::string(") {\n")) + (body_str)) + std::string("")) + ((*this).indent())) + std::string("    }"));
    }

    void emit_stmt(const Stmt& s, bool m) {
        {
            const auto& _match_23 = s;
            if (_match_23._tag == "None") {
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::ExprStmt>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::ExprStmt>(_match_23._data);
                auto& expr = _v.expr;
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("")) + ((*this).emit_expr(expr, m))) + std::string(";\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Let>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Let>(_match_23._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                (*this).emit_let(name, var_type, initializer, m, is_ref, is_mut);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Const>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Const>(_match_23._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                std::string cpp_type = (*this).emit_type(const_type);
                std::string val = (*this).emit_expr(value, m);
                this->output = (this->output + ((((((((std::string("") + ((*this).indent())) + std::string("const ")) + (cpp_type)) + std::string(" ")) + (name.lexeme)) + std::string(" = ")) + (val)) + std::string(";\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Return>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Return>(_match_23._data);
                auto& keyword = _v.keyword;
                auto& value = _v.value;
                {
                    const auto& _match_24 = value;
                    if (_match_24._tag == "None") {
                        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("return;\n")));
                    }
                    else {
                        this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("return ")) + ((*this).emit_expr(value, m))) + std::string(";\n")));
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::If>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::If>(_match_23._data);
                auto& condition = _v.condition;
                auto& then_branch = *_v.then_branch;
                auto& else_branch = *_v.else_branch;
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("if (")) + ((*this).emit_expr(condition, m))) + std::string(") ")));
                (*this).emit_block_or_stmt(then_branch, m);
                {
                    const auto& _match_25 = else_branch;
                    if (_match_25._tag == "None") {
                        /* pass */
                    }
                    else {
                        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("else ")));
                        (*this).emit_block_or_stmt(else_branch, m);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::While>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::While>(_match_23._data);
                auto& condition = _v.condition;
                auto& body = *_v.body;
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("while (")) + ((*this).emit_expr(condition, m))) + std::string(") ")));
                (*this).emit_block_or_stmt(body, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::For>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::For>(_match_23._data);
                auto& item_name = _v.item_name;
                auto& collection = _v.collection;
                auto& body = *_v.body;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                {
                    const auto& _match_26 = collection;
                    if (std::holds_alternative<std::decay_t<decltype(_match_26)>::Range>(_match_26._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_26)>::Range>(_match_26._data);
                        auto& start = *_v.start;
                        auto& end = *_v.end;
                        this->output = (this->output + ((((((((((((std::string("") + ((*this).indent())) + std::string("for (int64_t ")) + (item_name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(start, m))) + std::string("; ")) + (item_name.lexeme)) + std::string(" < ")) + ((*this).emit_expr(end, m))) + std::string("; ")) + (item_name.lexeme)) + std::string("++) ")));
                        (*this).emit_block_or_stmt(body, m);
                    }
                    else {
                        {
                            const auto& _match_27 = collection;
                            if (std::holds_alternative<std::decay_t<decltype(_match_27)>::Variable>(_match_27._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_27)>::Variable>(_match_27._data);
                                auto& tok = _v.name;
                                if ((*this).is_dynamic_var(tok.lexeme)) {
                                    (*this).add_dynamic_var(item_name.lexeme);
                                }
                            }
                            else {
                                /* pass */
                            }
                        }
                        std::string for_type = std::string("auto");
                        if (is_ref && is_mut) {
                            for_type = std::string("auto&");
                        }
                        else {
                            if (is_ref) {
                                for_type = std::string("const auto&");
                            }
                        }
                        this->output = (this->output + ((((((((std::string("") + ((*this).indent())) + std::string("for (")) + (for_type)) + std::string(" ")) + (item_name.lexeme)) + std::string(" : ")) + ((*this).emit_expr(collection, m))) + std::string(") ")));
                        (*this).emit_block_or_stmt(body, m);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Block>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Block>(_match_23._data);
                auto& statements = _v.statements;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("{\n")));
                this->indent_level = (this->indent_level + 1LL);
                for (const auto& st : statements) {
                    (*this).emit_stmt(st, m);
                }
                this->indent_level = (this->indent_level - 1LL);
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Try>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Try>(_match_23._data);
                auto& try_body = *_v.try_body;
                auto& catch_body = *_v.catch_body;
                auto& exception_name = _v.exception_name;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("try ")));
                (*this).emit_block_or_stmt(try_body, m);
                std::string exc_name = std::string("e");
                if ((exception_name != std::string(""))) {
                    exc_name = exception_name;
                }
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string(" catch (const std::exception& ")) + (exc_name)) + std::string(") ")));
                (*this).emit_block_or_stmt(catch_body, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Function>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Function>(_match_23._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                (*this).emit_function(name, params, return_type, body, type_params, comptime_mode);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Class>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Class>(_match_23._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                std::vector<std::string> empty_tp = {};
                (*this).emit_class(name, body, empty_tp);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Struct>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Struct>(_match_23._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                (*this).emit_class(name, body, type_params);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Enum>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Enum>(_match_23._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                (*this).emit_enum(name, variants, methods, type_params);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Match>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Match>(_match_23._data);
                auto& expr = _v.expr;
                auto& arm_patterns = _v.arm_patterns;
                auto& arm_bodies = _v.arm_bodies;
                (*this).emit_match_impl(expr, arm_patterns, arm_bodies, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Namespace>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Namespace>(_match_23._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("// TODO: unsupported namespace\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Import>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Import>(_match_23._data);
                auto& path = _v.path;
                auto& alias = _v.alias;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("// TODO: unsupported import\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Break>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Break>(_match_23._data);
                auto& keyword = _v.keyword;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("break;\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Continue>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Continue>(_match_23._data);
                auto& keyword = _v.keyword;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("continue;\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Pass>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Pass>(_match_23._data);
                auto& keyword = _v.keyword;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("/* pass */\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::CppBlock>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::CppBlock>(_match_23._data);
                auto& code = _v.code;
                auto lines = lv_split(code, std::string("\n"));
                for (const auto& line : lines) {
                    auto trimmed = lv_trim(line);
                    if ((trimmed != std::string(""))) {
                        this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("")) + (trimmed)) + std::string("\n")));
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_23)>::Extern>(_match_23._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_23)>::Extern>(_match_23._data);
                auto& header = _v.header;
                auto& link_lib = _v.link_lib;
                auto& types = _v.types;
                auto& functions = _v.functions;
                std::string inc_line = std::string("");
                bool has_dot = (lv_index_of(header, std::string(".")) >= 0LL);
                if (has_dot) {
                    inc_line = ((std::string("#include \"") + (header)) + std::string("\""));
                }
                else {
                    inc_line = ((std::string("#include <") + (header)) + std::string(">"));
                }
                bool already = false;
                for (const auto& existing : this->extern_includes) {
                    if ((existing == inc_line)) {
                        already = true;
                    }
                }
                if ((!already)) {
                    this->extern_includes.push_back(inc_line);
                }
                if ((link_lib != std::string(""))) {
                    this->extern_link_libs.push_back(link_lib);
                }
                for (const auto& et : types) {
                    if ((et.lavina_name != et.cpp_name)) {
                        this->extern_type_names[et.lavina_name] = et.cpp_name;
                    }
                }
                for (const auto& ef : functions) {
                    if ((ef.name != ef.cpp_name)) {
                        this->extern_fn_names[ef.name] = ef.cpp_name;
                    }
                    this->extern_fn_params[ef.name] = ef.params;
                }
            }
            else {
                /* pass */
            }
        }
    }

    void emit_let(const Token& name, const TypeNode& var_type, const Expr& initializer, bool in_method, bool is_ref, bool is_mut) {
        std::string cpp_type = (*this).emit_type(var_type);
        this->var_types[name.lexeme] = var_type;
        if ((*this).type_contains_dynamic(var_type)) {
            (*this).add_dynamic_var(name.lexeme);
        }
        std::string const_str = std::string("");
        std::string ref_str = std::string("");
        if (is_ref) {
            ref_str = std::string("&");
            if ((!is_mut)) {
                const_str = std::string("const ");
            }
        }
        std::string init_str = std::string("");
        {
            const auto& _match_28 = initializer;
            if (_match_28._tag == "None") {
                init_str = (*this).default_init(cpp_type);
            }
            else {
                std::string val = (*this).emit_expr(initializer, in_method);
                bool is_ptr = false;
                {
                    const auto& _match_29 = var_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_29)>::Ptr>(_match_29._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_29)>::Ptr>(_match_29._data);
                        auto& inner = *_v.inner;
                        is_ptr = true;
                    }
                    else {
                        /* pass */
                    }
                }
                if (is_ptr && (val == std::string("std::nullopt"))) {
                    val = std::string("nullptr");
                }
                init_str = ((std::string(" = ") + (val)) + std::string(""));
            }
        }
        this->output = (this->output + ((((((((((((std::string("") + ((*this).indent())) + std::string("")) + (const_str)) + std::string("")) + (cpp_type)) + std::string("")) + (ref_str)) + std::string(" ")) + (name.lexeme)) + std::string("")) + (init_str)) + std::string(";\n")));
    }

    void emit_block_or_stmt(const Stmt& s, bool m) {
        {
            const auto& _match_30 = s;
            if (std::holds_alternative<std::decay_t<decltype(_match_30)>::Block>(_match_30._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_30)>::Block>(_match_30._data);
                auto& stmts = _v.statements;
                this->output = (this->output + std::string("{\n"));
                this->indent_level = (this->indent_level + 1LL);
                for (const auto& st : stmts) {
                    (*this).emit_stmt(st, m);
                }
                this->indent_level = (this->indent_level - 1LL);
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
            else {
                this->output = (this->output + std::string("{\n"));
                this->indent_level = (this->indent_level + 1LL);
                (*this).emit_stmt(s, m);
                this->indent_level = (this->indent_level - 1LL);
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
        }
    }

    void emit_function(const Token& name, const std::vector<Param>& params, const TypeNode& return_type, const std::vector<Stmt>& body, const std::vector<std::string>& type_params, int64_t comptime_mode) {
        std::vector<std::string> saved_dynamic = this->dynamic_vars;
        this->output = (this->output + (*this).template_prefix(type_params));
        std::string ret_type = (*this).emit_type(return_type);
        std::string param_str = (*this).emit_params(params, true);
        std::string prefix = std::string("");
        if ((comptime_mode == 1LL)) {
            prefix = std::string("constexpr ");
        }
        else {
            if ((comptime_mode == 2LL)) {
                prefix = std::string("consteval ");
            }
        }
        this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (prefix)) + std::string("")) + (ret_type)) + std::string(" ")) + (name.lexeme)) + std::string("(")) + (param_str)) + std::string(") {\n")));
        this->indent_level = (this->indent_level + 1LL);
        for (const auto& st : body) {
            (*this).emit_stmt(st, false);
        }
        this->indent_level = (this->indent_level - 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n\n")));
        this->dynamic_vars = saved_dynamic;
    }

    void emit_class_fields(const std::vector<std::string>& init_field_names, const std::vector<std::string>& init_field_types, const std::vector<std::string>& let_field_names, const std::vector<std::string>& let_field_types) {
        for (int64_t fi = 0LL; fi < static_cast<int64_t>(init_field_names.size()); fi++) {
            bool is_let = false;
            for (const auto& lfn : let_field_names) {
                if ((lfn == init_field_names[fi])) {
                    is_let = true;
                }
            }
            if ((!is_let)) {
                this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (init_field_types[fi])) + std::string(" ")) + (init_field_names[fi])) + std::string(";\n")));
            }
        }
        for (int64_t li = 0LL; li < static_cast<int64_t>(let_field_names.size()); li++) {
            this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (let_field_types[li])) + std::string(" ")) + (let_field_names[li])) + std::string(";\n")));
        }
        if ((static_cast<int64_t>(init_field_names.size()) > 0LL) || (static_cast<int64_t>(let_field_names.size()) > 0LL)) {
            this->output = (this->output + std::string("\n"));
        }
    }

    void emit_constructor(const Token& name, const std::vector<Param>& init_params, const std::vector<Stmt>& init_body) {
        std::string param_str = (*this).emit_params(init_params, false);
        std::vector<std::string> init_list = {};
        std::vector<Stmt> remaining_body = {};
        for (const auto& st : init_body) {
            {
                const auto& _match_31 = st;
                if (std::holds_alternative<std::decay_t<decltype(_match_31)>::ExprStmt>(_match_31._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_31)>::ExprStmt>(_match_31._data);
                    auto& expr = _v.expr;
                    bool handled = false;
                    {
                        const auto& _match_32 = expr;
                        if (std::holds_alternative<std::decay_t<decltype(_match_32)>::Set>(_match_32._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_32)>::Set>(_match_32._data);
                            auto& object = *_v.object;
                            auto& prop = _v.name;
                            auto& value = *_v.value;
                            {
                                const auto& _match_33 = object;
                                if (std::holds_alternative<std::decay_t<decltype(_match_33)>::This>(_match_33._data)) {
                                    auto& _v = std::get<std::decay_t<decltype(_match_33)>::This>(_match_33._data);
                                    auto& kw = _v.keyword;
                                    {
                                        const auto& _match_34 = value;
                                        if (std::holds_alternative<std::decay_t<decltype(_match_34)>::Variable>(_match_34._data)) {
                                            auto& _v = std::get<std::decay_t<decltype(_match_34)>::Variable>(_match_34._data);
                                            auto& tok = _v.name;
                                            init_list.push_back(((((std::string("") + (prop.lexeme)) + std::string("(")) + (tok.lexeme)) + std::string(")")));
                                            handled = true;
                                        }
                                        else {
                                            /* pass */
                                        }
                                    }
                                }
                                else {
                                    /* pass */
                                }
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                    if ((!handled)) {
                        remaining_body.push_back(st);
                    }
                }
                else {
                    remaining_body.push_back(st);
                }
            }
        }
        if ((static_cast<int64_t>(init_list.size()) == 0LL)) {
            this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (name.lexeme)) + std::string("(")) + (param_str)) + std::string(") {\n")));
        }
        else {
            this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (name.lexeme)) + std::string("(")) + (param_str)) + std::string(")\n")));
            this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("    : ")) + (lv_join(init_list, std::string(", ")))) + std::string(" {\n")));
        }
        std::vector<std::string> saved_dyn = this->dynamic_vars;
        this->indent_level = (this->indent_level + 1LL);
        for (const auto& st : remaining_body) {
            (*this).emit_stmt(st, true);
        }
        this->indent_level = (this->indent_level - 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n\n")));
        this->dynamic_vars = saved_dyn;
    }

    void emit_class_method(const Token& class_name, const Stmt& method) {
        {
            const auto& _match_35 = method;
            if (std::holds_alternative<std::decay_t<decltype(_match_35)>::Function>(_match_35._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_35)>::Function>(_match_35._data);
                auto& mname = _v.name;
                auto& mparams = _v.params;
                auto& mret = _v.return_type;
                auto& mbody = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& m_tp = _v.type_params;
                std::vector<std::string> saved_dyn = this->dynamic_vars;
                std::string ret_type = (*this).emit_type(mret);
                std::string mparam_str = (*this).emit_params(mparams, true);
                std::string ct_prefix = std::string("");
                if ((comptime_mode == 1LL)) {
                    ct_prefix = std::string("constexpr ");
                }
                else {
                    if ((comptime_mode == 2LL)) {
                        ct_prefix = std::string("consteval ");
                    }
                }
                if ((mname.lexeme == std::string("destructor"))) {
                    this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("~")) + (class_name.lexeme)) + std::string("() {\n")));
                }
                else {
                    if ((mname.lexeme == std::string("to_string"))) {
                        this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (ct_prefix)) + std::string("")) + (ret_type)) + std::string(" ")) + (mname.lexeme)) + std::string("(")) + (mparam_str)) + std::string(") const {\n")));
                    }
                    else {
                        this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (ct_prefix)) + std::string("")) + (ret_type)) + std::string(" ")) + (mname.lexeme)) + std::string("(")) + (mparam_str)) + std::string(") {\n")));
                    }
                }
                this->indent_level = (this->indent_level + 1LL);
                for (const auto& st : mbody) {
                    (*this).emit_stmt(st, true);
                }
                this->indent_level = (this->indent_level - 1LL);
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n\n")));
                this->dynamic_vars = saved_dyn;
            }
            else {
                /* pass */
            }
        }
    }

    void emit_class(const Token& name, const std::vector<Stmt>& body, const std::vector<std::string>& type_params) {
        std::vector<Stmt> init_body = {};
        std::vector<Param> init_params = {};
        bool has_init = false;
        std::vector<Stmt> methods = {};
        std::vector<std::string> let_field_names = {};
        std::vector<std::string> let_field_types = {};
        for (const auto& st : body) {
            {
                const auto& _match_36 = st;
                if (std::holds_alternative<std::decay_t<decltype(_match_36)>::Function>(_match_36._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_36)>::Function>(_match_36._data);
                    auto& fname = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& fbody = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& fn_tp = _v.type_params;
                    if ((fname.lexeme == std::string("constructor"))) {
                        has_init = true;
                        init_body = fbody;
                        init_params = params;
                    }
                    else {
                        methods.push_back(st);
                    }
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_36)>::Let>(_match_36._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_36)>::Let>(_match_36._data);
                    auto& fname = _v.name;
                    auto& var_type = _v.var_type;
                    auto& init = _v.initializer;
                    auto& visibility = _v.visibility;
                    auto& is_ref = _v.is_ref;
                    auto& is_mut = _v.is_mut;
                    let_field_names.push_back(fname.lexeme);
                    let_field_types.push_back((*this).emit_type(var_type));
                }
                else {
                    /* pass */
                }
            }
        }
        std::vector<std::string> init_field_names = {};
        std::vector<std::string> init_field_types = {};
        if (has_init) {
            std::vector<std::string> param_names = {};
            std::vector<std::string> param_types = {};
            for (const auto& p : init_params) {
                param_names.push_back(p.name.lexeme);
                param_types.push_back((*this).emit_type(p.param_type));
            }
            std::vector<std::string> seen = {};
            for (const auto& st : init_body) {
                {
                    const auto& _match_37 = st;
                    if (std::holds_alternative<std::decay_t<decltype(_match_37)>::ExprStmt>(_match_37._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_37)>::ExprStmt>(_match_37._data);
                        auto& expr = _v.expr;
                        {
                            const auto& _match_38 = expr;
                            if (std::holds_alternative<std::decay_t<decltype(_match_38)>::Set>(_match_38._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_38)>::Set>(_match_38._data);
                                auto& object = *_v.object;
                                auto& prop = _v.name;
                                auto& value = *_v.value;
                                {
                                    const auto& _match_39 = object;
                                    if (std::holds_alternative<std::decay_t<decltype(_match_39)>::This>(_match_39._data)) {
                                        auto& _v = std::get<std::decay_t<decltype(_match_39)>::This>(_match_39._data);
                                        auto& kw = _v.keyword;
                                        bool already = false;
                                        for (const auto& s : seen) {
                                            if ((s == prop.lexeme)) {
                                                already = true;
                                            }
                                        }
                                        if ((!already)) {
                                            seen.push_back(prop.lexeme);
                                            std::string cpp_type = (*this).infer_expr_type(value, param_names, param_types);
                                            init_field_names.push_back(prop.lexeme);
                                            init_field_types.push_back(cpp_type);
                                        }
                                    }
                                    else {
                                        /* pass */
                                    }
                                }
                            }
                            else {
                                /* pass */
                            }
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        this->output = (this->output + ((((((std::string("") + ((*this).template_prefix(type_params))) + std::string("")) + ((*this).indent())) + std::string("struct ")) + (name.lexeme)) + std::string(" {\n")));
        this->indent_level = (this->indent_level + 1LL);
        (*this).emit_class_fields(init_field_names, init_field_types, let_field_names, let_field_types);
        if (has_init) {
            (*this).emit_constructor(name, init_params, init_body);
        }
        for (const auto& m : methods) {
            (*this).emit_class_method(name, m);
        }
        this->indent_level = (this->indent_level - 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("};\n\n")));
        bool has_to_string = false;
        for (const auto& m : methods) {
            {
                const auto& _match_40 = m;
                if (std::holds_alternative<std::decay_t<decltype(_match_40)>::Function>(_match_40._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_40)>::Function>(_match_40._data);
                    auto& mname = _v.name;
                    auto& mparams = _v.params;
                    auto& mret = _v.return_type;
                    auto& mbody = _v.body;
                    auto& mi = _v.is_inline;
                    auto& mc = _v.comptime_mode;
                    auto& ms = _v.is_static;
                    auto& mv = _v.visibility;
                    auto& mtp = _v.type_params;
                    if ((mname.lexeme == std::string("to_string"))) {
                        has_to_string = true;
                    }
                }
                else {
                    /* pass */
                }
            }
        }
        if (has_to_string) {
            std::string tmpl_line = (*this).template_prefix(type_params);
            std::string type_suffix = std::string("");
            if ((static_cast<int64_t>(type_params.size()) > 0LL)) {
                type_suffix = ((std::string("<") + (lv_join(type_params, std::string(", ")))) + std::string(">"));
            }
            std::string full_name = ((((std::string("") + (name.lexeme)) + std::string("")) + (type_suffix)) + std::string(""));
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("")) + ((*this).indent())) + std::string("void print(const ")) + (full_name)) + std::string("& _v) { std::cout << _v.to_string() << std::endl; }\n")));
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("")) + ((*this).indent())) + std::string("std::string operator+(const std::string& _s, const ")) + (full_name)) + std::string("& _v) { return _s + _v.to_string(); }\n")));
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("")) + ((*this).indent())) + std::string("std::string operator+(const ")) + (full_name)) + std::string("& _v, const std::string& _s) { return _v.to_string() + _s; }\n\n")));
        }
    }

    std::string infer_expr_type(const Expr& e, const std::vector<std::string>& param_names, const std::vector<std::string>& param_types) {
        {
            const auto& _match_41 = e;
            if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Literal>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Literal>(_match_41._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("int"))) {
                    return std::string("int64_t");
                }
                else {
                    if ((kind == std::string("float"))) {
                        return std::string("double");
                    }
                    else {
                        if ((kind == std::string("string"))) {
                            return std::string("std::string");
                        }
                        else {
                            if ((kind == std::string("bool"))) {
                                return std::string("bool");
                            }
                            else {
                                if ((kind == std::string("null"))) {
                                    return std::string("std::nullptr_t");
                                }
                            }
                        }
                    }
                }
                return std::string("std::any");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Variable>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Variable>(_match_41._data);
                auto& tok = _v.name;
                for (int64_t i = 0LL; i < static_cast<int64_t>(param_names.size()); i++) {
                    if ((param_names[i] == tok.lexeme)) {
                        if ((param_types[i] == std::string("auto"))) {
                            return std::string("std::any");
                        }
                        return param_types[i];
                    }
                }
                return std::string("std::any");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Binary>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Binary>(_match_41._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string lt = (*this).infer_expr_type(left, param_names, param_types);
                std::string rt = (*this).infer_expr_type(right, param_names, param_types);
                if ((lt == rt) && (lt != std::string("std::any"))) {
                    return lt;
                }
                if ((lt == std::string("std::string")) || (rt == std::string("std::string"))) {
                    return std::string("std::string");
                }
                return std::string("std::any");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Unary>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Unary>(_match_41._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                return (*this).infer_expr_type(right, param_names, param_types);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Grouping>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Grouping>(_match_41._data);
                auto& inner = *_v.inner;
                return (*this).infer_expr_type(inner, param_names, param_types);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Call>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Call>(_match_41._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                {
                    const auto& _match_42 = callee;
                    if (std::holds_alternative<std::decay_t<decltype(_match_42)>::Variable>(_match_42._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_42)>::Variable>(_match_42._data);
                        auto& tok = _v.name;
                        if ((*this).is_known_enum(tok.lexeme)) {
                            return tok.lexeme;
                        }
                        for (int64_t ci = 0LL; ci < static_cast<int64_t>(param_names.size()); ci++) {
                            if ((param_names[ci] == tok.lexeme)) {
                                return param_types[ci];
                            }
                        }
                        return tok.lexeme;
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_42)>::StaticGet>(_match_42._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_42)>::StaticGet>(_match_42._data);
                        auto& object = *_v.object;
                        auto& name = _v.name;
                        {
                            const auto& _match_43 = object;
                            if (std::holds_alternative<std::decay_t<decltype(_match_43)>::Variable>(_match_43._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_43)>::Variable>(_match_43._data);
                                auto& tok = _v.name;
                                if ((*this).is_known_enum(tok.lexeme)) {
                                    return tok.lexeme;
                                }
                            }
                            else {
                                /* pass */
                            }
                        }
                        return std::string("std::any");
                    }
                    else {
                        return std::string("std::any");
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Vector>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Vector>(_match_41._data);
                auto& elements = _v.elements;
                if ((static_cast<int64_t>(elements.size()) > 0LL)) {
                    std::string inner = (*this).infer_expr_type(elements[0LL], param_names, param_types);
                    if ((inner != std::string("std::any"))) {
                        return ((std::string("std::vector<") + (inner)) + std::string(">"));
                    }
                }
                return std::string("std::vector<std::any>");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Map>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Map>(_match_41._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                std::string kt = std::string("std::any");
                std::string vt = std::string("std::any");
                if ((static_cast<int64_t>(keys.size()) > 0LL)) {
                    kt = (*this).infer_expr_type(keys[0LL], param_names, param_types);
                    vt = (*this).infer_expr_type(values[0LL], param_names, param_types);
                }
                return ((((std::string("std::unordered_map<") + (kt)) + std::string(", ")) + (vt)) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_41)>::Cast>(_match_41._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_41)>::Cast>(_match_41._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                return (*this).emit_type(target_type);
            }
            else {
                return std::string("std::any");
            }
        }
    }

    void emit_enum_structs(const std::string& enum_name, const std::vector<EnumVariantNode>& variants, const std::string& type_suffix) {
        for (const auto& v : variants) {
            if ((static_cast<int64_t>(v.types.size()) > 0LL)) {
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("struct ")) + (v.name.lexeme)) + std::string(" { ")));
                for (int64_t fi = 0LL; fi < static_cast<int64_t>(v.types.size()); fi++) {
                    std::string cpp_type = (*this).emit_type(v.types[fi]);
                    std::string fname = v.field_names[fi];
                    {
                        const auto& _match_44 = v.types[fi];
                        if (std::holds_alternative<std::decay_t<decltype(_match_44)>::Custom>(_match_44._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_44)>::Custom>(_match_44._data);
                            auto& n = _v.name;
                            auto& _ = _v.type_args;
                            if ((n == enum_name)) {
                                cpp_type = ((((std::string("std::shared_ptr<") + (enum_name)) + std::string("")) + (type_suffix)) + std::string(">"));
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                    this->output = (this->output + ((((std::string("") + (cpp_type)) + std::string(" ")) + (fname)) + std::string("; ")));
                }
                this->output = (this->output + std::string("};\n"));
            }
            else {
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("struct ")) + (v.name.lexeme)) + std::string(" {};\n")));
            }
        }
        this->output = (this->output + std::string("\n"));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("std::string _tag;\n")));
        std::vector<std::string> variant_inner_types = {};
        for (const auto& v : variants) {
            variant_inner_types.push_back(((((std::string("") + (enum_name)) + std::string("::")) + (v.name.lexeme)) + std::string("")));
        }
        this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("std::variant<")) + (lv_join(variant_inner_types, std::string(", ")))) + std::string("> _data;\n")));
        this->output = (this->output + std::string("\n"));
    }

    void emit_enum_makers(const std::string& enum_name, const std::vector<EnumVariantNode>& variants, const std::string& type_suffix) {
        for (const auto& v : variants) {
            std::string vname = v.name.lexeme;
            if ((static_cast<int64_t>(v.types.size()) == 0LL)) {
                this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("static ")) + (enum_name)) + std::string(" make_")) + (vname)) + std::string("() { return {\"")) + (vname)) + std::string("\", ")) + (vname)) + std::string("{}}; }\n")));
            }
            else {
                std::vector<std::string> params = {};
                std::vector<std::string> field_inits = {};
                for (int64_t fi = 0LL; fi < static_cast<int64_t>(v.types.size()); fi++) {
                    std::string cpp_type = (*this).emit_type(v.types[fi]);
                    std::string fname = v.field_names[fi];
                    bool is_self_ref = false;
                    {
                        const auto& _match_45 = v.types[fi];
                        if (std::holds_alternative<std::decay_t<decltype(_match_45)>::Custom>(_match_45._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_45)>::Custom>(_match_45._data);
                            auto& n = _v.name;
                            auto& _ = _v.type_args;
                            if ((n == enum_name)) {
                                is_self_ref = true;
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                    if (is_self_ref) {
                        params.push_back(((((((std::string("") + (enum_name)) + std::string("")) + (type_suffix)) + std::string(" ")) + (fname)) + std::string("")));
                        field_inits.push_back(((((((std::string("std::make_shared<") + (enum_name)) + std::string("")) + (type_suffix)) + std::string(">(std::move(")) + (fname)) + std::string("))")));
                    }
                    else {
                        params.push_back(((((std::string("") + (cpp_type)) + std::string(" ")) + (fname)) + std::string("")));
                        field_inits.push_back(fname);
                    }
                }
                this->output = (this->output + ((((((((((((((std::string("") + ((*this).indent())) + std::string("static ")) + (enum_name)) + std::string(" make_")) + (vname)) + std::string("(")) + (lv_join(params, std::string(", ")))) + std::string(") { return {\"")) + (vname)) + std::string("\", ")) + (vname)) + std::string("{")) + (lv_join(field_inits, std::string(", ")))) + std::string("}}; }\n")));
            }
        }
        this->output = (this->output + std::string("\n"));
    }

    void emit_enum_operators(const std::string& enum_name, const std::string& type_suffix, const std::string& tmpl_line, bool has_to_string) {
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("std::string operator[](const std::string& key) const {\n")));
        this->indent_level = (this->indent_level + 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("if (key == \"_tag\") return _tag;\n")));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("return \"\";\n")));
        this->indent_level = (this->indent_level - 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
        this->indent_level = (this->indent_level - 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("};\n\n")));
        if (has_to_string) {
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("void print(const ")) + (enum_name)) + std::string("")) + (type_suffix)) + std::string("& e) { std::cout << e.to_string() << std::endl; }\n")));
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const std::string& s, const ")) + (enum_name)) + std::string("")) + (type_suffix)) + std::string("& e) { return s + e.to_string(); }\n")));
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const ")) + (enum_name)) + std::string("")) + (type_suffix)) + std::string("& e, const std::string& s) { return e.to_string() + s; }\n\n")));
        }
        else {
            this->output = (this->output + ((((((((std::string("") + (tmpl_line)) + std::string("void print(const ")) + (enum_name)) + std::string("")) + (type_suffix)) + std::string("& e) { std::cout << \"")) + (enum_name)) + std::string("(\" << e._tag << \")\" << std::endl; }\n")));
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const std::string& s, const ")) + (enum_name)) + std::string("")) + (type_suffix)) + std::string("& e) { return s + e._tag; }\n")));
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const ")) + (enum_name)) + std::string("")) + (type_suffix)) + std::string("& e, const std::string& s) { return e._tag + s; }\n\n")));
        }
    }

    void emit_enum(const Token& name, const std::vector<EnumVariantNode>& variants, const std::vector<Stmt>& methods, const std::vector<std::string>& type_params) {
        std::string enum_name = name.lexeme;
        this->known_enums[enum_name] = variants;
        bool has_self_ref = false;
        for (const auto& v : variants) {
            for (const auto& ft : v.types) {
                {
                    const auto& _match_46 = ft;
                    if (std::holds_alternative<std::decay_t<decltype(_match_46)>::Custom>(_match_46._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_46)>::Custom>(_match_46._data);
                        auto& n = _v.name;
                        auto& _ = _v.type_args;
                        if ((n == enum_name)) {
                            has_self_ref = true;
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        std::string tmpl_line = (*this).template_prefix(type_params);
        std::string type_suffix = std::string("");
        if ((static_cast<int64_t>(type_params.size()) > 0LL)) {
            type_suffix = ((std::string("<") + (lv_join(type_params, std::string(", ")))) + std::string(">"));
        }
        if (has_self_ref) {
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("")) + ((*this).indent())) + std::string("struct ")) + (enum_name)) + std::string(";\n")));
        }
        this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("")) + ((*this).indent())) + std::string("struct ")) + (enum_name)) + std::string(" {\n")));
        this->indent_level = (this->indent_level + 1LL);
        (*this).emit_enum_structs(enum_name, variants, type_suffix);
        (*this).emit_enum_makers(enum_name, variants, type_suffix);
        for (const auto& m : methods) {
            (*this).emit_class_method(name, m);
        }
        bool has_to_string = false;
        for (const auto& m : methods) {
            {
                const auto& _match_47 = m;
                if (std::holds_alternative<std::decay_t<decltype(_match_47)>::Function>(_match_47._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_47)>::Function>(_match_47._data);
                    auto& mname = _v.name;
                    auto& mparams = _v.params;
                    auto& mret = _v.return_type;
                    auto& mbody = _v.body;
                    auto& mi = _v.is_inline;
                    auto& mc = _v.comptime_mode;
                    auto& ms = _v.is_static;
                    auto& mv = _v.visibility;
                    auto& mtp = _v.type_params;
                    if ((mname.lexeme == std::string("to_string"))) {
                        has_to_string = true;
                    }
                }
                else {
                    /* pass */
                }
            }
        }
        (*this).emit_enum_operators(enum_name, type_suffix, tmpl_line, has_to_string);
    }

    void emit_match_impl(const Expr& expr, const std::vector<MatchArm>& arm_patterns, const std::vector<Stmt>& arm_bodies, bool in_method) {
        std::string match_val = (*this).emit_expr(expr, in_method);
        std::string temp = ((std::string("_match_") + (this->temp_counter)) + std::string(""));
        this->temp_counter = (this->temp_counter + 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("{\n")));
        this->indent_level = (this->indent_level + 1LL);
        this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("const auto& ")) + (temp)) + std::string(" = ")) + (match_val)) + std::string(";\n")));
        bool first = true;
        for (int64_t ai = 0LL; ai < static_cast<int64_t>(arm_patterns.size()); ai++) {
            auto arm = arm_patterns[ai];
            Stmt arm_body = arm_bodies[ai];
            if ((arm.pattern_name == std::string("_"))) {
                if (first) {
                    this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("{\n")));
                }
                else {
                    this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("else {\n")));
                }
                this->indent_level = (this->indent_level + 1LL);
                (*this).emit_arm_body(arm_body, in_method);
                this->indent_level = (this->indent_level - 1LL);
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
            else {
                std::string keyword = std::string("if");
                if ((!first)) {
                    keyword = std::string("else if");
                }
                first = false;
                std::string ename = (*this).find_enum_for_variant(arm.pattern_name);
                if ((ename != std::string(""))) {
                    this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (keyword)) + std::string(" (std::holds_alternative<std::decay_t<decltype(")) + (temp)) + std::string(")>::")) + (arm.pattern_name)) + std::string(">(")) + (temp)) + std::string("._data)) {\n")));
                }
                else {
                    this->output = (this->output + ((((((((std::string("") + ((*this).indent())) + std::string("")) + (keyword)) + std::string(" (")) + (temp)) + std::string("._tag == \"")) + (arm.pattern_name)) + std::string("\") {\n")));
                }
                this->indent_level = (this->indent_level + 1LL);
                if ((static_cast<int64_t>(arm.bindings.size()) > 0LL)) {
                    if ((ename != std::string(""))) {
                        this->output = (this->output + ((((((((std::string("") + ((*this).indent())) + std::string("auto& _v = std::get<std::decay_t<decltype(")) + (temp)) + std::string(")>::")) + (arm.pattern_name)) + std::string(">(")) + (temp)) + std::string("._data);\n")));
                        auto vinfo = (*this).get_variant_info(ename, arm.pattern_name);
                        for (int64_t bi = 0LL; bi < static_cast<int64_t>(arm.bindings.size()); bi++) {
                            if ((bi < static_cast<int64_t>(vinfo.field_names.size()))) {
                                std::string field_name = vinfo.field_names[bi];
                                bool is_self_ref = false;
                                if ((bi < static_cast<int64_t>(vinfo.types.size()))) {
                                    {
                                        const auto& _match_48 = vinfo.types[bi];
                                        if (std::holds_alternative<std::decay_t<decltype(_match_48)>::Custom>(_match_48._data)) {
                                            auto& _v = std::get<std::decay_t<decltype(_match_48)>::Custom>(_match_48._data);
                                            auto& n = _v.name;
                                            auto& _ = _v.type_args;
                                            if ((n == ename)) {
                                                is_self_ref = true;
                                            }
                                        }
                                        else {
                                            /* pass */
                                        }
                                    }
                                }
                                if (is_self_ref) {
                                    this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("auto& ")) + (arm.bindings[bi])) + std::string(" = *_v.")) + (field_name)) + std::string(";\n")));
                                }
                                else {
                                    this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("auto& ")) + (arm.bindings[bi])) + std::string(" = _v.")) + (field_name)) + std::string(";\n")));
                                }
                            }
                        }
                    }
                }
                (*this).emit_arm_body(arm_body, in_method);
                this->indent_level = (this->indent_level - 1LL);
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
        }
        this->indent_level = (this->indent_level - 1LL);
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
    }

    void emit_arm_body(const Stmt& arm_body, bool in_method) {
        {
            const auto& _match_49 = arm_body;
            if (std::holds_alternative<std::decay_t<decltype(_match_49)>::Block>(_match_49._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_49)>::Block>(_match_49._data);
                auto& stmts = _v.statements;
                for (const auto& st : stmts) {
                    (*this).emit_stmt(st, in_method);
                }
            }
            else {
                (*this).emit_stmt(arm_body, in_method);
            }
        }
    }

    void emit_using_if_public(const std::string& ns, const Stmt& stmt) {
        {
            const auto& _match_50 = stmt;
            if (std::holds_alternative<std::decay_t<decltype(_match_50)>::Function>(_match_50._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_50)>::Function>(_match_50._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_50)>::Class>(_match_50._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_50)>::Class>(_match_50._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_50)>::Struct>(_match_50._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_50)>::Struct>(_match_50._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_50)>::Enum>(_match_50._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_50)>::Enum>(_match_50._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_50)>::Const>(_match_50._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_50)>::Const>(_match_50._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_50)>::Let>(_match_50._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_50)>::Let>(_match_50._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else {
                /* pass */
            }
        }
    }

    void emit_module(int64_t index) {
        std::string full_name = this->module_full_names[index];
        std::string short_name = this->module_short_names[index];
        std::string alias = this->module_aliases[index];
        this->output = (this->output + ((std::string("namespace ") + (full_name)) + std::string(" {\n")));
        this->indent_level = 1LL;
        for (const auto& stmt : this->module_stmts[index]) {
            {
                const auto& _match_51 = stmt;
                if (std::holds_alternative<std::decay_t<decltype(_match_51)>::Function>(_match_51._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_51)>::Function>(_match_51._data);
                    auto& name = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& body = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& type_params = _v.type_params;
                    if ((name.lexeme == std::string("main"))) {
                        /* pass */
                    }
                    else {
                        (*this).emit_stmt(stmt, false);
                    }
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_51)>::Extern>(_match_51._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_51)>::Extern>(_match_51._data);
                    auto& header = _v.header;
                    auto& link_lib = _v.link_lib;
                    auto& types = _v.types;
                    auto& functions = _v.functions;
                    /* pass */
                }
                else {
                    (*this).emit_stmt(stmt, false);
                }
            }
        }
        this->indent_level = 0LL;
        this->output = (this->output + ((std::string("} // namespace ") + (full_name)) + std::string("\n")));
        if ((short_name != full_name)) {
            this->output = (this->output + ((((std::string("namespace ") + (short_name)) + std::string(" = ")) + (full_name)) + std::string(";\n")));
        }
        if ((alias != std::string(""))) {
            this->output = (this->output + ((((std::string("namespace ") + (alias)) + std::string(" = ")) + (full_name)) + std::string(";\n")));
        }
        for (const auto& stmt : this->module_stmts[index]) {
            (*this).emit_using_if_public(full_name, stmt);
        }
        this->output = (this->output + std::string("\n"));
        this->declarations = (this->declarations + this->output);
        this->output = std::string("");
    }

    std::string generate(const std::vector<Stmt>& stmts) {
        for (int64_t mi = 0LL; mi < static_cast<int64_t>(this->module_stmts.size()); mi++) {
            for (const auto& stmt : this->module_stmts[mi]) {
                {
                    const auto& _match_52 = stmt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_52)>::Extern>(_match_52._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_52)>::Extern>(_match_52._data);
                        auto& header = _v.header;
                        auto& link_lib = _v.link_lib;
                        auto& types = _v.types;
                        auto& functions = _v.functions;
                        (*this).emit_stmt(stmt, false);
                        this->output = std::string("");
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        for (const auto& stmt : stmts) {
            {
                const auto& _match_53 = stmt;
                if (std::holds_alternative<std::decay_t<decltype(_match_53)>::Extern>(_match_53._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_53)>::Extern>(_match_53._data);
                    auto& header = _v.header;
                    auto& link_lib = _v.link_lib;
                    auto& types = _v.types;
                    auto& functions = _v.functions;
                    (*this).emit_stmt(stmt, false);
                    this->output = std::string("");
                }
                else {
                    /* pass */
                }
            }
        }
        this->declarations = std::string("#include \"lavina.h\"\n");
        for (const auto& inc : this->extern_includes) {
            this->declarations = (this->declarations + ((std::string("") + (inc)) + std::string("\n")));
        }
        this->declarations = (this->declarations + std::string("\n"));
        for (int64_t mi = 0LL; mi < static_cast<int64_t>(this->module_stmts.size()); mi++) {
            (*this).emit_module(mi);
        }
        for (const auto& stmt : stmts) {
            {
                const auto& _match_54 = stmt;
                if (std::holds_alternative<std::decay_t<decltype(_match_54)>::Function>(_match_54._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_54)>::Function>(_match_54._data);
                    auto& name = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& body = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& type_params = _v.type_params;
                    if ((name.lexeme == std::string("main"))) {
                        this->has_main = true;
                        this->output = (this->output + std::string("int main(int argc, char* argv[]) {\n"));
                        this->output = (this->output + std::string("    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);\n"));
                        this->indent_level = 1LL;
                        for (const auto& s : body) {
                            (*this).emit_stmt(s, false);
                        }
                        this->indent_level = 0LL;
                        this->output = (this->output + std::string("}\n"));
                        this->declarations = (this->declarations + this->output);
                        this->output = std::string("");
                    }
                    else {
                        (*this).emit_stmt(stmt, false);
                        this->declarations = (this->declarations + this->output);
                        this->output = std::string("");
                    }
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_54)>::Extern>(_match_54._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_54)>::Extern>(_match_54._data);
                    auto& header = _v.header;
                    auto& link_lib = _v.link_lib;
                    auto& types = _v.types;
                    auto& functions = _v.functions;
                    /* pass */
                }
                else {
                    (*this).emit_stmt(stmt, false);
                    this->declarations = (this->declarations + this->output);
                    this->output = std::string("");
                }
            }
        }
        return this->declarations;
    }

};

struct Symbol {
    std::string name;
    TypeNode sym_type;
    std::string kind;
    bool is_ref;
    bool is_mutable;

};

struct Scope {
    std::vector<std::string> names;
    std::vector<Symbol> symbols;

};

struct Checker {
    std::vector<Scope> scopes;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::unordered_map<std::string, std::vector<EnumVariantNode>> known_enums;
    std::unordered_map<std::string, ExternFn> known_funcs;
    std::unordered_map<std::string, std::vector<Stmt>> known_classes;
    TypeNode current_return_type;
    bool in_class;
    std::string current_class_name;

    Checker() {
        this->scopes = {};
        this->errors = {};
        this->warnings = {};
        this->known_enums = {{}};
        this->known_funcs = {{}};
        this->known_classes = {{}};
        this->current_return_type = TypeNode::make_None();
        this->in_class = false;
        this->current_class_name = std::string("");
    }

    void error(const std::string& msg, const Token& t) {
        this->errors.push_back(((((((std::string("[") + (t.line)) + std::string(":")) + (t.col)) + std::string("] Error: ")) + (msg)) + std::string("")));
    }

    void error_msg(const std::string& msg) {
        this->errors.push_back(((std::string("Error: ") + (msg)) + std::string("")));
    }

    void warn(const std::string& msg, const Token& t) {
        this->warnings.push_back(((((((std::string("[") + (t.line)) + std::string(":")) + (t.col)) + std::string("] Warning: ")) + (msg)) + std::string("")));
    }

    void push_scope() {
        std::vector<std::string> names = {};
        std::vector<Symbol> syms = {};
        this->scopes.push_back(Scope(names, syms));
    }

    void pop_scope() {
        lv_pop(this->scopes);
    }

    void declare(const std::string& name, const TypeNode& t, const std::string& kind, bool is_ref, bool is_mutable, const Token& tok) {
        if ((static_cast<int64_t>(this->scopes.size()) == 0LL)) {
            return;
        }
        auto& scope = this->scopes[(static_cast<int64_t>(this->scopes.size()) - 1LL)];
        for (const auto& n : scope.names) {
            if ((n == name)) {
                (*this).warn(((std::string("Redefinition of '") + (name)) + std::string("' in same scope")), tok);
                return;
            }
        }
        scope.names.push_back(name);
        scope.symbols.push_back(Symbol(name, t, kind, is_ref, is_mutable));
    }

    bool resolve(const std::string& name) {
        int64_t i = (static_cast<int64_t>(this->scopes.size()) - 1LL);
        while ((i >= 0LL)) {
            auto& scope = this->scopes[i];
            for (const auto& n : scope.names) {
                if ((n == name)) {
                    return true;
                }
            }
            i = (i - 1LL);
        }
        if ((this->known_funcs.count(name) > 0)) {
            return true;
        }
        if ((this->known_classes.count(name) > 0)) {
            return true;
        }
        if ((this->known_enums.count(name) > 0)) {
            return true;
        }
        int64_t angle = lv_index_of(name, std::string("<"));
        if ((angle >= 0LL)) {
            std::string base = name.substr(0LL, (angle) - (0LL));
            return (*this).resolve(base);
        }
        return false;
    }

    Symbol lookup(const std::string& name) {
        int64_t i = (static_cast<int64_t>(this->scopes.size()) - 1LL);
        while ((i >= 0LL)) {
            auto& scope = this->scopes[i];
            for (int64_t j = 0LL; j < static_cast<int64_t>(scope.names.size()); j++) {
                if ((scope.names[j] == name)) {
                    return scope.symbols[j];
                }
            }
            i = (i - 1LL);
        }
        return Symbol(name, TypeNode::make_Auto(), std::string("unknown"), false, false);
    }

    bool types_compatible(const TypeNode& expected, const TypeNode& actual) {
        {
            const auto& _match_55 = actual;
            if (std::holds_alternative<std::decay_t<decltype(_match_55)>::Array>(_match_55._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_55)>::Array>(_match_55._data);
                auto& a_inner = *_v.inner;
                {
                    const auto& _match_56 = a_inner;
                    if (std::holds_alternative<std::decay_t<decltype(_match_56)>::Auto>(_match_56._data)) {
                        {
                            const auto& _match_57 = expected;
                            if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Array>(_match_57._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Array>(_match_57._data);
                                auto& e_inner = *_v.inner;
                                return true;
                            }
                            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::HashSet>(_match_57._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_57)>::HashSet>(_match_57._data);
                                auto& e_inner = *_v.inner;
                                return true;
                            }
                            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::HashMap>(_match_57._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_57)>::HashMap>(_match_57._data);
                                auto& ek = *_v.key_type;
                                auto& ev = *_v.value_type;
                                return true;
                            }
                            else {
                                /* pass */
                            }
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_58 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_58)>::Array>(_match_58._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_58)>::Array>(_match_58._data);
                auto& e_inner = *_v.inner;
                {
                    const auto& _match_59 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_59)>::Array>(_match_59._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_59)>::Array>(_match_59._data);
                        auto& a_inner = *_v.inner;
                        return (*this).types_compatible(e_inner, a_inner);
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_60 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_60)>::Auto>(_match_60._data)) {
                return true;
            }
            else if (_match_60._tag == "None") {
                return true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_60)>::Dynamic>(_match_60._data)) {
                return true;
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_61 = actual;
            if (std::holds_alternative<std::decay_t<decltype(_match_61)>::Auto>(_match_61._data)) {
                return true;
            }
            else if (_match_61._tag == "None") {
                return true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_61)>::Dynamic>(_match_61._data)) {
                return true;
            }
            else {
                /* pass */
            }
        }
        std::string e = (*this).type_name(expected);
        std::string a = (*this).type_name(actual);
        if ((e == a)) {
            return true;
        }
        bool e_is_int = false;
        bool a_is_int = false;
        {
            const auto& _match_62 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_62)>::Int>(_match_62._data)) {
                e_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_62)>::Int8>(_match_62._data)) {
                e_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_62)>::Int16>(_match_62._data)) {
                e_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_62)>::Int32>(_match_62._data)) {
                e_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_62)>::USize>(_match_62._data)) {
                e_is_int = true;
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_63 = actual;
            if (std::holds_alternative<std::decay_t<decltype(_match_63)>::Int>(_match_63._data)) {
                a_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_63)>::Int8>(_match_63._data)) {
                a_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_63)>::Int16>(_match_63._data)) {
                a_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_63)>::Int32>(_match_63._data)) {
                a_is_int = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_63)>::USize>(_match_63._data)) {
                a_is_int = true;
            }
            else {
                /* pass */
            }
        }
        if (e_is_int && a_is_int) {
            return true;
        }
        bool e_is_float = false;
        bool a_is_float = false;
        {
            const auto& _match_64 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_64)>::Float>(_match_64._data)) {
                e_is_float = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_64)>::Float32>(_match_64._data)) {
                e_is_float = true;
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_65 = actual;
            if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Float>(_match_65._data)) {
                a_is_float = true;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Float32>(_match_65._data)) {
                a_is_float = true;
            }
            else {
                /* pass */
            }
        }
        if (e_is_float && a_is_float) {
            return true;
        }
        if (e_is_float && a_is_int) {
            return true;
        }
        {
            const auto& _match_66 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_66)>::CString>(_match_66._data)) {
                {
                    const auto& _match_67 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_67)>::Str>(_match_67._data)) {
                        return true;
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_66)>::Str>(_match_66._data)) {
                {
                    const auto& _match_68 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_68)>::CString>(_match_68._data)) {
                        return true;
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_69 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_69)>::Nullable>(_match_69._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_69)>::Nullable>(_match_69._data);
                auto& inner = *_v.inner;
                {
                    const auto& _match_70 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_70)>::NullType>(_match_70._data)) {
                        return true;
                    }
                    else {
                        return (*this).types_compatible(inner, actual);
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_71 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_71)>::Ptr>(_match_71._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_71)>::Ptr>(_match_71._data);
                auto& inner = *_v.inner;
                {
                    const auto& _match_72 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_72)>::NullType>(_match_72._data)) {
                        return true;
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        return false;
    }

    std::string type_name(const TypeNode& t) {
        {
            const auto& _match_73 = t;
            if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Int>(_match_73._data)) {
                return std::string("int");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Float>(_match_73._data)) {
                return std::string("float");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Str>(_match_73._data)) {
                return std::string("string");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Bool>(_match_73._data)) {
                return std::string("bool");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Void>(_match_73._data)) {
                return std::string("void");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Auto>(_match_73._data)) {
                return std::string("auto");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Dynamic>(_match_73._data)) {
                return std::string("dynamic");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::NullType>(_match_73._data)) {
                return std::string("null");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Custom>(_match_73._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_73)>::Custom>(_match_73._data);
                auto& name = _v.name;
                auto& _ = _v.type_args;
                return name;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Array>(_match_73._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_73)>::Array>(_match_73._data);
                auto& inner = *_v.inner;
                return ((std::string("vector[") + ((*this).type_name(inner))) + std::string("]"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::HashSet>(_match_73._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_73)>::HashSet>(_match_73._data);
                auto& inner = *_v.inner;
                return ((std::string("set[") + ((*this).type_name(inner))) + std::string("]"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::HashMap>(_match_73._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_73)>::HashMap>(_match_73._data);
                auto& k = *_v.key_type;
                auto& v = *_v.value_type;
                return ((((std::string("map[") + ((*this).type_name(k))) + std::string(", ")) + ((*this).type_name(v))) + std::string("]"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Nullable>(_match_73._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_73)>::Nullable>(_match_73._data);
                auto& inner = *_v.inner;
                return ((std::string("") + ((*this).type_name(inner))) + std::string("?"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Int8>(_match_73._data)) {
                return std::string("int8");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Int16>(_match_73._data)) {
                return std::string("int16");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Int32>(_match_73._data)) {
                return std::string("int32");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Float32>(_match_73._data)) {
                return std::string("float32");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::USize>(_match_73._data)) {
                return std::string("usize");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::CString>(_match_73._data)) {
                return std::string("cstring");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Ptr>(_match_73._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_73)>::Ptr>(_match_73._data);
                auto& inner = *_v.inner;
                return ((std::string("ptr[") + ((*this).type_name(inner))) + std::string("]"));
            }
            else {
                return std::string("unknown");
            }
        }
    }

    TypeNode infer_type(const Expr& e) {
        {
            const auto& _match_74 = e;
            if (_match_74._tag == "None") {
                return TypeNode::make_None();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Literal>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Literal>(_match_74._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("int"))) {
                    return TypeNode::make_Int();
                }
                else {
                    if ((kind == std::string("float"))) {
                        return TypeNode::make_Float();
                    }
                    else {
                        if ((kind == std::string("string"))) {
                            return TypeNode::make_Str();
                        }
                        else {
                            if ((kind == std::string("bool"))) {
                                return TypeNode::make_Bool();
                            }
                            else {
                                if ((kind == std::string("null"))) {
                                    return TypeNode::make_NullType();
                                }
                            }
                        }
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Variable>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Variable>(_match_74._data);
                auto& name = _v.name;
                auto sym = (*this).lookup(name.lexeme);
                return sym.sym_type;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Binary>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Binary>(_match_74._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                auto lt = (*this).infer_type(left);
                auto rt = (*this).infer_type(right);
                if ((op.token_type == TK_EQUAL_EQUAL) || (op.token_type == TK_BANG_EQUAL) || (op.token_type == TK_LESS) || (op.token_type == TK_LESS_EQUAL) || (op.token_type == TK_GREATER) || (op.token_type == TK_GREATER_EQUAL)) {
                    return TypeNode::make_Bool();
                }
                {
                    const auto& _match_75 = lt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_75)>::Str>(_match_75._data)) {
                        return TypeNode::make_Str();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_76 = lt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_76)>::Float>(_match_76._data)) {
                        return TypeNode::make_Float();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_77 = rt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_77)>::Float>(_match_77._data)) {
                        return TypeNode::make_Float();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_78 = lt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_78)>::Int>(_match_78._data)) {
                        return TypeNode::make_Int();
                    }
                    else {
                        /* pass */
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Unary>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Unary>(_match_74._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                if ((op.token_type == TK_BANG) || (op.token_type == TK_NOT)) {
                    return TypeNode::make_Bool();
                }
                return (*this).infer_type(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Logical>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Logical>(_match_74._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                return TypeNode::make_Bool();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Call>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Call>(_match_74._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                return (*this).infer_call_type(callee);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Grouping>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Grouping>(_match_74._data);
                auto& inner = *_v.inner;
                return (*this).infer_type(inner);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Index>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Index>(_match_74._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                auto ot = (*this).infer_type(object);
                {
                    const auto& _match_79 = ot;
                    if (std::holds_alternative<std::decay_t<decltype(_match_79)>::Array>(_match_79._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_79)>::Array>(_match_79._data);
                        auto& inner = *_v.inner;
                        return inner;
                    }
                    else {
                        return TypeNode::make_Auto();
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Vector>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Vector>(_match_74._data);
                auto& elements = _v.elements;
                if ((static_cast<int64_t>(elements.size()) > 0LL)) {
                    auto inner = (*this).infer_type(elements[0LL]);
                    return TypeNode::make_Array(inner);
                }
                return TypeNode::make_Array(TypeNode::make_Auto());
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Cast>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Cast>(_match_74._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                return target_type;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::This>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::This>(_match_74._data);
                auto& kw = _v.keyword;
                return TypeNode::make_Custom(this->current_class_name, {});
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Own>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::Own>(_match_74._data);
                auto& expr = *_v.expr;
                return (*this).infer_type(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_74)>::AddressOf>(_match_74._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_74)>::AddressOf>(_match_74._data);
                auto& expr = *_v.expr;
                return TypeNode::make_Ptr((*this).infer_type(expr));
            }
            else {
                return TypeNode::make_Auto();
            }
        }
    }

    TypeNode infer_call_type(const Expr& callee) {
        {
            const auto& _match_80 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_80)>::Variable>(_match_80._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_80)>::Variable>(_match_80._data);
                auto& name = _v.name;
                if ((this->known_funcs.count(name.lexeme) > 0)) {
                    ExternFn fi = this->known_funcs[name.lexeme];
                    return fi.return_type;
                }
                if ((this->known_classes.count(name.lexeme) > 0)) {
                    return TypeNode::make_Custom(name.lexeme, {});
                }
                if ((this->known_enums.count(name.lexeme) > 0)) {
                    return TypeNode::make_Custom(name.lexeme, {});
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_80)>::Get>(_match_80._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_80)>::Get>(_match_80._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_80)>::StaticGet>(_match_80._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_80)>::StaticGet>(_match_80._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                {
                    const auto& _match_81 = object;
                    if (std::holds_alternative<std::decay_t<decltype(_match_81)>::Variable>(_match_81._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_81)>::Variable>(_match_81._data);
                        auto& obj_name = _v.name;
                        if ((this->known_enums.count(obj_name.lexeme) > 0)) {
                            return TypeNode::make_Custom(obj_name.lexeme, {});
                        }
                    }
                    else {
                        /* pass */
                    }
                }
                return TypeNode::make_Auto();
            }
            else {
                return TypeNode::make_Auto();
            }
        }
    }

    std::string find_enum_for_variant(const std::string& variant_name) {
        std::vector<std::string> keys = lv_keys(this->known_enums);
        for (const auto& key : keys) {
            std::vector<EnumVariantNode> variants = this->known_enums[key];
            for (const auto& v : variants) {
                if ((v.name.lexeme == variant_name)) {
                    return key;
                }
            }
        }
        return std::string("");
    }

    void register_builtins() {
        std::vector<std::string> builtins = std::vector{std::string("print"), std::string("println"), std::string("lv_assert"), std::string("fs_read"), std::string("fs_write"), std::string("fs_exists"), std::string("fs_append"), std::string("fs_read_lines"), std::string("fs_remove"), std::string("os_exec"), std::string("os_args"), std::string("os_env"), std::string("os_clock"), std::string("os_sleep"), std::string("os_cwd"), std::string("to_string"), std::string("to_int"), std::string("to_float"), std::string("input"), std::string("typeof"), std::string("len"), std::string("exit"), std::string("abs"), std::string("str_to_int"), std::string("str_to_float"), std::string("int_to_str"), std::string("float_to_str"), std::string("lv_abs"), std::string("lv_fabs"), std::string("lv_min"), std::string("lv_max"), std::string("lv_fmin"), std::string("lv_fmax"), std::string("lv_clamp"), std::string("lv_fclamp"), std::string("lv_floor"), std::string("lv_ceil"), std::string("lv_round"), std::string("lv_sqrt"), std::string("lv_pow"), std::string("lv_log"), std::string("lv_log2"), std::string("lv_sin"), std::string("lv_cos"), std::string("lv_random"), std::string("lv_random_float"), std::string("lv_count")};
        std::vector<Param> empty_params = {};
        for (const auto& name : builtins) {
            this->known_funcs[name] = ExternFn(name, name, TypeNode::make_Auto(), empty_params);
        }
        /* pass */
    }

    void check(const std::vector<Stmt>& stmts) {
        (*this).register_builtins();
        (*this).push_scope();
        for (const auto& s : stmts) {
            (*this).collect_decl(s);
        }
        for (const auto& s : stmts) {
            (*this).check_stmt(s);
        }
        (*this).pop_scope();
    }

    void collect_decl(const Stmt& s) {
        {
            const auto& _match_82 = s;
            if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Function>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Function>(_match_82._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                this->known_funcs[name.lexeme] = ExternFn(name.lexeme, name.lexeme, return_type, params);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Class>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Class>(_match_82._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                this->known_classes[name.lexeme] = body;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Enum>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Enum>(_match_82._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& enum_tp = _v.type_params;
                this->known_enums[name.lexeme] = variants;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Const>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Const>(_match_82._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                (*this).declare(name.lexeme, const_type, std::string("const"), false, false, name);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Struct>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Struct>(_match_82._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& struct_tp = _v.type_params;
                this->known_classes[name.lexeme] = body;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Namespace>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Namespace>(_match_82._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                for (const auto& ns_stmt : body) {
                    (*this).collect_decl(ns_stmt);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Extern>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Extern>(_match_82._data);
                auto& header = _v.header;
                auto& link_lib = _v.link_lib;
                auto& types = _v.types;
                auto& functions = _v.functions;
                for (const auto& et : types) {
                    std::vector<Stmt> empty_body = {};
                    this->known_classes[et.lavina_name] = empty_body;
                }
                for (const auto& ef : functions) {
                    this->known_funcs[ef.name] = ef;
                }
            }
            else {
                /* pass */
            }
        }
    }

    void check_stmt(const Stmt& s) {
        {
            const auto& _match_83 = s;
            if (std::holds_alternative<std::decay_t<decltype(_match_83)>::ExprStmt>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::ExprStmt>(_match_83._data);
                auto& expr = _v.expr;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Let>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Let>(_match_83._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                (*this).check_expr(initializer);
                auto init_type = (*this).infer_type(initializer);
                {
                    const auto& _match_84 = var_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Auto>(_match_84._data)) {
                        /* pass */
                    }
                    else if (_match_84._tag == "None") {
                        /* pass */
                    }
                    else {
                        if ((!(*this).types_compatible(var_type, init_type))) {
                            (*this).error(((((std::string("Cannot assign ") + ((*this).type_name(init_type))) + std::string(" to ")) + ((*this).type_name(var_type))) + std::string("")), name);
                        }
                    }
                }
                (*this).declare(name.lexeme, var_type, std::string("var"), is_ref, true, name);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Const>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Const>(_match_83._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                (*this).check_expr(value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Return>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Return>(_match_83._data);
                auto& keyword = _v.keyword;
                auto& value = _v.value;
                (*this).check_expr(value);
                {
                    const auto& _match_85 = this->current_return_type;
                    if (_match_85._tag == "None") {
                        /* pass */
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_85)>::Void>(_match_85._data)) {
                        {
                            const auto& _match_86 = value;
                            if (_match_86._tag == "None") {
                                /* pass */
                            }
                            else {
                                (*this).error(std::string("Cannot return a value from void function"), keyword);
                            }
                        }
                    }
                    else {
                        auto val_type = (*this).infer_type(value);
                        if ((!(*this).types_compatible(this->current_return_type, val_type))) {
                            (*this).error(((((std::string("Return type mismatch: expected ") + ((*this).type_name(this->current_return_type))) + std::string(", got ")) + ((*this).type_name(val_type))) + std::string("")), keyword);
                        }
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::If>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::If>(_match_83._data);
                auto& condition = _v.condition;
                auto& then_branch = *_v.then_branch;
                auto& else_branch = *_v.else_branch;
                (*this).check_expr(condition);
                (*this).check_stmt(then_branch);
                (*this).check_stmt(else_branch);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::While>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::While>(_match_83._data);
                auto& condition = _v.condition;
                auto& body = *_v.body;
                (*this).check_expr(condition);
                (*this).check_stmt(body);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::For>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::For>(_match_83._data);
                auto& item_name = _v.item_name;
                auto& collection = _v.collection;
                auto& body = *_v.body;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                (*this).check_expr(collection);
                (*this).push_scope();
                auto coll_type = (*this).infer_type(collection);
                auto item_type = TypeNode::make_Auto();
                {
                    const auto& _match_87 = coll_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_87)>::Array>(_match_87._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_87)>::Array>(_match_87._data);
                        auto& inner = *_v.inner;
                        item_type = inner;
                    }
                    else {
                        /* pass */
                    }
                }
                (*this).declare(item_name.lexeme, item_type, std::string("var"), is_ref, is_mut || (!is_ref), item_name);
                (*this).check_stmt(body);
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Block>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Block>(_match_83._data);
                auto& statements = _v.statements;
                (*this).push_scope();
                for (const auto& st : statements) {
                    (*this).check_stmt(st);
                }
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Function>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Function>(_match_83._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                (*this).check_function(name, params, return_type, body);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Class>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Class>(_match_83._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                (*this).check_class(name, body);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Struct>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Struct>(_match_83._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& struct_tp = _v.type_params;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Enum>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Enum>(_match_83._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& enum_tp = _v.type_params;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Match>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Match>(_match_83._data);
                auto& expr = _v.expr;
                auto& arm_patterns = _v.arm_patterns;
                auto& arm_bodies = _v.arm_bodies;
                (*this).check_expr(expr);
                (*this).check_match(expr, arm_patterns, arm_bodies);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Try>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Try>(_match_83._data);
                auto& try_body = *_v.try_body;
                auto& catch_body = *_v.catch_body;
                auto& exception_name = _v.exception_name;
                (*this).push_scope();
                (*this).check_stmt(try_body);
                (*this).pop_scope();
                (*this).push_scope();
                if ((exception_name != std::string(""))) {
                    (*this).declare(exception_name, TypeNode::make_Auto(), std::string("var"), false, false, Token(TK_IDENTIFIER, exception_name, 0LL, 0LL));
                }
                (*this).check_stmt(catch_body);
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Namespace>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Namespace>(_match_83._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                (*this).push_scope();
                for (const auto& ns_stmt : body) {
                    (*this).collect_decl(ns_stmt);
                }
                for (const auto& ns_stmt : body) {
                    (*this).check_stmt(ns_stmt);
                }
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Import>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Import>(_match_83._data);
                auto& path = _v.path;
                auto& alias = _v.alias;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Break>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Break>(_match_83._data);
                auto& keyword = _v.keyword;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Continue>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Continue>(_match_83._data);
                auto& keyword = _v.keyword;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Pass>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Pass>(_match_83._data);
                auto& keyword = _v.keyword;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::CppBlock>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::CppBlock>(_match_83._data);
                auto& code = _v.code;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_83)>::Extern>(_match_83._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_83)>::Extern>(_match_83._data);
                auto& header = _v.header;
                auto& link_lib = _v.link_lib;
                auto& types = _v.types;
                auto& functions = _v.functions;
                /* pass */
            }
            else {
                /* pass */
            }
        }
    }

    void check_function(const Token& name, const std::vector<Param>& params, const TypeNode& return_type, const std::vector<Stmt>& body) {
        auto saved_return = this->current_return_type;
        this->current_return_type = return_type;
        (*this).push_scope();
        for (const auto& p : params) {
            (*this).declare(p.name.lexeme, p.param_type, std::string("param"), p.is_ref, true, p.name);
        }
        for (const auto& s : body) {
            (*this).check_stmt(s);
        }
        (*this).pop_scope();
        this->current_return_type = saved_return;
    }

    void check_class(const Token& name, const std::vector<Stmt>& body) {
        bool saved_in_class = this->in_class;
        std::string saved_class_name = this->current_class_name;
        this->in_class = true;
        this->current_class_name = name.lexeme;
        (*this).push_scope();
        (*this).declare(std::string("this"), TypeNode::make_Custom(name.lexeme, {}), std::string("var"), false, false, name);
        for (const auto& s : body) {
            {
                const auto& _match_88 = s;
                if (std::holds_alternative<std::decay_t<decltype(_match_88)>::Function>(_match_88._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_88)>::Function>(_match_88._data);
                    auto& fname = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& fbody = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& fn_tp = _v.type_params;
                    (*this).check_function(fname, params, return_type, fbody);
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_88)>::Let>(_match_88._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_88)>::Let>(_match_88._data);
                    auto& lname = _v.name;
                    auto& var_type = _v.var_type;
                    auto& initializer = _v.initializer;
                    auto& visibility = _v.visibility;
                    auto& is_ref = _v.is_ref;
                    auto& is_mut = _v.is_mut;
                    (*this).check_expr(initializer);
                }
                else {
                    (*this).check_stmt(s);
                }
            }
        }
        (*this).pop_scope();
        this->in_class = saved_in_class;
        this->current_class_name = saved_class_name;
    }

    void check_expr(const Expr& e) {
        {
            const auto& _match_89 = e;
            if (_match_89._tag == "None") {
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Variable>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Variable>(_match_89._data);
                auto& name = _v.name;
                if ((!(*this).resolve(name.lexeme))) {
                    (*this).error(((std::string("Undefined variable '") + (name.lexeme)) + std::string("'")), name);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Binary>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Binary>(_match_89._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                (*this).check_expr(left);
                (*this).check_expr(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Unary>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Unary>(_match_89._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                (*this).check_expr(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Logical>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Logical>(_match_89._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                (*this).check_expr(left);
                (*this).check_expr(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Grouping>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Grouping>(_match_89._data);
                auto& inner = *_v.inner;
                (*this).check_expr(inner);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Call>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Call>(_match_89._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                (*this).check_expr(callee);
                for (const auto& a : args) {
                    (*this).check_expr(a);
                }
                (*this).check_call_args(callee, args, paren);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Assign>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Assign>(_match_89._data);
                auto& name = _v.name;
                auto& value = *_v.value;
                (*this).check_expr(value);
                if ((!(*this).resolve(name.lexeme))) {
                    (*this).error(((std::string("Undefined variable '") + (name.lexeme)) + std::string("'")), name);
                }
                else {
                    auto sym = (*this).lookup(name.lexeme);
                    if ((sym.kind == std::string("const"))) {
                        (*this).error(((std::string("Cannot assign to constant '") + (name.lexeme)) + std::string("'")), name);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Index>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Index>(_match_89._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                (*this).check_expr(object);
                (*this).check_expr(index);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::IndexSet>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::IndexSet>(_match_89._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                auto& value = *_v.value;
                (*this).check_expr(object);
                (*this).check_expr(index);
                (*this).check_expr(value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Vector>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Vector>(_match_89._data);
                auto& elements = _v.elements;
                for (const auto& el : elements) {
                    (*this).check_expr(el);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Map>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Map>(_match_89._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                for (const auto& k : keys) {
                    (*this).check_expr(k);
                }
                for (const auto& v : values) {
                    (*this).check_expr(v);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Get>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Get>(_match_89._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                (*this).check_expr(object);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Set>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Set>(_match_89._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                auto& value = *_v.value;
                (*this).check_expr(object);
                (*this).check_expr(value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::StaticGet>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::StaticGet>(_match_89._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                {
                    const auto& _match_90 = object;
                    if (std::holds_alternative<std::decay_t<decltype(_match_90)>::Variable>(_match_90._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_90)>::Variable>(_match_90._data);
                        auto& tok = _v.name;
                        /* pass */
                    }
                    else {
                        (*this).check_expr(object);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Cast>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Cast>(_match_89._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Throw>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Throw>(_match_89._data);
                auto& expr = *_v.expr;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Range>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Range>(_match_89._data);
                auto& start = *_v.start;
                auto& end = *_v.end;
                (*this).check_expr(start);
                (*this).check_expr(end);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Lambda>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Lambda>(_match_89._data);
                auto& params = _v.params;
                auto& body = *_v.body;
                (*this).push_scope();
                for (const auto& p : params) {
                    (*this).declare(p.name.lexeme, p.param_type, std::string("param"), p.is_ref, true, p.name);
                }
                (*this).check_expr(body);
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::BlockLambda>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::BlockLambda>(_match_89._data);
                auto& params = _v.params;
                auto& body_id = _v.body_id;
                (*this).push_scope();
                for (const auto& p : params) {
                    (*this).declare(p.name.lexeme, p.param_type, std::string("param"), p.is_ref, true, p.name);
                }
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Own>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::Own>(_match_89._data);
                auto& expr = *_v.expr;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_89)>::AddressOf>(_match_89._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_89)>::AddressOf>(_match_89._data);
                auto& expr = *_v.expr;
                (*this).check_expr(expr);
            }
            else {
                /* pass */
            }
        }
    }

    void check_call_args(const Expr& callee, const std::vector<Expr>& args, const Token& paren) {
        {
            const auto& _match_91 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_91)>::Variable>(_match_91._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_91)>::Variable>(_match_91._data);
                auto& name = _v.name;
                if ((this->known_funcs.count(name.lexeme) > 0)) {
                    ExternFn fi = this->known_funcs[name.lexeme];
                    if ((static_cast<int64_t>(fi.params.size()) > 0LL) && (static_cast<int64_t>(args.size()) != static_cast<int64_t>(fi.params.size()))) {
                        (*this).error(((((((std::string("Function '") + (name.lexeme)) + std::string("' expects ")) + (static_cast<int64_t>(fi.params.size()))) + std::string(" args, got ")) + (static_cast<int64_t>(args.size()))) + std::string("")), paren);
                    }
                    return;
                }
            }
            else {
                /* pass */
            }
        }
    }

    void check_match(const Expr& expr, const std::vector<MatchArm>& arm_patterns, const std::vector<Stmt>& arm_bodies) {
        bool has_wildcard = false;
        for (int64_t i = 0LL; i < static_cast<int64_t>(arm_patterns.size()); i++) {
            auto arm = arm_patterns[i];
            if ((arm.pattern_name == std::string("_"))) {
                has_wildcard = true;
            }
            else {
                std::string enum_name = (*this).find_enum_for_variant(arm.pattern_name);
                if ((enum_name == std::string(""))) {
                    /* pass */
                }
            }
            (*this).push_scope();
            for (const auto& b : arm.bindings) {
                (*this).declare(b, TypeNode::make_Auto(), std::string("var"), false, false, Token(TK_IDENTIFIER, b, 0LL, 0LL));
            }
            (*this).check_stmt(arm_bodies[i]);
            (*this).pop_scope();
        }
    }

};

std::vector<std::string> run_checker(const std::vector<Stmt>& stmts) {
    auto checker = Checker();
    checker.check(stmts);
    for (const auto& w : checker.warnings) {
        print(w);
    }
    return checker.errors;
}

struct Parser {
    std::vector<Token> tokens;
    int64_t current;
    bool in_class_body;
    std::vector<std::vector<Stmt>> lambda_blocks;

    Parser(std::vector<Token> tokens)
        : tokens(tokens) {
        this->current = 0LL;
        this->in_class_body = false;
        this->lambda_blocks = {};
    }

    bool is_at_end() {
        return ((*this).peek().token_type == TK_EOF);
    }

    Token peek() {
        return this->tokens[this->current];
    }

    Token peek_at(int64_t offset) {
        if (((this->current + offset) >= static_cast<int64_t>(this->tokens.size()))) {
            return this->tokens[(static_cast<int64_t>(this->tokens.size()) - 1LL)];
        }
        return this->tokens[(this->current + offset)];
    }

    Token previous() {
        return this->tokens[(this->current - 1LL)];
    }

    Token advance() {
        if ((!(*this).is_at_end())) {
            this->current = (this->current + 1LL);
        }
        return (*this).previous();
    }

    Token consume(std::string tk_type, std::string message) {
        if ((*this).check(tk_type)) {
            return (*this).advance();
        }
        auto t = (*this).peek();
        throw std::runtime_error(((((((((std::string("") + (message)) + std::string(" Got ")) + (t.token_type)) + std::string(" at ")) + (t.line)) + std::string(":")) + (t.col)) + std::string("")));
    }

    bool check(std::string tk_type) {
        if ((*this).is_at_end()) {
            return false;
        }
        return ((*this).peek().token_type == tk_type);
    }

    bool match_any(std::vector<std::string> types) {
        for (const auto& t : types) {
            if ((*this).check(t)) {
                (*this).advance();
                return true;
            }
        }
        return false;
    }

    void skip_formatting() {
        while ((*this).match_any(std::vector{TK_NEWLINE, TK_INDENT, TK_DEDENT})) {
            /* pass */
        }
    }

    bool is_type_at_pos(int64_t pos) {
        auto t = (*this).peek_at(pos).token_type;
        if ((t == TK_INT_TYPE)) {
            return true;
        }
        if ((t == TK_FLOAT_TYPE)) {
            return true;
        }
        if ((t == TK_STRING_TYPE)) {
            return true;
        }
        if ((t == TK_BOOL)) {
            return true;
        }
        if ((t == TK_VOID)) {
            return true;
        }
        if ((t == TK_AUTO)) {
            return true;
        }
        if ((t == TK_DYNAMIC)) {
            return true;
        }
        if ((t == TK_VECTOR)) {
            return true;
        }
        if ((t == TK_HASHMAP)) {
            return true;
        }
        if ((t == TK_HASHSET)) {
            return true;
        }
        if ((t == TK_IDENTIFIER)) {
            return true;
        }
        if ((t == TK_INT8)) {
            return true;
        }
        if ((t == TK_INT16)) {
            return true;
        }
        if ((t == TK_INT32)) {
            return true;
        }
        if ((t == TK_INT64)) {
            return true;
        }
        if ((t == TK_FLOAT32)) {
            return true;
        }
        if ((t == TK_FLOAT64)) {
            return true;
        }
        if ((t == TK_USIZE)) {
            return true;
        }
        if ((t == TK_CSTRING)) {
            return true;
        }
        if ((t == TK_PTR)) {
            return true;
        }
        return false;
    }

    int64_t skip_type_tokens(int64_t pos) {
        auto tt = (*this).peek_at(pos).token_type;
        if ((tt == TK_PTR)) {
            pos = (pos + 1LL);
            pos = (pos + 1LL);
            pos = (*this).skip_type_tokens(pos);
            pos = (pos + 1LL);
            return pos;
        }
        if ((tt == TK_VECTOR) || (tt == TK_HASHSET)) {
            pos = (pos + 1LL);
            pos = (pos + 1LL);
            pos = (*this).skip_type_tokens(pos);
            pos = (pos + 1LL);
            return pos;
        }
        else {
            if ((tt == TK_HASHMAP)) {
                pos = (pos + 1LL);
                pos = (pos + 1LL);
                pos = (*this).skip_type_tokens(pos);
                pos = (pos + 1LL);
                pos = (*this).skip_type_tokens(pos);
                pos = (pos + 1LL);
                return pos;
            }
        }
        if ((tt == TK_IDENTIFIER) && ((*this).peek_at((pos + 1LL)).token_type == TK_LEFT_BRACKET)) {
            int64_t try_pos = (pos + 2LL);
            try_pos = (*this).skip_type_tokens(try_pos);
            while (((*this).peek_at(try_pos).token_type == TK_COMMA)) {
                try_pos = (try_pos + 1LL);
                try_pos = (*this).skip_type_tokens(try_pos);
            }
            if (((*this).peek_at(try_pos).token_type == TK_RIGHT_BRACKET)) {
                return (try_pos + 1LL);
            }
        }
        return (pos + 1LL);
    }

    bool is_function_start() {
        int64_t offset = 0LL;
        if (((*this).peek_at(offset).token_type == TK_COMPTIME) || ((*this).peek_at(offset).token_type == TK_COMPTIME_STRICT)) {
            offset = (offset + 1LL);
        }
        if ((!(*this).is_type_at_pos(offset))) {
            return false;
        }
        offset = (*this).skip_type_tokens(offset);
        if (((*this).peek_at(offset).token_type == TK_QUESTION)) {
            offset = (offset + 1LL);
        }
        bool still_modifiers = true;
        while ((offset < static_cast<int64_t>(this->tokens.size())) && still_modifiers) {
            auto mt = (*this).peek_at(offset).token_type;
            if ((mt == TK_INLINE) || (mt == TK_COMPTIME)) {
                offset = (offset + 1LL);
            }
            else {
                still_modifiers = false;
            }
        }
        auto final_tt = (*this).peek_at(offset).token_type;
        return (final_tt == TK_FN) || (final_tt == TK_OPERATOR);
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
                                                    t = TypeNode::make_HashMap(k, v);
                                                }
                                                else {
                                                    if ((*this).match_any(std::vector{TK_INT8})) {
                                                        t = TypeNode::make_Int8();
                                                    }
                                                    else {
                                                        if ((*this).match_any(std::vector{TK_INT16})) {
                                                            t = TypeNode::make_Int16();
                                                        }
                                                        else {
                                                            if ((*this).match_any(std::vector{TK_INT32})) {
                                                                t = TypeNode::make_Int32();
                                                            }
                                                            else {
                                                                if ((*this).match_any(std::vector{TK_INT64})) {
                                                                    t = TypeNode::make_Int();
                                                                }
                                                                else {
                                                                    if ((*this).match_any(std::vector{TK_FLOAT32})) {
                                                                        t = TypeNode::make_Float32();
                                                                    }
                                                                    else {
                                                                        if ((*this).match_any(std::vector{TK_FLOAT64})) {
                                                                            t = TypeNode::make_Float();
                                                                        }
                                                                        else {
                                                                            if ((*this).match_any(std::vector{TK_USIZE})) {
                                                                                t = TypeNode::make_USize();
                                                                            }
                                                                            else {
                                                                                if ((*this).match_any(std::vector{TK_CSTRING})) {
                                                                                    t = TypeNode::make_CString();
                                                                                }
                                                                                else {
                                                                                    if ((*this).match_any(std::vector{TK_PTR})) {
                                                                                        (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'ptr'."));
                                                                                        TypeNode inner = (*this).parse_type();
                                                                                        (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after ptr type."));
                                                                                        t = TypeNode::make_Ptr(inner);
                                                                                    }
                                                                                    else {
                                                                                        if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
                                                                                            std::string custom_name = (*this).previous().lexeme;
                                                                                            std::vector<TypeNode> type_args = {};
                                                                                            if ((*this).check(TK_LEFT_BRACKET)) {
                                                                                                int64_t save_pos = this->current;
                                                                                                (*this).advance();
                                                                                                bool is_type_args = true;
                                                                                                try {
                                                                                                    TypeNode first_arg = (*this).parse_type();
                                                                                                    type_args.push_back(first_arg);
                                                                                                    while ((*this).match_any(std::vector{TK_COMMA})) {
                                                                                                        type_args.push_back((*this).parse_type());
                                                                                                    }
                                                                                                    if ((!(*this).check(TK_RIGHT_BRACKET))) {
                                                                                                        is_type_args = false;
                                                                                                    }
                                                                                                }
                                                                                                 catch (const std::exception& e) {
                                                                                                    is_type_args = false;
                                                                                                }
                                                                                                if (is_type_args) {
                                                                                                    (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after type arguments."));
                                                                                                }
                                                                                                else {
                                                                                                    type_args = {};
                                                                                                    this->current = save_pos;
                                                                                                }
                                                                                            }
                                                                                            t = TypeNode::make_Custom(custom_name, type_args);
                                                                                        }
                                                                                        else {
                                                                                            auto tok = (*this).peek();
                                                                                            throw std::runtime_error(((((((std::string("Expect type. Got ") + (tok.token_type)) + std::string(" at ")) + (tok.line)) + std::string(":")) + (tok.col)) + std::string("")));
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
        if ((*this).match_any(std::vector{TK_QUESTION})) {
            t = TypeNode::make_Nullable(t);
        }
        return t;
    }

    std::string type_to_string(const TypeNode& t) {
        {
            const auto& _match_92 = t;
            if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Int>(_match_92._data)) {
                return std::string("int64_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Float>(_match_92._data)) {
                return std::string("double");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Str>(_match_92._data)) {
                return std::string("std::string");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Bool>(_match_92._data)) {
                return std::string("bool");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Void>(_match_92._data)) {
                return std::string("void");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Auto>(_match_92._data)) {
                return std::string("auto");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Custom>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Custom>(_match_92._data);
                auto& name = _v.name;
                auto& type_args = _v.type_args;
                if ((static_cast<int64_t>(type_args.size()) > 0LL)) {
                    std::vector<std::string> ta = {};
                    for (const auto& a : type_args) {
                        ta.push_back((*this).type_to_string(a));
                    }
                    return ((((std::string("") + (name)) + std::string("<")) + (lv_join(ta, std::string(", ")))) + std::string(">"));
                }
                return name;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Array>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Array>(_match_92._data);
                auto& inner = *_v.inner;
                return ((std::string("std::vector<") + ((*this).type_to_string(inner))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Int8>(_match_92._data)) {
                return std::string("int8_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Int16>(_match_92._data)) {
                return std::string("int16_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Int32>(_match_92._data)) {
                return std::string("int32_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Float32>(_match_92._data)) {
                return std::string("float");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::USize>(_match_92._data)) {
                return std::string("size_t");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::CString>(_match_92._data)) {
                return std::string("const char*");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Ptr>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Ptr>(_match_92._data);
                auto& inner = *_v.inner;
                return ((std::string("") + ((*this).type_to_string(inner))) + std::string("*"));
            }
            else {
                return std::string("auto");
            }
        }
    }

    Expr expression() {
        return (*this).assignment();
    }

    Expr assignment() {
        Expr expr = (*this).logical_or();
        if ((*this).match_any(std::vector{TK_EQUAL})) {
            Expr value = (*this).assignment();
            {
                const auto& _match_93 = expr;
                if (std::holds_alternative<std::decay_t<decltype(_match_93)>::Variable>(_match_93._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_93)>::Variable>(_match_93._data);
                    auto& name = _v.name;
                    return Expr::make_Assign(name, value);
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_93)>::Get>(_match_93._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_93)>::Get>(_match_93._data);
                    auto& object = *_v.object;
                    auto& name = _v.name;
                    return Expr::make_Set(object, name, value);
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_93)>::Index>(_match_93._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_93)>::Index>(_match_93._data);
                    auto& object = *_v.object;
                    auto& bracket = _v.bracket;
                    auto& index = *_v.index;
                    return Expr::make_IndexSet(object, bracket, index, value);
                }
                else {
                    throw std::runtime_error(std::string("Invalid assignment target."));
                }
            }
        }
        else {
            if ((*this).match_any(std::vector{TK_PLUS_EQUAL, TK_MINUS_EQUAL, TK_STAR_EQUAL})) {
                auto compound_op = (*this).previous();
                Expr rhs = (*this).assignment();
                std::string base_type = TK_PLUS;
                std::string base_lexeme = std::string("+");
                if ((compound_op.token_type == TK_MINUS_EQUAL)) {
                    base_type = TK_MINUS;
                    base_lexeme = std::string("-");
                }
                else {
                    if ((compound_op.token_type == TK_STAR_EQUAL)) {
                        base_type = TK_STAR;
                        base_lexeme = std::string("*");
                    }
                }
                auto op_token = Token(base_type, base_lexeme, compound_op.line, compound_op.col);
                {
                    const auto& _match_94 = expr;
                    if (std::holds_alternative<std::decay_t<decltype(_match_94)>::Variable>(_match_94._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_94)>::Variable>(_match_94._data);
                        auto& name = _v.name;
                        Expr bin = Expr::make_Binary(Expr::make_Variable(name), op_token, rhs);
                        return Expr::make_Assign(name, bin);
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_94)>::Get>(_match_94._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_94)>::Get>(_match_94._data);
                        auto& object = *_v.object;
                        auto& name = _v.name;
                        Expr bin = Expr::make_Binary(Expr::make_Get(object, name), op_token, rhs);
                        return Expr::make_Set(object, name, bin);
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_94)>::Index>(_match_94._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_94)>::Index>(_match_94._data);
                        auto& object = *_v.object;
                        auto& bracket = _v.bracket;
                        auto& index = *_v.index;
                        Expr bin = Expr::make_Binary(Expr::make_Index(object, bracket, index), op_token, rhs);
                        return Expr::make_IndexSet(object, bracket, index, bin);
                    }
                    else {
                        throw std::runtime_error(std::string("Invalid compound assignment target."));
                    }
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
        if ((*this).match_any(std::vector{TK_OWN})) {
            Expr right = (*this).unary();
            return Expr::make_Own(right);
        }
        if ((*this).match_any(std::vector{TK_AMPERSAND})) {
            Expr right = (*this).unary();
            return Expr::make_AddressOf(right);
        }
        return (*this).call();
    }

    Expr call() {
        Expr expr = (*this).primary();
        {
            const auto& _match_95 = expr;
            if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Variable>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Variable>(_match_95._data);
                auto& tok = _v.name;
                if ((*this).check(TK_LEFT_BRACKET)) {
                    int64_t save_pos = this->current;
                    (*this).advance();
                    bool is_type_args = true;
                    std::vector<std::string> type_strs = {};
                    try {
                        TypeNode first_t = (*this).parse_type();
                        type_strs.push_back((*this).type_to_string(first_t));
                        while ((*this).match_any(std::vector{TK_COMMA})) {
                            TypeNode next_t = (*this).parse_type();
                            type_strs.push_back((*this).type_to_string(next_t));
                        }
                        if ((!(*this).check(TK_RIGHT_BRACKET))) {
                            is_type_args = false;
                        }
                    }
                     catch (const std::exception& e) {
                        is_type_args = false;
                    }
                    if (is_type_args) {
                        (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']'."));
                        if ((*this).check(TK_LEFT_PAREN) || (*this).check(TK_DOUBLE_COLON)) {
                            std::string new_name = ((((std::string("") + (tok.lexeme)) + std::string("<")) + (lv_join(type_strs, std::string(", ")))) + std::string(">"));
                            expr = Expr::make_Variable(Token(TK_IDENTIFIER, new_name, tok.line, tok.col));
                        }
                        else {
                            this->current = save_pos;
                        }
                    }
                    else {
                        this->current = save_pos;
                    }
                }
            }
            else {
                /* pass */
            }
        }
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

    Expr finish_call(const Expr& callee) {
        {
            const auto& _match_96 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_96)>::Variable>(_match_96._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_96)>::Variable>(_match_96._data);
                auto& name = _v.name;
                if ((name.lexeme == std::string("cast"))) {
                    Expr expr = (*this).expression();
                    (*this).consume(TK_COMMA, std::string("Expect ',' after value in cast()."));
                    TypeNode target = (*this).parse_type();
                    (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after type in cast()."));
                    return Expr::make_Cast(expr, target);
                }
            }
            else {
                /* pass */
            }
        }
        std::vector<Expr> args = {};
        if ((!(*this).check(TK_RIGHT_PAREN))) {
            (*this).match_any(std::vector{TK_REF, TK_REF_MUT});
            args.push_back((*this).expression());
            while ((*this).match_any(std::vector{TK_COMMA})) {
                (*this).match_any(std::vector{TK_REF, TK_REF_MUT});
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
            if ((*this).try_parse_lambda()) {
                return (*this).parse_lambda();
            }
            Expr expr = (*this).expression();
            (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after expression."));
            return Expr::make_Grouping(expr);
        }
        if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
            std::vector<Expr> elements = {};
            (*this).skip_formatting();
            if ((!(*this).check(TK_RIGHT_BRACKET))) {
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
            if ((!(*this).check(TK_RIGHT_BRACE))) {
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
        throw std::runtime_error(((((((std::string("Expect expression. Got ") + (t.token_type)) + std::string(" at ")) + (t.line)) + std::string(":")) + (t.col)) + std::string("")));
    }

    std::vector<Param> parse_param_list() {
        std::vector<Param> params = {};
        if ((!(*this).check(TK_RIGHT_PAREN))) {
            bool p_mut = (*this).match_any(std::vector{TK_REF_MUT});
            bool p_ref = p_mut || (*this).match_any(std::vector{TK_REF});
            TypeNode param_type = (*this).parse_type();
            auto param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
            params.push_back(Param(param_name, param_type, p_ref, p_mut));
            while ((*this).match_any(std::vector{TK_COMMA})) {
                p_mut = (*this).match_any(std::vector{TK_REF_MUT});
                p_ref = p_mut || (*this).match_any(std::vector{TK_REF});
                param_type = (*this).parse_type();
                param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
                params.push_back(Param(param_name, param_type, p_ref, p_mut));
            }
        }
        return params;
    }

    bool try_parse_lambda() {
        int64_t saved = this->current;
        if ((*this).check(TK_RIGHT_PAREN)) {
            std::string next_type = (*this).peek_at(1LL).token_type;
            if ((next_type == TK_FAT_ARROW) || (next_type == TK_COLON)) {
                return true;
            }
        }
        int64_t start_pos = 0LL;
        if (((*this).peek_at(0LL).token_type == TK_REF) || ((*this).peek_at(0LL).token_type == TK_REF_MUT)) {
            start_pos = 1LL;
        }
        if ((*this).is_type_at_pos(start_pos)) {
            int64_t pos = start_pos;
            bool valid = true;
            while (valid) {
                pos = (*this).skip_type_tokens(pos);
                if (((*this).peek_at(pos).token_type == TK_QUESTION)) {
                    pos = (pos + 1LL);
                }
                if (((*this).peek_at(pos).token_type != TK_IDENTIFIER)) {
                    valid = false;
                }
                else {
                    pos = (pos + 1LL);
                    if (((*this).peek_at(pos).token_type == TK_COMMA)) {
                        pos = (pos + 1LL);
                        if (((*this).peek_at(pos).token_type == TK_REF) || ((*this).peek_at(pos).token_type == TK_REF_MUT)) {
                            pos = (pos + 1LL);
                        }
                        if ((!(*this).is_type_at_pos(pos))) {
                            valid = false;
                        }
                    }
                    else {
                        if (((*this).peek_at(pos).token_type == TK_RIGHT_PAREN)) {
                            std::string after_paren = (*this).peek_at((pos + 1LL)).token_type;
                            if ((after_paren == TK_FAT_ARROW) || (after_paren == TK_COLON)) {
                                return true;
                            }
                            return false;
                        }
                        else {
                            valid = false;
                        }
                    }
                }
            }
        }
        return false;
    }

    Expr parse_lambda() {
        std::vector<Param> params = (*this).parse_param_list();
        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after lambda parameters."));
        if ((*this).match_any(std::vector{TK_FAT_ARROW})) {
            Expr body = (*this).expression();
            return Expr::make_Lambda(params, body);
        }
        (*this).consume(TK_COLON, std::string("Expect '=>' or ':' after lambda parameters."));
        std::vector<Stmt> body = (*this).block();
        int64_t body_id = static_cast<int64_t>(this->lambda_blocks.size());
        this->lambda_blocks.push_back(body);
        return Expr::make_BlockLambda(params, body_id);
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
        if ((*this).match_any(std::vector{TK_PASS})) {
            auto kw = (*this).previous();
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_Pass(kw);
        }
        if ((*this).match_any(std::vector{TK_CPP})) {
            auto code = (*this).previous().lexeme;
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_CppBlock(code);
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
        bool f_mut = (*this).match_any(std::vector{TK_REF_MUT});
        bool f_ref = f_mut || (*this).match_any(std::vector{TK_REF});
        auto item_name = (*this).consume(TK_IDENTIFIER, std::string("Expect variable name after 'for'."));
        (*this).consume(TK_IN, std::string("Expect 'in' after variable name."));
        Expr collection = (*this).expression();
        if ((*this).match_any(std::vector{TK_DOT_DOT})) {
            Expr end_expr = (*this).expression();
            collection = Expr::make_Range(collection, end_expr);
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after for loop header."));
        Stmt body = Stmt::make_Block((*this).block());
        return Stmt::make_For(item_name, collection, body, f_ref, f_mut);
    }

    Stmt return_statement() {
        auto keyword = (*this).previous();
        Expr value = Expr::make_None();
        if ((!(*this).check(TK_NEWLINE)) && (!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
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
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                auto pattern_tok = (*this).consume(TK_IDENTIFIER, std::string("Expect pattern name in match arm."));
                std::string pattern_name = pattern_tok.lexeme;
                std::vector<std::string> bindings = {};
                if ((pattern_name != std::string("_"))) {
                    if ((*this).match_any(std::vector{TK_LEFT_PAREN})) {
                        if ((!(*this).check(TK_RIGHT_PAREN))) {
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
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
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
        return (*this).var_declaration_with_ref(visibility, false, false);
    }

    Stmt var_declaration_with_ref(std::string visibility, bool is_ref, bool is_mut) {
        TypeNode var_type = (*this).parse_type();
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect variable name after type."));
        Expr initializer = Expr::make_None();
        if ((*this).match_any(std::vector{TK_EQUAL})) {
            initializer = (*this).expression();
        }
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Let(name, var_type, initializer, visibility, is_ref, is_mut);
    }

    Stmt const_declaration(std::string visibility) {
        TypeNode const_type = (*this).parse_type();
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect constant name after type."));
        (*this).consume(TK_EQUAL, std::string("Const declaration must have an initializer."));
        Expr value = (*this).expression();
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Const(name, const_type, value, visibility);
    }

    Stmt function_declaration(std::string visibility, bool is_static, int64_t comptime_mode) {
        TypeNode return_type = (*this).parse_type();
        bool is_inline = false;
        while ((*this).match_any(std::vector{TK_INLINE})) {
            is_inline = true;
        }
        Token name = Token(std::string(""), std::string(""), 0LL, 0LL);
        bool is_operator = false;
        if ((*this).match_any(std::vector{TK_OPERATOR})) {
            is_operator = true;
            auto op_tok = (*this).advance();
            name = Token(TK_IDENTIFIER, ((std::string("operator") + (op_tok.lexeme)) + std::string("")), op_tok.line, op_tok.col);
        }
        else {
            (*this).consume(TK_FN, std::string("Expect 'fn' keyword after return type."));
            name = (*this).consume(TK_IDENTIFIER, std::string("Expect function name."));
        }
        std::vector<std::string> type_params = {};
        if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
            type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            while ((*this).match_any(std::vector{TK_COMMA})) {
                type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            }
            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after type parameters."));
        }
        (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after function name."));
        std::vector<Param> params = (*this).parse_param_list();
        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after parameters."));
        (*this).consume(TK_COLON, std::string("Expect ':' before function body."));
        std::vector<Stmt> body = (*this).block();
        return Stmt::make_Function(name, params, return_type, body, is_inline, comptime_mode, is_static, visibility, type_params);
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
        std::vector<std::string> type_params = {};
        if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
            type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            while ((*this).match_any(std::vector{TK_COMMA})) {
                type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            }
            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after type parameters."));
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after struct name."));
        auto old = this->in_class_body;
        this->in_class_body = true;
        auto body = (*this).block();
        this->in_class_body = old;
        return Stmt::make_Struct(name, body, visibility, type_params);
    }

    Stmt enum_declaration(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect enum name."));
        std::vector<std::string> type_params = {};
        if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
            type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            while ((*this).match_any(std::vector{TK_COMMA})) {
                type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            }
            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after type parameters."));
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after enum name."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start enum body."));
        std::vector<EnumVariantNode> variants = {};
        std::vector<Stmt> methods = {};
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                if ((*this).is_function_start()) {
                    auto old = this->in_class_body;
                    this->in_class_body = true;
                    int64_t method_ct = 0LL;
                    if ((*this).match_any(std::vector{TK_COMPTIME_STRICT})) {
                        method_ct = 2LL;
                    }
                    else {
                        if ((*this).match_any(std::vector{TK_COMPTIME})) {
                            method_ct = 1LL;
                        }
                    }
                    methods.push_back((*this).function_declaration(std::string("public"), false, method_ct));
                    this->in_class_body = old;
                }
                else {
                    if ((*this).check(TK_IDENTIFIER) && ((*this).peek_at(1LL).token_type == TK_LEFT_PAREN)) {
                        auto vname = (*this).consume(TK_IDENTIFIER, std::string("Expect variant name."));
                        (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after variant name."));
                        std::vector<TypeNode> fields = {};
                        std::vector<std::string> fnames = {};
                        if ((!(*this).check(TK_RIGHT_PAREN))) {
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
                        if ((*this).check(TK_IDENTIFIER) && (((*this).peek_at(1LL).token_type == TK_NEWLINE) || ((*this).peek_at(1LL).token_type == TK_DEDENT))) {
                            auto vname = (*this).advance();
                            std::vector<TypeNode> empty_types = {};
                            std::vector<std::string> empty_fnames = {};
                            variants.push_back(EnumVariantNode(vname, empty_types, empty_fnames));
                            (*this).match_any(std::vector{TK_NEWLINE});
                        }
                        else {
                            TypeNode vtype = (*this).parse_type();
                            auto vname = (*this).consume(TK_IDENTIFIER, std::string("Expect variant name."));
                            std::vector<TypeNode> types = {};
                            std::vector<std::string> old_fnames = {};
                            bool is_unit_type = false;
                            {
                                const auto& _match_97 = vtype;
                                if (std::holds_alternative<std::decay_t<decltype(_match_97)>::NullType>(_match_97._data)) {
                                    is_unit_type = true;
                                }
                                else if (std::holds_alternative<std::decay_t<decltype(_match_97)>::Void>(_match_97._data)) {
                                    is_unit_type = true;
                                }
                                else {
                                    /* pass */
                                }
                            }
                            if ((!is_unit_type)) {
                                types.push_back(vtype);
                                old_fnames.push_back(std::string("value"));
                            }
                            variants.push_back(EnumVariantNode(vname, types, old_fnames));
                            (*this).match_any(std::vector{TK_NEWLINE});
                        }
                    }
                }
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end enum body."));
        return Stmt::make_Enum(name, variants, methods, visibility, type_params);
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
            while ((!(*this).check(TK_NEWLINE)) && (!(*this).is_at_end())) {
                (*this).advance();
            }
            (*this).match_any(std::vector{TK_NEWLINE});
        }
        if ((*this).match_any(std::vector{TK_EXTERN})) {
            return (*this).extern_declaration();
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
            if (((ctor_name == std::string("constructor")) || (ctor_name == std::string("destructor"))) && ((*this).peek_at(1LL).token_type == TK_LEFT_PAREN)) {
                auto name_tok = (*this).advance();
                (*this).consume(TK_LEFT_PAREN, ((std::string("Expect '(' after ") + (ctor_name)) + std::string(".")));
                std::vector<Param> params = (*this).parse_param_list();
                (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after parameters."));
                (*this).consume(TK_COLON, std::string("Expect ':' before body."));
                std::vector<Stmt> body = (*this).block();
                std::vector<std::string> empty_tp = {};
                return Stmt::make_Function(name_tok, params, TypeNode::make_Void(), body, false, 0LL, is_static, visibility, empty_tp);
            }
        }
        int64_t comptime_mode = 0LL;
        if ((*this).match_any(std::vector{TK_COMPTIME_STRICT})) {
            comptime_mode = 2LL;
        }
        else {
            if ((*this).match_any(std::vector{TK_COMPTIME})) {
                comptime_mode = 1LL;
            }
        }
        if ((*this).match_any(std::vector{TK_REF_MUT})) {
            return (*this).var_declaration_with_ref(visibility, true, true);
        }
        if ((*this).match_any(std::vector{TK_REF})) {
            return (*this).var_declaration_with_ref(visibility, true, false);
        }
        if ((*this).is_type_at_pos(0LL)) {
            int64_t next_pos = (*this).skip_type_tokens(0LL);
            if (((*this).peek_at(next_pos).token_type == TK_QUESTION)) {
                next_pos = (next_pos + 1LL);
            }
            auto next_token = (*this).peek_at(next_pos).token_type;
            if ((next_token == TK_IDENTIFIER) || (next_token == TK_FN) || (next_token == TK_INLINE) || (next_token == TK_COMPTIME) || (next_token == TK_OPERATOR)) {
                if ((*this).is_function_start()) {
                    return (*this).function_declaration(visibility, is_static, comptime_mode);
                }
                return (*this).var_declaration(visibility);
            }
        }
        return (*this).statement();
    }

    Stmt extern_declaration() {
        auto header = (*this).consume(TK_STRING, std::string("Expect header string after 'extern'.")).lexeme;
        std::string link_lib = std::string("");
        if ((*this).match_any(std::vector{TK_LINK})) {
            link_lib = (*this).consume(TK_STRING, std::string("Expect library string after 'link'.")).lexeme;
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after extern header."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start extern body."));
        std::vector<ExternType> types = {};
        std::vector<ExternFn> functions = {};
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                if ((*this).check(TK_IDENTIFIER) && ((*this).peek().lexeme == std::string("type"))) {
                    (*this).advance();
                    auto type_name = (*this).consume(TK_IDENTIFIER, std::string("Expect type name after 'type'."));
                    std::string cpp_name = type_name.lexeme;
                    if ((*this).match_any(std::vector{TK_EQUAL})) {
                        cpp_name = (*this).consume(TK_STRING, std::string("Expect C++ name string.")).lexeme;
                    }
                    types.push_back(ExternType(type_name.lexeme, cpp_name));
                    (*this).match_any(std::vector{TK_NEWLINE});
                }
                else {
                    TypeNode ret_type = (*this).parse_type();
                    (*this).consume(TK_FN, std::string("Expect 'fn' in extern function declaration."));
                    auto fn_name = (*this).consume(TK_IDENTIFIER, std::string("Expect function name."));
                    (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after extern function name."));
                    std::vector<Param> params = (*this).parse_param_list();
                    (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after extern function params."));
                    std::string fn_cpp_name = fn_name.lexeme;
                    if ((*this).match_any(std::vector{TK_EQUAL})) {
                        fn_cpp_name = (*this).consume(TK_STRING, std::string("Expect C++ name string.")).lexeme;
                    }
                    functions.push_back(ExternFn(fn_name.lexeme, fn_cpp_name, ret_type, params));
                    (*this).match_any(std::vector{TK_NEWLINE});
                }
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end extern body."));
        return Stmt::make_Extern(header, link_lib, types, functions);
    }

    std::vector<Stmt> parse_program() {
        std::vector<Stmt> statements = {};
        while ((!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
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
    print((std::string("Parsed statements: ") + static_cast<int64_t>(stmts.size())));
    for (const auto& stmt : stmts) {
        {
            const auto& _match_98 = stmt;
            if (std::holds_alternative<std::decay_t<decltype(_match_98)>::ExprStmt>(_match_98._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_98)>::ExprStmt>(_match_98._data);
                auto& expr = _v.expr;
                {
                    const auto& _match_99 = expr;
                    if (std::holds_alternative<std::decay_t<decltype(_match_99)>::Binary>(_match_99._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_99)>::Binary>(_match_99._data);
                        auto& left = *_v.left;
                        auto& op = _v.op;
                        auto& right = *_v.right;
                        print((std::string("Binary: ") + op.lexeme));
                        {
                            const auto& _match_100 = right;
                            if (std::holds_alternative<std::decay_t<decltype(_match_100)>::Binary>(_match_100._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_100)>::Binary>(_match_100._data);
                                auto& rl = *_v.left;
                                auto& rop = _v.op;
                                auto& rr = *_v.right;
                                print((std::string("  Right is Binary: ") + rop.lexeme));
                                print(std::string("OK: precedence correct (1 + (2 * 3))"));
                            }
                            else {
                                /* pass */
                            }
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
    }
}

struct ModuleInfo {
    std::string short_name;
    std::string full_name;
    std::string alias;
    std::string source;

};

struct ImportResolver {
    std::vector<std::string> resolved_paths;
    std::vector<ModuleInfo> modules;

    ImportResolver() {
        this->resolved_paths = {};
        this->modules = {};
    }

    bool already_resolved(const std::string& path) {
        for (const auto& p : this->resolved_paths) {
            if ((p == path)) {
                return true;
            }
        }
        return false;
    }

    std::string get_directory(const std::string& path) {
        int64_t i = (static_cast<int64_t>(path.size()) - 1LL);
        while ((i >= 0LL)) {
            if ((std::string(1, path[i]) == std::string("/"))) {
                return path.substr(0LL, ((i + 1LL)) - (0LL));
            }
            i = (i - 1LL);
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
        for (const auto& line : lines) {
            std::string trimmed = lv_trim(line);
            if (trimmed.starts_with(std::string("import "))) {
                std::string import_str = lv_trim(trimmed.substr(7LL, (static_cast<int64_t>(trimmed.size())) - (7LL)));
                std::string alias = std::string("");
                std::string mod_path = import_str;
                int64_t as_pos = lv_index_of(import_str, std::string(" as "));
                if ((as_pos >= 0LL)) {
                    alias = lv_trim(import_str.substr((as_pos + 4LL), (static_cast<int64_t>(import_str.size())) - ((as_pos + 4LL))));
                    mod_path = lv_trim(import_str.substr(0LL, (as_pos) - (0LL)));
                }
                std::string file_rel = lv_replace(mod_path, std::string("::"), std::string("/"));
                std::vector<std::string> segments = lv_split(file_rel, std::string("/"));
                std::string module_file = ((((std::string("") + (dir)) + std::string("")) + (file_rel)) + std::string(".lv"));
                if ((static_cast<int64_t>(segments.size()) > 1LL)) {
                    std::string short_name = segments[(static_cast<int64_t>(segments.size()) - 1LL)];
                    std::string full_name = lv_join(segments, std::string("_"));
                    std::string mod_source = (*this).resolve(module_file);
                    if ((mod_source != std::string(""))) {
                        this->modules.push_back(ModuleInfo(short_name, full_name, alias, mod_source));
                    }
                }
                else {
                    std::string resolved = (*this).resolve(module_file);
                    if ((resolved != std::string(""))) {
                        result = (result + (resolved + std::string("\n")));
                    }
                }
            }
            else {
                result = (result + (line + std::string("\n")));
            }
        }
        return result;
    }

};

void cleanup(const std::string& cpp_path, const std::string& header_path, const std::string& liblavina_path, bool wrote_header) {
    os_exec(((std::string("rm -f ") + (cpp_path)) + std::string("")));
    if (wrote_header) {
        os_exec(((std::string("rm -f ") + (header_path)) + std::string("")));
        os_exec(((std::string("rm -rf ") + (liblavina_path)) + std::string("")));
    }
}

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);
    auto args = os_args();
    if ((static_cast<int64_t>(args.size()) < 2LL)) {
        print(std::string("Usage: bootstrap [--emit-cpp | compile] <file.lv>"));
        return 1LL;
    }
    std::string mode = std::string("run");
    std::string path = std::string("");
    bool no_check = false;
    int64_t ai = 1LL;
    while ((ai < static_cast<int64_t>(args.size()))) {
        if ((args[ai] == std::string("--no-check"))) {
            no_check = true;
        }
        else {
            if ((args[ai] == std::string("--emit-cpp"))) {
                mode = std::string("emit-cpp");
            }
            else {
                if ((args[ai] == std::string("compile"))) {
                    mode = std::string("compile");
                }
                else {
                    path = args[ai];
                }
            }
        }
        ai = (ai + 1LL);
    }
    if ((path == std::string(""))) {
        print(std::string("Usage: bootstrap [--emit-cpp | compile] [--no-check] <file.lv>"));
        return 1LL;
    }
    auto resolver = ImportResolver();
    std::string source = resolver.resolve(path);
    std::vector<std::string> mod_short_names = {};
    std::vector<std::string> mod_full_names = {};
    std::vector<std::string> mod_aliases = {};
    std::vector<std::vector<Stmt>> mod_stmts_list = {};
    std::vector<std::vector<Stmt>> all_lambda_blocks = {};
    for (const auto& mod : resolver.modules) {
        auto mod_scanner = Scanner(mod.source);
        mod_scanner.scan_tokens();
        if ((static_cast<int64_t>(mod_scanner.errors.size()) > 0LL)) {
            print(((std::string("Scanner errors in module ") + (mod.full_name)) + std::string(":")));
            for (const auto& err : mod_scanner.errors) {
                print(err);
            }
            return 1LL;
        }
        try {
            auto mod_parser = Parser(mod_scanner.tokens);
            std::vector<Stmt> mod_stmts = mod_parser.parse_program();
            mod_short_names.push_back(mod.short_name);
            mod_full_names.push_back(mod.full_name);
            mod_aliases.push_back(mod.alias);
            mod_stmts_list.push_back(mod_stmts);
            for (const auto& lb : mod_parser.lambda_blocks) {
                all_lambda_blocks.push_back(lb);
            }
        }
         catch (const std::exception& err) {
            print(((((std::string("Error in module ") + (mod.full_name)) + std::string(": ")) + (err.what())) + std::string("")));
            return 1LL;
        }
    }
    auto scanner = Scanner(source);
    scanner.scan_tokens();
    if ((static_cast<int64_t>(scanner.errors.size()) > 0LL)) {
        print(std::string("Scanner errors:"));
        for (const auto& err : scanner.errors) {
            print(err);
        }
        return 1LL;
    }
    std::vector<Stmt> stmts = {};
    std::string cpp = std::string("");
    bool has_main = false;
    try {
        auto parser = Parser(scanner.tokens);
        stmts = parser.parse_program();
        if ((!no_check)) {
            std::vector<Stmt> all_stmts = {};
            for (const auto& ms : mod_stmts_list) {
                for (const auto& s : ms) {
                    all_stmts.push_back(s);
                }
            }
            for (const auto& s : stmts) {
                all_stmts.push_back(s);
            }
            auto check_errors = run_checker(all_stmts);
            if ((static_cast<int64_t>(check_errors.size()) > 0LL)) {
                print(std::string("Type check errors:"));
                for (const auto& cerr : check_errors) {
                    print(((std::string("  ") + (cerr)) + std::string("")));
                }
                return 1LL;
            }
        }
        for (const auto& lb : parser.lambda_blocks) {
            all_lambda_blocks.push_back(lb);
        }
        auto codegen = CppCodegen();
        codegen.set_modules(mod_short_names, mod_full_names, mod_aliases, mod_stmts_list);
        codegen.lambda_blocks = all_lambda_blocks;
        cpp = codegen.generate(stmts);
        has_main = codegen.has_main;
    }
     catch (const std::exception& err) {
        print(((std::string("Error: ") + (err.what())) + std::string("")));
        return 1LL;
    }
    if ((mode == std::string("emit-cpp"))) {
        print(cpp);
        return 0LL;
    }
    if ((!has_main)) {
        print(std::string("Error: no main() function defined."));
        return 1LL;
    }
    std::string dir = resolver.get_directory(path);
    std::string base = path;
    int64_t si = (static_cast<int64_t>(path.size()) - 1LL);
    while ((si >= 0LL)) {
        if ((std::string(1, path[si]) == std::string("/"))) {
            base = path.substr((si + 1LL), (static_cast<int64_t>(path.size())) - ((si + 1LL)));
            break;
        }
        si = (si - 1LL);
    }
    if (base.ends_with(std::string(".lv"))) {
        base = base.substr(0LL, ((static_cast<int64_t>(base.size()) - 3LL)) - (0LL));
    }
    std::string cpp_path = ((((std::string("") + (dir)) + std::string("")) + (base)) + std::string(".cpp"));
    std::string bin_path = ((((std::string("") + (dir)) + std::string("")) + (base)) + std::string(""));
    std::string header_path = ((std::string("") + (dir)) + std::string("lavina.h"));
    fs_write(cpp_path, cpp);
    bool wrote_header = false;
    std::string liblavina_path = ((std::string("") + (dir)) + std::string("liblavina"));
    if ((!fs_exists(header_path))) {
        try {
            std::string header_content = fs_read(std::string("runtime/lavina.h"));
            fs_write(header_path, header_content);
            os_exec(((std::string("cp -r runtime/liblavina ") + (liblavina_path)) + std::string("")));
            wrote_header = true;
        }
         catch (const std::exception& e) {
            print(std::string("Warning: could not find runtime/lavina.h"));
        }
    }
    std::string compile_cmd = ((((std::string("g++ -std=c++23 -o ") + (bin_path)) + std::string(" ")) + (cpp_path)) + std::string(""));
    int64_t compile_result = os_exec(compile_cmd);
    if ((compile_result != 0LL)) {
        print(std::string("Compilation failed"));
        cleanup(cpp_path, header_path, liblavina_path, wrote_header);
        return 1LL;
    }
    if ((mode == std::string("compile"))) {
        cleanup(cpp_path, header_path, liblavina_path, wrote_header);
        print(((std::string("Compiled: ") + (bin_path)) + std::string("")));
        return 0LL;
    }
    int64_t run_result = os_exec(bin_path);
    cleanup(cpp_path, header_path, liblavina_path, wrote_header);
    os_exec(((std::string("rm -f ") + (bin_path)) + std::string("")));
    if ((run_result != 0LL)) {
        return 1LL;
    }
    return 0LL;
}

