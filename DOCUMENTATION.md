# Lavina Language Documentation

## Table of Contents

1. [Basics](#basics)
2. [Types](#types)
3. [Variables and Constants](#variables-and-constants)
4. [Functions](#functions)
5. [Control Flow](#control-flow)
6. [Classes](#classes)
7. [Structs](#structs)
8. [Enums and Pattern Matching](#enums-and-pattern-matching)
9. [Generics](#generics)
10. [References and Ownership](#references-and-ownership)
11. [Lambdas](#lambdas)
12. [Collections](#collections)
13. [Strings](#strings)
14. [Error Handling](#error-handling)
15. [Imports and Modules](#imports-and-modules)
16. [Operator Overloading](#operator-overloading)
17. [Compile-Time Evaluation](#compile-time-evaluation)
18. [FFI (Foreign Function Interface)](#ffi-foreign-function-interface)
19. [Standard Library](#standard-library)
20. [Extension Methods](#extension-methods)
21. [Compiler and Bootstrap](#compiler-and-bootstrap)

---

## Basics

Lavina uses **indentation** to define blocks. No braces, no semicolons. A colon `:` opens a new block, and the body is indented by 4 spaces.

```lavina
void fn main():
    print("Hello, world!")
```

Comments use `//`:

```lavina
// This is a comment
int x = 42  // inline comment
```

Every program needs a `main()` function as its entry point. It can return `int` or `void`.

---

## Types

### Primitive Types

| Type | Description | C++ mapping |
|------|-------------|-------------|
| `int` | 64-bit signed integer | `int64_t` |
| `float` | 64-bit floating point | `double` |
| `bool` | Boolean | `bool` |
| `string` | UTF-8 string | `std::string` |
| `void` | No value | `void` |

### Collection Types

| Type | Description | C++ mapping |
|------|-------------|-------------|
| `vector[T]` | Dynamic array | `std::vector<T>` |
| `hashmap[K, V]` | Hash map | `std::unordered_map<K, V>` |
| `hashset[T]` | Hash set | `std::unordered_set<T>` |

### Special Types

| Type | Description |
|------|-------------|
| `auto` | Type inference — compiler deduces the type |
| `T?` | Nullable — `T` or `null` (maps to `std::optional<T>`) |

### Type Inference

Use `auto` to let the compiler infer the type:

```lavina
auto x = 42          // int
auto name = "Alice"  // string
auto items = [1, 2]  // vector[int]
```

---

## Variables and Constants

### Variables

```lavina
int x = 10
string name = "Alice"
auto inferred = 3.14
vector[int] nums = [1, 2, 3]
```

Variables declared without an initializer get a default value (`0`, `0.0`, `false`, `""` depending on type).

### Constants

```lavina
const int MAX = 100
const string PREFIX = "lv_"
```

Constants must have an initializer and cannot be reassigned.

### Compound Assignment

```lavina
x += 1
x -= 2
x *= 3
```

---

## Functions

### Declaration

Functions follow the pattern `ReturnType fn name(params):`.

```lavina
int fn add(int a, int b):
    return a + b

string fn greet(string name):
    return "Hello, ${name}!"

void fn say_hello():
    print("hello")
```

### Recursion

```lavina
int fn factorial(int n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

### Generic Functions

Type parameters go in `[brackets]` after the function name:

```lavina
T fn identity[T](T x):
    return x

U fn transform[T, U](T input, U default_val):
    return default_val
```

Usage — type arguments are inferred:

```lavina
auto x = identity(42)       // int
auto s = identity("hello")  // string
```

---

## Control Flow

### If / Elif / Else

```lavina
if x > 10:
    print("big")
elif x > 5:
    print("medium")
else:
    print("small")
```

### While Loop

```lavina
int i = 0
while i < 10:
    print(i)
    i += 1
```

### For Loop

Iterate over collections:

```lavina
auto items = [10, 20, 30]
for item in items:
    print(item)
```

Range-based loop (exclusive upper bound):

```lavina
for i in 0..10:
    print(i)  // 0, 1, 2, ..., 9
```

### Break and Continue

```lavina
while true:
    if done:
        break

for x in items:
    if x < 0:
        continue
    process(x)
```

### Pass

Empty block placeholder:

```lavina
if condition:
    pass  // do nothing
```

---

## Classes

Classes have fields (inferred from constructor), a constructor, and methods.

```lavina
class Counter:
    constructor(int start):
        this.value = start

    void fn increment():
        this.value += 1

    int fn get():
        return this.value
```

Usage:

```lavina
auto c = Counter(0)
c.increment()
print(c.get())     // 1
print(c.value)     // 1  (direct field access)
```

Fields are inferred from `this.field = value` assignments in the constructor. The constructor can take parameters which are assigned to fields.

### Destructor

```lavina
class Resource:
    constructor():
        this.handle = acquire()

    destructor():
        release(this.handle)
```

---

## Structs

Structs are like classes but with explicitly declared typed fields. They support generics.

```lavina
struct Point:
    float x
    float y
```

Struct instances are constructed by passing field values in order:

```lavina
auto p = Point(1.0, 2.0)
print(p.x)  // 1.0
```

### Struct Methods

```lavina
struct Box[T]:
    T value

    T fn get():
        return this.value

    void fn set(T v):
        this.value = v
```

### Generic Structs

```lavina
struct Pair[T, U]:
    T first
    U second

auto p = Pair[int, string](1, "hello")
```

---

## Enums and Pattern Matching

### Enum Declaration

Enums are sum types (tagged unions). Variants can have named fields, a single typed value, or no data.

```lavina
// Named fields (preferred)
enum Shape:
    Circle(float radius)
    Rect(float width, float height)
    Point

// Single-value shorthand
enum Token:
    int Number
    string Word
    null EndOfFile
```

### Creating Enum Values

Use `EnumName::VariantName(args)`:

```lavina
auto c = Shape::Circle(5.0)
auto r = Shape::Rect(3.0, 4.0)
auto p = Shape::Point()
```

### Pattern Matching

`match` destructures enum variants and binds their fields:

```lavina
match shape:
    Circle(r):
        print("Circle with radius ${r}")
    Rect(w, h):
        print("Rectangle ${w} x ${h}")
    Point():
        print("A point")
    _:
        print("Unknown")  // wildcard
```

The `_` arm is a wildcard that matches anything.

### Tag Access

Every enum value has a `_tag` string field:

```lavina
auto s = Shape::Circle(1.0)
print(s._tag)         // "Circle"
print(s["_tag"])      // "Circle"
```

### Generic Enums

```lavina
enum Option[T]:
    Some(T val)
    EmptyNone

enum Result[T, E]:
    Ok(T val)
    Err(E err)
```

Usage:

```lavina
Option[int] x = Option[int]::Some(42)
match x:
    Some(v):
        print("Got ${v}")
    EmptyNone():
        print("Nothing")
```

### Self-Referential Enums

Enums can reference themselves for recursive data structures. Self-referential fields are automatically wrapped in `shared_ptr`.

```lavina
enum Tree[T]:
    Leaf(T val)
    Node(Tree left, T val, Tree right)

auto tree = Tree[int]::Node(
    Tree[int]::Leaf(1),
    2,
    Tree[int]::Leaf(3)
)

match tree:
    Node(l, v, r):
        print(v)      // 2
        print(l._tag)  // "Leaf"  (auto-dereferenced)
    _:
        pass
```

---

## Generics

### Syntax

Type parameters use `[T, U, ...]` after the name:

```lavina
// Generic function
T fn first[T](vector[T] items):
    return items[0]

// Generic struct
struct Wrapper[T]:
    T value

// Generic enum
enum Option[T]:
    Some(T val)
    EmptyNone
```

### Instantiation

Type arguments are specified in square brackets:

```lavina
auto w = Wrapper[int](42)
auto p = Pair[string, bool]("ok", true)
```

For generic functions, type arguments are usually inferred:

```lavina
auto x = identity(42)  // T inferred as int
```

### Nested Generics

```lavina
Pair[int, Pair[string, bool]] nested = Pair[int, Pair[string, bool]](
    1, Pair[string, bool]("ok", true)
)
```

---

## References and Ownership

### Reference Parameters

By default, parameters are passed by value (copied). Use `ref` for an immutable reference, `ref!` for a mutable reference:

```lavina
void fn read_vec(ref vector[int] v):
    // v is read-only, no copy
    print(v.len())

void fn modify_vec(ref! vector[int] v):
    // v is mutable, modifies caller's variable
    v.push(42)
```

At the call site, `ref!` arguments must be marked:

```lavina
auto items = [1, 2, 3]
read_vec(items)        // implicit ref
modify_vec(ref! items) // explicit ref!
items.len()            // 4
```

### Reference Bindings

Create local aliases with `ref` / `ref!`:

```lavina
ref auto x = some_value      // immutable alias
ref! auto y = some_value     // mutable alias
y = 10                        // modifies some_value
```

### For Loop References

```lavina
for ref item in items:     // read-only reference
    print(item)

for ref! item in items:    // mutable reference
    item += 1              // modifies elements in-place
```

### Own (Move Semantics)

`own` transfers ownership, preventing further use of the source:

```lavina
auto data = [1, 2, 3]
auto moved = own data    // data is moved into moved
```

---

## Lambdas

### Expression Lambdas

Single-expression lambdas use `(params) => expr` syntax:

```lavina
auto double = (int x) => x * 2
auto add = (int x, int y) => x + y
auto greet = () => "hello"

print(double(5))   // 10
print(add(3, 4))   // 7
```

Lambdas capture variables from the enclosing scope (closures):

```lavina
int offset = 10
auto shifted = (int x) => x + offset
print(shifted(5))  // 15
```

Lambdas can be passed as arguments (using `auto` parameter type):

```lavina
int fn apply(auto f, int x):
    return f(x)

print(apply((int x) => x * x, 6))  // 36
```

### Block Lambdas

For multi-statement lambdas, use `(params):` followed by an indented block:

```lavina
auto process = (int x):
    auto result = x * 2
    print("Processing ${x} -> ${result}")
    return result

process(5)  // prints "Processing 5 -> 10"
```

Block lambdas are commonly used as callbacks:

```lavina
svr.Get("/", (ref Request req, ref! Response res):
    auto body = build_response()
    res.set_content(body, "text/html")
)

---

## Collections

### Vectors

```lavina
vector[int] v = [1, 2, 3]
auto v2 = [1, 2, 3]          // type inferred
vector[string] empty = []

v.push(4)                     // append
auto last = v.pop()           // remove and return last
v.len()                       // length
v.contains(2)                 // true
v.is_empty()                  // false
v.remove(0)                   // remove at index
v.reverse()                   // reverse in-place
v[0]                          // index access
v[0] = 10                     // index assignment
```

### Hashmaps

```lavina
hashmap[string, int] m = {"x": 10, "y": 20}

m["z"] = 30                   // insert / update
auto val = m["x"]             // lookup
m.has("x")                    // true
m.keys()                      // vector of keys
m.values()                    // vector of values
m.len()                       // number of entries
```

### Hashsets

```lavina
hashset[string] s = []

s.add("a")
s.add("b")
s.contains("a")               // true
s.len()                        // 2
```

---

## Strings

### Basics

```lavina
string s = "hello"
s.len()                        // 5
s.upper()                      // "HELLO"
s.lower()                      // "hello"
s.trim()                       // strip whitespace
s.replace("l", "r")           // "herro"
s.contains("ell")             // true
s.indexOf("ll")               // 2
s.starts_with("he")           // true
s.ends_with("lo")             // true
s.charAt(0)                   // "h"
s.substring(1, 4)             // "ell"
s.split(",")                  // split into vector
```

### String Interpolation

Use `${expr}` inside double-quoted strings:

```lavina
int x = 42
string msg = "The answer is ${x}"
print("${x} + ${x} = ${x + x}")
```

### Concatenation

The `+` operator concatenates strings. Non-string types are automatically converted:

```lavina
string s = "value: " + 42     // "value: 42"
string s2 = "pi = " + 3.14   // "pi = 3.140000"
```

### Join

```lavina
auto parts = ["a", "b", "c"]
parts.join(", ")               // "a, b, c"
```

---

## Error Handling

### Try / Catch

```lavina
try:
    auto data = fs_read("file.txt")
    process(data)
catch err:
    print("Error: ${err.what()}")
```

The `catch` variable name is optional:

```lavina
try:
    risky_operation()
catch:
    print("something went wrong")
```

### Throw

Throw a string to raise an exception:

```lavina
void fn validate(int x):
    if x < 0:
        throw "Value must be non-negative"
```

Exceptions propagate up the call stack and are caught by the nearest `try/catch`.

---

## Imports and Modules

### Basic Imports

Import another `.lv` file by module name (relative to the current file's directory):

```lavina
import scanner
import parser
```

`import scanner` resolves to `scanner.lv` in the same directory.

### Module Paths

Use `::` for multi-segment imports. These resolve to nested directories:

```lavina
import helpers::math_helper    // helpers/math_helper.lv
import utils::string_utils     // utils/string_utils.lv
```

Multi-segment imports are wrapped in C++ namespaces. Access exported symbols via the module name:

```lavina
import helpers::math_helper

void fn main():
    auto result = math_helper.add(1, 2)
```

### Aliases

Rename imports with `as`:

```lavina
import helpers::math_helper as math

void fn main():
    auto result = math.add(1, 2)
```

### Visibility

Symbols in imported modules are `private` by default. Mark them `public` to export:

```lavina
// In helpers/math_helper.lv:
public int fn add(int a, int b):
    return a + b

int fn internal_helper():   // private, not accessible from outside
    return 42
```

---

## Operator Overloading

Define custom operators on structs and classes using `Type operator OP (params):`:

```lavina
struct Vec2:
    float x
    float y

    Vec2 operator + (ref Vec2 other):
        return Vec2(this.x + other.x, this.y + other.y)

    Vec2 operator * (float scalar):
        return Vec2(this.x * scalar, this.y * scalar)

    bool operator == (ref Vec2 other):
        return this.x == other.x and this.y == other.y
```

If a type defines a `to_string()` method, it is automatically used for `print()` and string concatenation:

```lavina
struct Point:
    float x
    float y

    string fn to_string():
        return "(${this.x}, ${this.y})"

void fn main():
    auto p = Point(1.0, 2.0)
    print(p)                    // prints "(1.0, 2.0)"
    string s = "point: " + p   // "point: (1.0, 2.0)"
```

### Enum Methods

Enums can also have methods:

```lavina
enum Color:
    Red
    Green
    Blue

    string fn name():
        match this:
            Red():
                return "red"
            Green():
                return "green"
            Blue():
                return "blue"
        return ""

void fn main():
    auto c = Color::Red()
    print(c.name())  // "red"
```

---

## Compile-Time Evaluation

### comptime

Mark a function `comptime` to make it evaluable at compile time (maps to C++ `constexpr`):

```lavina
comptime int fn factorial(int n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

`comptime` functions can also be called at runtime.

### comptime!

Mark a function `comptime!` to force compile-time-only evaluation (maps to C++ `consteval`):

```lavina
comptime! int fn square(int x):
    return x * x
```

`comptime!` functions can only be called with compile-time-known arguments.

---

## FFI (Foreign Function Interface)

### Expanded Types

Additional types for FFI interop:

| Type | C++ mapping |
|------|-------------|
| `int8` | `int8_t` |
| `int16` | `int16_t` |
| `int32` | `int32_t` |
| `int64` | `int64_t` (alias for `int`) |
| `float32` | `float` |
| `float64` | `double` (alias for `float`) |
| `usize` | `size_t` |
| `cstring` | `const char*` |
| `ptr[T]` | `T*` |

### extern Blocks

Declare C/C++ functions and types from external headers:

```lavina
extern "httplib.h":
    type Server = "httplib::Server"
    type Request = "httplib::Request"
    fn some_function(int32 x) = "ext_func"
```

- `type Name = "cpp_name"` declares an external type with optional C++ name mapping
- `fn name(params) = "cpp_name"` declares an external function with optional name mapping
- Type conversions (`string` <-> `cstring`, `int` <-> `int32`, etc.) happen automatically at the boundary

### cpp {} Blocks

Embed raw C++ code as an escape hatch:

```lavina
int fn str_to_int(string s):
    cpp {
        return std::stoll(s);
    }
    return 0
```

The C++ code has access to all variables in scope. The `return 0` after the `cpp` block is needed to satisfy the Lavina type checker.

---

## Standard Library

### I/O

| Function | Description |
|----------|-------------|
| `print(value)` | Print with newline. Works with `int`, `float`, `bool`, `string`, `vector`. |
| `input(prompt)` | Read a line from stdin. Optional prompt string. |

### Filesystem

| Function | Description |
|----------|-------------|
| `fs_read(path)` | Read file contents as string. Throws on error. |
| `fs_write(path, content)` | Write string to file. Throws on error. |
| `fs_append(path, content)` | Append string to file. |
| `fs_exists(path)` | Check if file exists. Returns `bool`. |
| `fs_remove(path)` | Delete a file. Returns `bool`. |
| `fs_is_dir(path)` | Check if path is a directory. Returns `bool`. |
| `fs_listdir(path)` | List directory entries. Returns `vector[string]`. |
| `fs_read_lines(path)` | Read file as vector of lines. |

### OS

| Function | Description |
|----------|-------------|
| `os_exec(cmd)` | Execute shell command. Returns exit code (`int`). |
| `os_args()` | Get command-line arguments as `vector[string]`. |
| `os_env(name)` | Get environment variable. Returns `""` if not set. |
| `os_clock()` | Milliseconds since epoch (`int`). |
| `os_sleep(ms)` | Sleep for given milliseconds. |
| `os_cwd()` | Get current working directory. |
| `exit(code)` | Exit the process with given code. |

### Conversion

| Function | Description |
|----------|-------------|
| `to_string(value)` | Convert `int`, `float`, `bool` to `string`. |
| `to_int(value)` | Parse string to `int`. |
| `to_float(value)` | Parse string to `float`. |

### Utility

| Function | Description |
|----------|-------------|
| `len(value)` | Length of string or vector. |
| `lv_assert(cond, msg)` | Assert condition, exit with message on failure. |

### Math

| Function | Description |
|----------|-------------|
| `abs(n)` | Absolute value (int). |
| `fabs(n)` | Absolute value (float). |
| `min(a, b)` / `max(a, b)` | Integer min/max. |
| `fmin(a, b)` / `fmax(a, b)` | Float min/max. |
| `clamp(val, lo, hi)` | Clamp integer to range. |
| `floor(n)` / `ceil(n)` / `round(n)` | Rounding (float). |
| `sqrt(n)` / `pow(base, exp)` | Square root, power. |
| `log(n)` / `log2(n)` | Logarithms. |
| `sin(n)` / `cos(n)` | Trigonometry. |
| `random(min, max)` | Random integer in range. |
| `random_float()` | Random float in [0, 1). |

### Standard Library Modules

The standard library is organized into importable modules:

```lavina
import std::fs           // fs::read(), fs::write(), ...
import std::os           // os::args(), os::exec(), ...
import std::math         // math::PI, math::sqrt(), ...
import std::collections  // vector/hashset dot-methods + free functions
```

**std::collections** provides higher-order functions both as free functions and as dot-notation methods via `extend`:

```lavina
import std::collections

vector[int] nums = [1, 2, 3, 4, 5]

// Dot-notation (via extend)
auto doubled = nums.map((int x) => x * 2)
auto evens = nums.filter((int x) => x % 2 == 0)
int sum = nums.reduce((int acc, int x) => acc + x, 0)

// Free function style
auto doubled2 = collections::map(nums, (int x) => x * 2)

// Generators (free functions only)
auto r = collections::range(0, 10)
```

Available dot-methods on vectors: `map`, `filter`, `reduce`, `for_each`, `zip`, `take`, `drop`, `enumerate`.
Available dot-methods on hashsets: `union_with`, `intersect`, `difference`.

---

## Extension Methods

The `extend` keyword adds methods to existing types:

```lavina
extend vector:
    auto fn double_all():
        return __lv_col_map(this, (auto x) => x * 2)

    int fn sum():
        int result = 0
        for x in this:
            result += x
        return result

extend string:
    string fn shout():
        return this + "!"
```

Usage:

```lavina
vector[int] nums = [1, 2, 3]
auto d = nums.double_all()    // [2, 4, 6]
int s = nums.sum()            // 6

string greeting = "hello"
print(greeting.shout())       // hello!
```

Extend methods work on: `vector`, `hashmap`, `hashset`, `string`, and custom types (structs/enums by name).

Use `this` inside extend methods to refer to the object. Built-in methods (`.push()`, `.sort()`, etc.) take priority over extend methods.

---

## Compiler and Bootstrap

### Architecture

The Lavina compiler is self-hosting, written in Lavina itself:

```
src/scanner.lv   — Lexer: source code → tokens
src/ast.lv       — AST type definitions (enums and structs)
src/parser.lv    — Parser: tokens → AST
src/checker.lv   — Type checker: validates AST
src/codegen.lv   — Code generator: AST → C++ source
src/main.lv      — CLI: import resolution, compilation pipeline
```

### Compilation Pipeline

```
.lv source → Scanner → Tokens → Parser → AST → Checker → Codegen → C++ → g++  → binary
```

### CLI Usage

```sh
# Compile and run
./bootstrap program.lv

# Compile only (produces binary)
./bootstrap compile program.lv

# Emit C++ source
./bootstrap --emit-cpp program.lv

# Skip type checking
./bootstrap --no-check program.lv
```

### Bootstrap Process

The compiler maintains C++ snapshots in `stages/`. Each snapshot is a complete single-file C++ compilation of the compiler.

```sh
make bootstrap   # Build from latest stage, verify fixed point
make snapshot    # Save new stage after codegen changes
make evolve      # Handle codegen output format changes (2-pass)
make test        # Run test suite (37 tests)
```

**Fixed point verification**: The bootstrap compiles `src/main.lv` twice — once with the previous stage and once with the newly built compiler. The outputs must be identical, proving the compiler correctly compiles itself.

**Adding new features**:
1. Implement the feature in `src/*.lv`
2. `make bootstrap` — build with old stage
3. `make snapshot` — save new stage
4. Now you can use the new syntax in `.lv` files
5. `make bootstrap` — verify everything still works
