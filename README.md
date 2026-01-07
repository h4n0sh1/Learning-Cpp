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
