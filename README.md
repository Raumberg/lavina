# Lavina

**Lavina** is a statically typed, indentation-based programming language that compiles to C++. The compiler is self-hosting — written in Lavina itself.

File extension: `.lv`

## Key Principles
1. **Static typing** with type inference (`auto`)
2. **Indentation-based syntax** — no braces or semicolons
3. **Compiled to C++** via source-to-source translation
4. **Self-hosting** — the compiler bootstraps from saved C++ snapshots

## Quick Example

```lavina
enum Shape:
    Circle(float radius)
    Rect(float w, float h)

float fn area(ref Shape s):
    match s:
        Circle(r):
            return 3.14159 * r * r
        Rect(w, h):
            return w * h
    return 0.0

void fn main():
    auto shapes = [Shape::Circle(5.0), Shape::Rect(3.0, 4.0)]
    for ref s in shapes:
        print("Area: ${area(s)}")
```

## Types

| Category | Types |
|----------|-------|
| Primitive | `int`, `float`, `bool`, `string`, `void` |
| Collections | `vector[T]`, `hashmap[K, V]`, `hashset[T]` |
| Optional | `T?` (nullable) |
| Inference | `auto` |
| User-defined | `class`, `struct`, `enum` |

## Features

- **Functions** with generics: `T fn identity[T](T x):`
- **Classes** and **structs** with methods, constructors, generics
- **Enums / sum types** with named fields and pattern matching
- **Pattern matching**: `match expr:` with variant destructuring
- **Generics**: `[T, U]` on functions, structs, and enums
- **References**: `ref` (immutable) and `ref!` (mutable) parameters and bindings
- **Ownership**: `own x` for explicit move semantics
- **Lambdas**: `(int x) => x * 2`
- **Control flow**: `if/elif/else`, `while`, `for x in collection:`, `for i in 0..n:`
- **Exception handling**: `try:/catch:`
- **String interpolation**: `"value is ${expr}"`
- **Compound assignment**: `+=`, `-=`, `*=`
- **Imports**: `import module` (resolves `module.lv` relative to current file)

## Project Structure

```
src/          — compiler source (.lv): scanner, parser, checker, codegen, main
stages/       — saved C++ snapshots for bootstrapping (stageN.cpp)
runtime/      — C++ runtime header and support libraries
tests/        — test suite (.lv files)
examples/     — example .lv programs
```

## Building

```sh
make bootstrap   # compile from latest stage, verify fixed point
make snapshot    # save new stage if codegen changed
make test        # run test suite
make evolve      # bootstrap + handle codegen output changes
```
