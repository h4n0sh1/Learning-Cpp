// Deep dive: How the compiler parses "operator" functions

#include <iostream>

// GRAMMAR RULES (simplified from C++ standard):
//
// declaration:
//     decl-specifier-seq declarator
//
// declarator:
//     ptr-operator declarator
//     noptr-declarator
//
// noptr-declarator:
//     declarator-id ( parameter-declaration-clause )
//     ...
//
// declarator-id:
//     id-expression
//     operator-function-id        <-- THIS IS WHERE "operator" APPEARS
//
// operator-function-id:
//     "operator" operator-token
//
// operator-token: one of
//     +  -  *  /  %  ^  &  |  ~  !  =  <  >
//     += -= *= /= %= ^= &= |= << >> >>= <<=
//     == != <= >= && || ++ -- , ->* ->
//     () []  new  delete  new[]  delete[]

class Demo {
public:
    // The compiler's parsing steps for: int operator+(int x)
    // 
    // Token stream: int | operator | + | ( | int | x | )
    //               ^^^   ^^^^^^^^   ^
    //               type  keyword   symbol = operator-function-id
    //
    // Parse tree:
    //   function_declaration
    //   ├── return_type: "int"
    //   ├── function_name: operator-function-id
    //   │   ├── "operator" (keyword)
    //   │   └── "+" (operator-token)
    //   └── parameters: "(int x)"
    
    int operator+(int x) {
        return x + 100;
    }
    
    // The "operator" keyword is ONLY valid in specific grammatical positions:
    // 1. Function declarations/definitions
    // 2. friend declarations
    // 3. Conversion operators (operator int(), operator bool(), etc.)
};

// Name Mangling Perspective:
// C++ compiler mangles function names to include type information
// 
// Regular function:
//   void foo(int x) -> mangled name: _Z3fooi
//   (Z = C++ mangled, 3 = length of name, foo = name, i = int parameter)
//
// Operator function:
//   Demo::operator+(int x) -> mangled name: _ZN4DemopiEi
//   (N...E = nested name, Demo = class, pl = "plus" operator, i = int)
//
// Each operator has a special encoding:
//   operator+   -> "pl" (plus)
//   operator-   -> "mi" (minus)
//   operator*   -> "ml" (multiply)
//   operator==  -> "eq" (equal)
//   operator<<  -> "ls" (left shift)
//   operator[]  -> "ix" (index)
//   operator()  -> "cl" (call)

// Compiler's symbol table entry for operator functions:
//
// struct FunctionSymbol {
//     string name;              // "operator+"
//     bool is_operator;         // true
//     OperatorKind op_kind;     // PLUS
//     Type return_type;         // int
//     vector<Type> param_types; // [int]
//     string mangled_name;      // "_ZN4DemopiEi"
// };

// Overload Resolution:
// When compiler sees: Demo d; d + 5;
//
// 1. Build list of candidate functions:
//    - Demo::operator+(int)        [member function]
//    - operator+(Demo, int)        [free function, if exists]
//
// 2. Check each candidate's parameters
// 3. Apply overload resolution rules
// 4. Select best match
// 5. Generate call to that function

void demonstrate_mangling() {
    std::cout << "=== Name Mangling for Operators ===" << std::endl << std::endl;
    
    // We can use typeid to see mangled names (platform-specific)
    Demo d;
    
    std::cout << "Regular call: d + 5" << std::endl;
    int result = d + 5;
    std::cout << "Result: " << result << std::endl << std::endl;
    
    std::cout << "Explicit call: d.operator+(5)" << std::endl;
    result = d.operator+(5);
    std::cout << "Result: " << result << std::endl << std::endl;
    
    std::cout << "Both compile to the same machine code!" << std::endl;
}

// The "operator" keyword is SYNTACTIC SUGAR:
// It lets you write:      a + b
// Instead of:             a.operator+(b)
//
// The compiler ALWAYS transforms operators to function calls.
// There's no special "operator machine code" - just regular function calls.

int main() {
    std::cout << "=== Compiler's View of 'operator' Keyword ===" << std::endl << std::endl;
    
    std::cout << "Key Insights:" << std::endl;
    std::cout << "1. 'operator' is a keyword that forms special function names" << std::endl;
    std::cout << "2. 'operator+' is literally the NAME of the function" << std::endl;
    std::cout << "3. Compiler transforms: a+b -> a.operator+(b)" << std::endl;
    std::cout << "4. Name mangling encodes operator type for linker" << std::endl;
    std::cout << "5. Operators are just functions with special calling syntax" << std::endl;
    std::cout << std::endl;
    
    demonstrate_mangling();
    
    return 0;
}
