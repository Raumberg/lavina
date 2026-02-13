# FFI Design — Lavina ↔ C/C++ Interop

## Philosophy

Lavina compiles to C++. We don't wrap C++ — we extend it. The FFI should make the entire C and C++ ecosystem accessible to Lavina code with minimal friction.

## Three Mechanisms

### 1. `extern` block — declare external types and functions

```lavina
extern "header.h" [link "library"]:
    type Name [= "cpp_name"]
    RetType fn name(params) [= "cpp_name"]
```

- `"header.h"` → codegen emits `#include "header.h"` (or `<header.h>`)
- `link "library"` → linker flag `-llibrary` (future: pass through to build system)
- `type Name` → declares that a type exists (for the checker)
- `= "cpp_name"` → optional mapping when Lavina name differs from C++ name
- Without `= "..."`, Lavina name = C++ name

**C library example:**
```lavina
extern "sqlite3.h" link "sqlite3":
    type sqlite3
    type sqlite3_stmt

    int32 fn sqlite3_open(cstring filename, ptr[ptr[sqlite3]] db)
    int32 fn sqlite3_close(ptr[sqlite3] db)
    int32 fn sqlite3_prepare_v2(ptr[sqlite3] db, cstring sql, int32 nbyte, ptr[ptr[sqlite3_stmt]] stmt, ptr[cstring] tail)
    int32 fn sqlite3_step(ptr[sqlite3_stmt] stmt)
    int32 fn sqlite3_finalize(ptr[sqlite3_stmt] stmt)
    cstring fn sqlite3_column_text(ptr[sqlite3_stmt] stmt, int32 col)
    int32 fn sqlite3_column_int(ptr[sqlite3_stmt] stmt, int32 col)
```

**C++ library example:**
```lavina
extern "nlohmann/json.hpp":
    type Json = "nlohmann::json"

extern "filesystem":
    type Path = "std::filesystem::path"
    bool fn path_exists(Path p) = "std::filesystem::exists"
    Path fn current_path() = "std::filesystem::current_path"
```

### 2. `ptr[T]` — raw pointer type

```lavina
ptr[T]         → T*
ptr[ptr[T]]    → T**
ptr[void]      → void*
```

Used at FFI boundaries for C-style pointer arguments. Not for general Lavina code.

### 3. `cpp { }` — inline C++ block (escape hatch)

```lavina
string fn get_hostname():
    cpp {
        char buf[256];
        gethostname(buf, sizeof(buf));
        return std::string(buf);
    }
```

- Scanner: `cpp` + `{` → slurp until matching `}` (track brace nesting)
- Codegen: insert contents verbatim
- Use case: complex C++ patterns that can't be expressed through `extern`

## Type System at FFI Boundary

### Expanded numeric types

| Lavina | C++ | Notes |
|--------|-----|-------|
| `int` | `int64_t` | Default integer, alias for `int64` |
| `int8` | `int8_t` | |
| `int16` | `int16_t` | |
| `int32` | `int32_t` | |
| `int64` | `int64_t` | Same as `int` |
| `float` | `double` | Default float, alias for `float64` |
| `float32` | `float` | |
| `float64` | `double` | Same as `float` |
| `usize` | `size_t` | Unsigned, for sizes and indices |
| `cstring` | `const char*` | C string pointer |
| `ptr[T]` | `T*` | Raw pointer |
| `bool` | `bool` | Unchanged |
| `string` | `std::string` | Unchanged |

### Automatic conversions at FFI boundary

When calling an `extern fn`, the compiler inserts conversions:

| From | To | Conversion |
|------|----|------------|
| `string` | `cstring` | `.c_str()` |
| `cstring` | `string` | `std::string(ptr)` |
| `int` | `int32` | `static_cast<int32_t>(x)` |
| `int32` | `int` | `static_cast<int64_t>(x)` |
| `int` | `usize` | `static_cast<size_t>(x)` |
| `usize` | `int` | `static_cast<int64_t>(x)` |
| `float` | `float32` | `static_cast<float>(x)` |
| `float32` | `float` | `static_cast<double>(x)` |

Conversions only happen at extern call sites. Internal Lavina code uses native types.

## Implementation Phases

### Phase 1: Expanded types (no FFI yet)
Add `int8`, `int16`, `int32`, `int64`, `float32`, `float64`, `usize`, `cstring`, `ptr[T]` to the type system. Scanner keywords, parser TypeNode variants, codegen emit_type mappings.

### Phase 2: extern block — basic
Parse `extern "header":` blocks. Codegen emits `#include`. Functions declared inside are callable from Lavina. No type conversions yet — types must match exactly.

### Phase 3: type declarations and name mapping
`type Name = "cpp_name"` in extern blocks. Support for C++ namespaced names.

### Phase 4: Automatic conversions
Insert `static_cast`, `.c_str()`, `std::string()` at extern call boundaries.

### Phase 5: cpp { } blocks
Scanner support for raw C++ blocks. Codegen passthrough.

### Phase 6: link directive
`link "library"` → propagate to compilation command (`-l` flag).

## Open Questions

- Should `ptr[T]` support arithmetic (`ptr + offset`)? Probably not in Lavina — use `cpp {}` for that.
- Should we support C-style arrays (`arr[T, N]` → `T[N]`)? Low priority.
- Callback passing: Lavina lambdas as C function pointers? Needs thought.
