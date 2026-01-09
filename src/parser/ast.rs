use crate::lexer::Token;

#[derive(Debug, Clone, PartialEq)]
pub enum Type {
    Int,
    Float,
    Bool,
    String,
    Void,
    Auto,
    Dynamic,
    Array(Box<Type>),
    Tuple(Vec<Type>),
    Dict(Box<Type>, Box<Type>),
    Function(Box<Type>, Vec<Type>),
    Nullable(Box<Type>),
    Custom(String),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    Literal(Literal),
    Unary(Token, Box<Expr>),
    Binary(Box<Expr>, Token, Box<Expr>),
    Grouping(Box<Expr>),
    Variable(Token),
    Assign(Token, Box<Expr>),
    Logical(Box<Expr>, Token, Box<Expr>),
    Call(Box<Expr>, Token, Vec<Expr>),
    Index(Box<Expr>, Token, Box<Expr>), // New: collection[index]
    Vector(Vec<Expr>),
    Map(Vec<(Expr, Expr)>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Literal {
    Int(i64),
    Float(f64),
    String(String),
    Bool(bool),
    Null,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    Expression(Expr),
    Let(Token, Option<Type>, Option<Expr>),
    Return(Token, Option<Expr>),
    If(Expr, Box<Stmt>, Option<Box<Stmt>>),
    While(Expr, Box<Stmt>),
    For(Token, Expr, Box<Stmt>), // New: for item in collection:
    Block(Vec<Stmt>),
    Function(FunctionDecl),
    Directive(Directive),
}

#[derive(Debug, Clone, PartialEq)]
pub struct FunctionDecl {
    pub name: Token,
    pub params: Vec<(Token, Type)>,
    pub return_type: Type,
    pub body: Vec<Stmt>,
    pub directives: Vec<Directive>,
    pub is_inline: bool,
    pub is_comptime: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Directive {
    Simple(String),
    Parametric(String, Vec<Expr>),
    Doc(String),
    Block(Vec<Directive>, Vec<Stmt>),
}

pub struct Program {
    pub statements: Vec<Stmt>,
}
