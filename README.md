# Lavina

**Lavina** is a statically typed, indentation-based programming language that compiles to C++. The compiler is self-hosting — written in Lavina itself.

With the power of C++ and Python-style syntax, you can unlock the capabilities of Lavina!

> [!NOTE]
> The language is in beta. I'm by no means a chad-programmer, but I welcome all contributors if you find Lavina interesting enough!

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

## Get Started

### Prerequisites

You need a C++ compiler with C++23 support:

| Platform | Compiler |
|----------|----------|
| **macOS** | Xcode Command Line Tools (`xcode-select --install`) |
| **Linux** | GCC 14+ (`sudo apt install g++-14` on Ubuntu/Debian) |
| **Windows** | [MSYS2](https://www.msys2.org/) with MinGW (`pacman -S mingw-w64-ucrt-x86_64-gcc`) |

### Option 1: Download a release (recommended)

Download the latest release from [GitHub Releases](https://github.com/Raumberg/lavina/releases):

```sh
# macOS (Apple Silicon)
curl -L https://github.com/Raumberg/lavina/releases/latest/download/lavina-macos-arm64.tar.gz | tar xz

# Linux (x86_64)
curl -L https://github.com/Raumberg/lavina/releases/latest/download/lavina-linux-x86_64.tar.gz | tar xz
```

This extracts a `bin/` and `lib/` directory. Add `bin/` to your `PATH`:

```sh
export PATH="$PWD/bin:$PATH"
```

Or install system-wide:

```sh
sudo cp -r bin/* /usr/local/bin/
sudo cp -r lib/* /usr/local/lib/
```

### Option 2: Build from source

```sh
git clone https://github.com/Raumberg/lavina.git
cd lavina
make bootstrap    # compile compiler from saved C++ snapshot
make test         # run the test suite (27 tests)
make build        # optimized binary → build/
make install      # install to /usr/local/ (or: make install PREFIX=~/.local)
```

### Your first program

Create `hello.lv`:

```lavina
void fn main():
    print("Hello, Lavina!")
```

Compile and run:

```sh
lavina compile hello.lv
./hello
```

Or compile and run in one step:

```sh
lavina hello.lv
```

### Using the standard library

Lavina ships with a standard library:

```lavina
import std::fs
import std::os
import std::math

void fn main():
    // File I/O
    fs::write("greeting.txt", "Hello!")
    print(fs::read("greeting.txt"))

    // OS interaction
    print("cwd: ${os::cwd()}")

    // Math
    print("pi = ${math::PI}")
    print("sqrt(2) = ${math::sqrt(2.0)}")
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
- **FFI** — `extern` blocks with `import`/`link` for automatic `-I`/`-l` flags, `cpp {}` inline C++, expanded types (`int8`/`int16`/`int32`, `float32`, `usize`, `cstring`, `ptr[T]`)
- **Package manager** — `lvpkg` for dependency management
- **Self-hosting** — the compiler bootstraps from a saved C++ snapshot

## Project Structure

```
src/           compiler source (.lv): scanner, parser, checker, codegen, main
stages/        C++ snapshot for bootstrapping (stage-latest.cpp)
runtime/       C++ runtime header and support libraries
  liblavina/   C++ runtime modules (12 headers)
  std/         standard library modules (fs, os, math)
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
- `examples/complex/lvg/` — grep utility with colorized output
- `examples/complex/webserver/` — HTTP todo app (uses httplib via lvpkg)
- `examples/complex/raylib/` — Raylib window with animated graphics
- `examples/sqlite.lv` — SQLite via FFI

## Package Manager

Lavina ships with `lvpkg` — a dependency manager for C++ header libraries and native libraries from GitHub.

Create a `lavina.pkg` in your project:

```
# header-only C++ library
dep httplib https://github.com/yhirose/cpp-httplib v0.18.3 httplib.h

# directory of headers
dep json https://github.com/nlohmann/json v3.11.3 single_include/nlohmann/

# native library (pre-built binary + headers)
lib raylib https://github.com/raysan5/raylib 5.5 lib/libraylib.a include/raylib.h
```

Then run:

```sh
lvpkg install    # clone and install dependencies into deps/
lvpkg update     # update to specified versions
lvpkg list       # show dependency status
lvpkg clean      # remove deps/
```

Dependencies are cloned into `deps/` and checked out at the pinned version. Headers go to `deps/include/`, libraries to `deps/lib/`. The compiler automatically adds `-Ideps/include` and `-Ldeps/lib` when these directories exist.

## Documentation

See [DOCUMENTATION.md](DOCUMENTATION.md) for the full language reference.

## License

[MIT](LICENSE)
