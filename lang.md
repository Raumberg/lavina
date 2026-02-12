# Lavina

**Lavina** is a statically typed, indentation-based programming language that compiles to C++. The compiler is self-hosting — written in Lavina itself.

File extension: `.lv`

## Key Principles
1. **Static typing** with type inference (`auto`)
2. **Indentation-based syntax** — no braces or semicolons
3. **Compiled to C++** via source-to-source translation
4. **Self-hosting** — the compiler bootstraps from saved C++ snapshots

## Example

```lavina
int fn factorial(int n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

void fn main():
    int result = factorial(10)
    print("Result: ${result}")
```

## Types

```
Primitive:  int, float, bool, string, void
Composite:  [T] (array), {K: V} (map)
```

## Features

- **Functions**: `ReturnType fn name(params):`
- **Classes**: fields, methods, constructors
- **Enums / Sum types**: `enum Name:` with variant destructuring
- **Pattern matching**: `match expr:` with variant arms
- **Control flow**: `if/else`, `while`, `for x in collection:`
- **Imports**: `import module` (resolves `module.lv`)
- **String interpolation**: `"value is {expr}"`
- **Compound assignment**: `+=`, `-=`, `*=`, `/=`

## Project Structure

```
src/          — compiler source (.lv)
stages/       — saved C++ snapshots for bootstrapping
runtime/      — C++ runtime header and support libraries
examples/     — example .lv programs
```

## Building

```sh
make bootstrap   # compile from latest stage, verify fixed point
make snapshot    # save new stage if codegen changed
```
