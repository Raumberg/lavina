# Lavina — Knowledge Base

## Обзор проекта

Lavina — статически типизированный язык программирования с Python-подобным синтаксисом (значимые отступы), написанный на Rust. Расширение файлов: `.lv`.

**Философия:** строгая типизация + быстрая обратная связь через интерпретацию, с поэтапной компиляцией от tree-walking до нативного кода.

---

## Архитектура (текущая)

```
Исходный код (.lv)
    │
    ▼
┌──────────┐
│  Lexer   │  src/lexer/scanner.rs, token.rs (~460 строк)
│          │  Токенизация с учётом отступов (Indent/Dedent)
└──────────┘
    │
    ▼
┌──────────┐
│  Parser  │  src/parser/parser.rs, ast.rs (~783 строк)
│          │  Recursive descent → AST
└──────────┘
    │
    ▼
┌──────────────┐
│ Type Checker │  src/type_checker/checker.rs, env.rs (~774 строк)
│              │  Статическая проверка типов, null safety, auto-вывод
└──────────────┘
    │
    ▼
┌──────────┐
│ Compiler │  src/compiler/compiler.rs, scope.rs (~917 строк)
│          │  AST → байткод (41 опкод)
└──────────┘
    │
    ▼
┌──────────┐
│   VM     │  src/vm/vm.rs, opcode.rs, chunk.rs, object.rs, frame.rs,
│          │  heap.rs, memory.rs (~965 строк)
│          │  Стековая виртуальная машина + Mark-and-Sweep GC
└──────────┘
```

Также существует устаревший tree-walking интерпретатор в `src/eval/`.

### Точка входа

`src/main.rs` — CLI: без аргументов запускает REPL, с аргументом — выполняет файл.

Pipeline: `Scanner → Parser → TypeChecker → Compiler → VM`

---

## Система типов

### Базовые типы
- `int` — 64-bit signed integer
- `float` — double precision
- `bool` — True/False
- `string` — UTF-8
- `void` — unit/null
- `auto` — вывод типа
- `dynamic` — динамическая типизация (runtime)
- `null`

### Составные типы
- `[T]` / `vector[T]` — массивы
- `(T1, T2)` — кортежи
- `{K: V}` / `hashmap[K, V]` — словари
- `T?` — nullable
- `R fn(T)` — функциональные типы

### Представление в коде (src/type_checker)
```rust
enum TypeInfo {
    Variable(Type),
    Function(Type, Vec<Type>, bool, bool),  // return, params, static, variadic
    Namespace(String, HashMap<String, (TypeInfo, Visibility)>),
    Class(String, HashMap<String, (TypeInfo, Visibility)>),
}
```

---

## Синтаксис языка

### Переменные и функции
```lavina
int x = 10
auto name = "Lavina"

int fn factorial(int n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

### Замыкания
```lavina
auto fn make_counter():
    int count = 0
    auto fn inc():
        count = count + 1
        return count
    return inc
```

### Классы
```lavina
class Player:
    string name
    int health

    constructor(string name):
        this.name = name
        this.health = 100

    void fn take_damage(int amount):
        this.health = this.health - amount
```

### Enum
```lavina
enum Result:
    int Ok
    string Error
    null Loading
```

### Коллекции
```lavina
vector[int] nums = [1, 2, 3]
hashmap[string, int] ages = {"alice": 30, "bob": 25}
```

### Управление потоком
```lavina
if condition:
    ...
else:
    ...

while i < 10:
    i = i + 1

for item in collection:
    print(item)

try:
    throw "error"
catch err:
    print(err)
```

### Директивы
```lavina
#[pure, optimize[2]]
inline int fn fast(int x):
    return x * x

#doc "Описание функции"
```

### Модули
```lavina
import math
import fs
```

---

## Виртуальная машина

### Опкоды (41 инструкция)
- **Арифметика:** Add, Subtract, Multiply, Divide, Negate
- **Сравнения:** Equal, Greater, Less
- **Логика:** Not, And, Or
- **Переходы:** Jump, JumpIfFalse, Loop
- **Переменные:** DefineGlobal, GetGlobal, SetGlobal, GetLocal, SetLocal
- **Функции:** Call, Return, Closure, GetUpvalue, SetUpvalue, CloseUpvalue
- **Коллекции:** Vector, HashMap, GetIndex, SetIndex
- **ООП:** Class, Method, GetProperty, SetProperty
- **Прочее:** Constant, Pop, Null, True, False, Namespace, Cast, Try, EndTry, Throw

### Стек и фреймы
- Стек на 256 значений
- Call frames для вызовов функций
- Heap-объекты: String, Vector, HashMap, Function, Closure, Upvalue, Namespace, Class, Instance, BoundMethod

### Сборка мусора
Mark-and-Sweep, порог запуска — 1MB.

---

## Стандартная библиотека

Расположена в `std/`, написана частично на Lavina с нативными привязками через `__native_*` функции.

| Модуль      | Файл        | Содержимое |
|-------------|-------------|------------|
| core        | core.lv     | len, clock, typeof, range, String, Int, Vector, HashMap, Result, Option |
| math        | math.lv     | sqrt, abs, sin, cos, tan, min, max, floor, ceil, round, PI, E |
| io          | io.lv       | input, print |
| fs          | fs.lv       | fs_read, fs_write, fs_exists |
| json        | json.lv     | json_stringify, json_parse |
| memory      | memory.lv   | memory_addr, memory_deref, memory_allocated, memory_sizeof |
| os          | os.lv       | os_args, os_env |

60+ нативных функций реализованы в Rust и проброшены через `__native_*` обёртки.

---

## Тесты

7 тестовых модулей в `tests/`:
- lexer, parser, type, exception, oop, stdlib, module

28 примеров-программ в `probes/`.

---

## Текущий статус разработки

| Фаза | Статус |
|-------|--------|
| Phase 1: Tree-walking interpreter | Завершена |
| Phase 2: Bytecode VM | В процессе (замыкания, ООП, исключения работают) |
| Phase 3: JIT/AOT | Не начата |

---

## Что нужно для компиляции в C (новый бэкенд)

### Уже есть
- Полный лексер, парсер, тайпчекер — переиспользуются без изменений
- AST содержит всю необходимую информацию для кодогенерации
- Файловый I/O в стандартной библиотеке
- Базовые средства работы с памятью

### Нужно добавить/изменить
- **Новый модуль кодогенерации:** AST → C-код (вместо/параллельно с AST → байткод)
- **Runtime-библиотека на C:** GC или refcounting, строки, коллекции, диспатч методов
- **FFI/extern:** объявление и вызов внешних C-функций
- **Управление памятью без VM:** замена GC на что-то работающее нативно
- **Дженерики:** для написания компилятора на самом Lavina нужны обобщённые типы (AST-ноды)

### Этапы миграции
1. Добавить C-бэкенд в текущий Rust-компилятор (AST → .c файл)
2. Написать минимальный C-runtime (строки, массивы, GC)
3. Скомпилировать простые .lv программы через C
4. Постепенно покрыть все фичи языка
5. Переписать компилятор на Lavina (self-host)
