#include <iostream>

// COMPILER PIPELINE FOR BRACE INITIALIZATION
// =========================================
//
// SOURCE CODE: int x{5};
//
// STEP 1: LEXICAL ANALYSIS (Tokenizer)
// -------------------------------------
// Input:  "int x{5};"
// Output: [INT_KEYWORD, IDENTIFIER("x"), LBRACE, INT_LITERAL(5), RBRACE, SEMICOLON]
//
// STEP 2: SYNTAX ANALYSIS (Parser)
// ---------------------------------
// Build Abstract Syntax Tree (AST):
//
//         VariableDeclaration
//              |
//         +----|----+
//         |         |
//      Type("int") Declarator
//                    |
//              +-----|-----+
//              |           |
//         Name("x")   BracedInitList
//                          |
//                     IntLiteral(5)
//
// Grammar rule applied: 
//   declarator: identifier braced-init-list
//   braced-init-list: '{' initializer-list? '}'
//
// STEP 3: SEMANTIC ANALYSIS
// --------------------------
// Check 1: What type is being initialized? -> int (built-in type)
// Check 2: What's in the braced-init-list? -> single int value (5)
// Check 3: Narrowing check: int <- int? -> OK (same type)
// Check 4: Build initialization sequence
//
// For built-in types:
//   -> Direct initialization with value 5
//
// For class types, precedence:
//   1. Check for std::initializer_list<T> constructor
//   2. Check for constructor matching argument types
//   3. Check if aggregate (public members, no constructors)
//   4. Error if none match
//
// STEP 4: CODE GENERATION
// ------------------------
// For: int x{5};
// Assembly (ARM64):
//   mov     w0, #5        // Load immediate value 5
//   str     w0, [sp, #12] // Store to stack location of x
//
// For: std::vector<int> v{1, 2, 3};
// Generates:
//   1. Create std::initializer_list<int> in read-only memory
//   2. Call vector's initializer_list constructor
//   3. Constructor copies elements to heap

class Example {
public:
    int a;
    double b;
    
    // Regular constructor
    Example(int x, double y) : a{x}, b{y} {
        std::cout << "Regular constructor: " << x << ", " << y << std::endl;
    }
    
    // initializer_list constructor - has PRIORITY over regular constructor!
    Example(std::initializer_list<int> list) {
        std::cout << "Initializer list constructor: " << list.size() << " elements" << std::endl;
        auto it = list.begin();
        a = *it++;
        b = (it != list.end()) ? *it : 0.0;
    }
};

void show_overload_resolution() {
    std::cout << "=== Overload Resolution for {} ===" << std::endl << std::endl;
    
    // What constructor does compiler choose?
    
    // Case 1: Matches initializer_list<int> - THAT ONE WINS
    std::cout << "Example e1{1, 2}:" << std::endl;
    Example e1{1, 2};  // Calls initializer_list constructor, not Example(int, double)!
    
    std::cout << std::endl << "Example e2(1, 2.5):" << std::endl;
    Example e2(1, 2.5);  // Calls regular constructor (using parentheses)
    
    // Case 2: Empty braces
    std::cout << std::endl << "Example e3{}:" << std::endl;
    // Example e3{};  // Would call default constructor (not initializer_list)
    
    std::cout << std::endl;
}

// NARROWING DETECTION ALGORITHM
// ------------------------------
// Compiler checks at semantic analysis phase:
//
// bool is_narrowing(Type source, Type target, Value value) {
//     if (target.is_integer() && source.is_floating()) return true;
//     if (target.is_integer() && source.is_integer()) {
//         if (target.size() < source.size()) return true;
//         if (value.known_at_compile_time()) {
//             return !target.can_represent(value);
//         }
//     }
//     if (target.is_floating() && source.is_floating()) {
//         return target.size() < source.size();
//     }
//     return false;
// }

void show_narrowing_detection() {
    std::cout << "=== Narrowing Detection ===" << std::endl << std::endl;
    
    // Compiler checks these at COMPILE TIME:
    
    int a{5};              // OK: int literal fits in int
    // int b{5.0};         // ERROR: double->int (different types)
    // int c{300000000000}; // ERROR: value too large for int
    
    long l = 100;
    // int d{l};           // ERROR: long->int (might lose data)
    
    std::cout << "int a{5}              -> OK" << std::endl;
    std::cout << "int b{5.0}            -> COMPILE ERROR (type mismatch)" << std::endl;
    std::cout << "int c{300000000000}   -> COMPILE ERROR (overflow)" << std::endl;
    std::cout << "int d{l} where l:long -> COMPILE ERROR (potential data loss)" << std::endl;
    
    std::cout << std::endl;
}

// MEMORY LAYOUT COMPARISON
// -------------------------
//
// OLD STYLE: int arr[] = {1, 2, 3};
// NEW STYLE: int arr[]{1, 2, 3};
//
// Both produce IDENTICAL assembly:
//   .data
//   arr:
//       .long 1
//       .long 2
//       .long 3
//
// The {} syntax doesn't add runtime overhead - it's pure compile-time checking!

int main() {
    std::cout << "=== Understanding {} from Compiler Perspective ===" << std::endl << std::endl;
    
    std::cout << "COMPILER PIPELINE:" << std::endl;
    std::cout << "1. LEXER:    Tokenize { and } as LBRACE, RBRACE" << std::endl;
    std::cout << "2. PARSER:   Build BracedInitList AST node" << std::endl;
    std::cout << "3. SEMANTIC: Type checking + narrowing detection" << std::endl;
    std::cout << "4. CODEGEN:  Direct init or constructor call" << std::endl;
    std::cout << std::endl;
    
    show_overload_resolution();
    show_narrowing_detection();
    
    std::cout << "=== Key Insights ===" << std::endl;
    std::cout << "• {} creates a 'braced-init-list' AST node" << std::endl;
    std::cout << "• Semantic analysis phase checks for narrowing" << std::endl;
    std::cout << "• Overload resolution prefers initializer_list constructors" << std::endl;
    std::cout << "• Zero runtime overhead - all checks at compile time" << std::endl;
    std::cout << "• Same memory layout as old-style initialization" << std::endl;
    
    return 0;
}
