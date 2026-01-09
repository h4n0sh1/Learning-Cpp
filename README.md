# Learning-Cpp
My journey of relearning Cpp from scratch with my best friend AI

## Day 1 - January 5, 2026

**Topic:** Understanding `std::cout` and operator overloading

Today we explored how `std::cout.operator<<()` works under the hood. We discovered an interesting quirk: when you explicitly call `.operator<<()` with a `const char*` pointer, the compiler may choose the `void*` overload instead of the `const char*` overload, resulting in hexadecimal memory address output instead of the actual string text. 

**Key learnings:**
- `std::ostream` has multiple overloaded versions of `operator<<`
- Direct `.operator<<()` calls can lead to ambiguous overload resolution
- The normal `<<` operator syntax (e.g., `std::cout << "text"`) always resolves correctly
- The `void*` overload outputs memory addresses in hexadecimal format
- Using free function `operator<<(std::cout, "text")` can help disambiguate when needed

This highlights an important C++ concept: explicit function calls can behave differently than their operator syntax equivalents due to overload resolution rules.

## Day 2 - January 7, 2026

**Topic:** Zero-overhead language interoperability (C++, C, and Assembly)

Today we explored what it means for languages to be binary compatible and how C++, C, and Assembly can call each other's functions without any overhead or data conversion. This is a fundamental feature of systems programming languages.

**What "Zero Overhead" Means:**
- Functions can call each other directly without conversion or wrappers
- Data structures have the same memory layout across languages
- Same calling conventions (how arguments are passed in registers/stack)
- Direct linking at the binary level

**Examples Created:**

### C++ and C Interoperability
Files: `c_interop_example.cpp`, `c_functions.c`

**Key learnings:**
- `extern "C"` prevents C++ name mangling, enabling C linkage
- Structs have identical binary layout in both languages
- Arrays and pointers passed directly without conversion
- Same calling convention means arguments go in the same registers

Compile: `gcc -c c_functions.c -o build/c_functions.o && g++ c_interop_example.cpp build/c_functions.o -o build/c_interop`

### C++ and Assembly Interoperability
Files: `asm_interop_example.cpp`, `asm_functions.s`

**Key learnings:**
- Direct register usage (ARM64: x0-x7 for 64-bit, w0-w7 for 32-bit)
- Assembly receives raw arguments from C++ in registers
- Pointers passed directly for manipulation
- Return values in designated registers (x0/w0)
- Platform-specific: ARM64 on Apple Silicon vs x86_64 on Intel

Compile: `as asm_functions.s -o build/asm_functions.o && g++ asm_interop_example.cpp build/asm_functions.o -o build/asm_interop`

**Why This Matters:**
This binary compatibility is why you can link C libraries with C++ programs, write performance-critical code in assembly and call it from C++, and use Fortran scientific libraries from C/C++ - all without marshalling, serialization, or foreign function interfaces (FFI).

## Day 3 - January 8, 2026

**Topic:** Deep dive into the `operator` keyword - grammar, compilation, and linking

Today we explored how the `operator` keyword works from the compiler's perspective, including parsing, name mangling, and how operator overloading is just syntactic sugar for function calls.

**Key learnings:**

### Grammar Level (Parsing)
- `operator+` is literally a **function name**, not special syntax
- Grammar rule: `operator-function-id ::= "operator" operator-symbol`
- The compiler parses it as a single identifier with special meaning
- Valid in function declarations, friend declarations, and conversion operators

### Compiler Transformation
When you write: `a + b`
1. Compiler checks if `a` is a user-defined type
2. Looks for `operator+` member function or free function
3. **Transforms to**: `a.operator+(b)` (regular function call)
4. Generates standard function call machine code

**No special "operator machine code"** - operators are just functions!

### Name Mangling (Linking)
```
Demo::operator+(int) → __ZN4DemoplEi
```
- Each operator gets a 2-letter code: `pl`(+), `mi`(-), `ls`(<<), `eq`(==), `ix`([]), `cl`(())
- Mangling encodes class name, operator type, and parameter types
- Linker uses mangled names to connect calls to definitions
- Tool `c++filt` demangles names for humans: `c++filt __ZN4DemoplEi`

### The Compilation Pipeline
1. **Compile time**: `extern` tells compiler "trust me, this exists elsewhere"
2. **Link time**: Linker matches symbol names using `nm` to view symbol tables
3. **Runtime**: Loader puts executable in memory with all symbols resolved

**Key insight:** You can call operator functions directly! `a.operator+(b)` and `a + b` compile to identical machine code.

**Files created:** `operator_keyword.cpp`, `operator_grammar.cpp`

Compile: `g++ operator_keyword.cpp -o build/operator_keyword`

## Day 4 - January 9, 2026

**Topic:** Member initializer lists and brace initialization `{}`

Today we explored two fundamental C++ initialization mechanisms: member initializer lists in constructors and the brace initialization syntax introduced in C++11.

**Key learnings:**

### Member Initializer Lists
Syntax: `Constructor(params) : member1{value1}, member2{value2} { body }`

**Why use them:**
- **Required for**: `const` members, reference members, base class constructors
- **More efficient**: Direct initialization (1 step) vs default-construct + assign (2 steps)
- **Initialization order**: Members initialized in declaration order, not list order
- **Best practice**: Always prefer initializer lists over assignment in constructor body

**Example:**
```cpp
Vector::Vector(int s) : elem{new double[s]}, sz{s} { }
// Better than: Vector(int s) { elem = new double[s]; sz = s; }
```

### Brace Initialization `{}` - Grammar & Compiler Perspective

**Grammar:** `braced-init-list ::= '{' initializer-list? '}'`

**Compiler Pipeline:**
1. **Lexer**: Tokenizes `{` and `}` as LBRACE, RBRACE
2. **Parser**: Creates `BracedInitList` AST node
3. **Semantic Analysis**: 
   - Checks for narrowing conversions (compile-time safety!)
   - Determines initialization type (constructor vs aggregate)
   - Overload resolution: prefers `std::initializer_list` constructor
4. **Code Generation**: Direct memory write or constructor call (zero overhead)

**Advantages over `=` and `()`:**
- **Prevents narrowing**: `int x{3.14}` → compile error (vs warning with `=` or `()`)
- **Aggregate initialization**: Works with structs without constructors
- **Solves "Most Vexing Parse"**: `Data d{Data{}}` is clearly an object
- **Uniform syntax**: Same `{}` works for all types (built-in, class, aggregate)

**Overload Resolution Priority:**
1. `std::initializer_list<T>` constructor (if exists)
2. Matching regular constructor
3. Aggregate initialization (if no constructors)

**Key insight:** Brace initialization provides **compile-time type safety with zero runtime overhead** - all narrowing checks happen during compilation, producing the same assembly as old-style initialization.

**Files created:** `initializer_list.cpp`, `brace_initialization.cpp`, `brace_init_compiler.cpp`

Compile: `g++ initializer_list.cpp -o build/initializer_list`
