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
