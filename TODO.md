# Lavina TODO

## Current Priority: FFI Foundation

### Phase 1: Expanded Type System
- [ ] `int8`, `int16`, `int32` numeric types
- [ ] `int64` as alias for `int`
- [ ] `float32` type
- [ ] `float64` as alias for `float`
- [ ] `usize` (size_t)
- [ ] `cstring` (const char*)
- [ ] `ptr[T]` (raw pointer)

### Phase 2: extern block (basic)
- [ ] `extern "header":` syntax — parser + codegen (#include)
- [ ] Function declarations inside extern blocks
- [ ] Callable from Lavina code

### Phase 3: Type declarations + name mapping
- [ ] `type Name` declarations in extern blocks
- [ ] `= "cpp_name"` mapping for types and functions
- [ ] C++ namespaced names support

### Phase 4: Automatic FFI conversions
- [ ] `string` ↔ `cstring` at extern boundary
- [ ] `int` ↔ `int32`/`usize` at extern boundary
- [ ] `float` ↔ `float32` at extern boundary

### Phase 5: cpp { } blocks
- [ ] Scanner: `cpp` + `{` → slurp raw C++ until matching `}`
- [ ] Codegen: verbatim insertion

### Phase 6: link directive
- [ ] `extern "header" link "lib":` syntax
- [ ] Pass `-l` flags to g++ compilation

## Next: Language Features
- [ ] Traits / interfaces (generic constraints)
- [ ] Operator overloading (via traits or methods)
- [ ] Enum methods (`option.unwrap()`)
- [ ] Default parameter values
- [ ] Destructuring assignment
- [ ] Multi-file compilation (replace import inlining)

## Std Library (Lavina-native)
- [ ] `Option[T]` with methods (map, unwrap, unwrap_or)
- [ ] `Result[T, E]` with methods (map, unwrap, is_ok)
- [ ] Iterator combinators (filter, map, reduce)

## Tooling
- [ ] LSP server for editor support
- [ ] Formatter
- [ ] Package manager
- [ ] Test framework
