# Lavina

**Lavina** is a statically typed, indentation-based programming language that compiles to C++. The compiler is self-hosting — written in Lavina itself.  

With a power of C++ and Python-style syntax, you can unlock capabilities of Lavina!  
> [!NOTE]
> The language is in beta, I'm by no means a chad-programmer, but I welcome all the contributors if you find Lavina interesting enough!

## Quick Example

```lavina
enum Shape:
    Circle(float radius)
    Rect(float w, float h)

    string fn describe():
        match this:
            Circle(r):
                return "circle r=${r}"
            Rect(w, h):
                return "rect ${w}x${h}"
        return ""

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
        print("${s.describe()}: area=${area(s)}")
```

## Features

- **Indentation-based syntax** — no braces or semicolons
- **Static typing** with type inference (`auto`)
- **Generics** — `[T, U]` on functions, structs, and enums
- **Enums / sum types** with named fields and pattern matching
- **Enum methods** — methods defined inside enum bodies
- **Operator overloading** — `Type operator + (params):`
- **References and ownership** — `ref`, `ref!`, `own`
- **Block lambdas** — `(params):` with indented body
- **Module system** — `import a::b`, `public`/`private`, `as` aliases
- **Compile-time evaluation** — `comptime` / `comptime!`
- **FFI** — `extern` blocks, `cpp {}` inline C++, expanded types (`int8`/`int16`/`int32`, `float32`, `usize`, `cstring`, `ptr[T]`)
- **Package manager** — `lvpkg` for dependency management
- **Self-hosting** — the compiler bootstraps from a saved C++ snapshot

## Building

Requires `g++` with C++23 support.

```sh
make bootstrap   # compile from saved stage, verify fixed point
make test        # run test suite (22 tests)
make build       # optimized binary in build/
make install     # copy to /usr/local/bin
```

## Project Structure

```
src/           compiler source (.lv): scanner, parser, checker, codegen, main
stages/        C++ snapshot for bootstrapping (stage-latest.cpp)
runtime/       C++ runtime header and support libraries
tests/         test suite (.lv files)
examples/      example programs
lvpkg/         package manager (written in Lavina)
editors/       editor support (nvim, vscode)
design/        language design documents
```

## Examples

- `examples/basics.lv` — variables, functions, control flow
- `examples/match.lv` — enums and pattern matching
- `examples/collections.lv` — vectors, hashmaps, hashsets
- `examples/complex/tree/` — file tree printer
- `examples/complex/lvg/` — Git client
- `examples/complex/webserver/` — HTTP todo app (uses httplib via lvpkg)
- `examples/sqlite.lv` — SQLite via FFI

## Package Manager

Lavina ships with `lvpkg` — a simple dependency manager for pulling C++ header-only libraries and Lavina modules from GitHub.

```sh
make lvpkg       # build the package manager
```

Create a `lavina.pkg` in your project:

```
# header-only C++ library
dep httplib https://github.com/yhirose/cpp-httplib v0.18.3 httplib.h

# directory of headers
dep json https://github.com/nlohmann/json v3.11.3 single_include/nlohmann/
```

Then run:

```sh
lvpkg install    # install dependencies into deps/
lvpkg update     # update to specified versions
lvpkg list       # show dependency status
lvpkg clean      # remove deps/
```

Dependencies are cloned into `deps/` and checked out at the pinned version.

## Documentation

See [DOCUMENTATION.md](DOCUMENTATION.md) for the full language reference.

## License

[MIT](LICENSE)
