#include <iostream>
#include <vector>

// Grammar for initialization in C++:
//
// initializer:
//     = initializer-clause
//     ( expression-list )
//     { initializer-list }        <-- This is brace initialization
//     { }
//
// Three types of initialization syntax:
// 1. Copy initialization:     int x = 5;
// 2. Direct initialization:   int x(5);
// 3. Brace initialization:    int x{5};    <-- Introduced in C++11

// GRAMMAR RULES for brace initialization:
//
// braced-init-list:
//     { initializer-list }
//     { initializer-list , }
//     { }
//
// initializer-list:
//     initializer-clause
//     initializer-list , initializer-clause
//
// Example parse:  int x{5};
//                     ^  ^
//                     |  braced-init-list
//                     type-specifier

class Data {
public:
    int value;
    double ratio;
    
    // Constructor
    Data(int v, double r) : value{v}, ratio{r} {
        std::cout << "Constructor called: " << v << ", " << r << std::endl;
    }
    
    // Initializer list constructor (takes std::initializer_list)
    Data(std::initializer_list<int> list) {
        std::cout << "Initializer list constructor with " << list.size() << " elements" << std::endl;
        if (list.size() > 0) value = *list.begin();
        if (list.size() > 1) ratio = *(list.begin() + 1);
    }
};

void demonstrate_types() {
    std::cout << "=== Different Initialization Syntaxes ===" << std::endl << std::endl;
    
    // 1. Copy initialization (may allow implicit conversions)
    int a = 5;
    int b = 3.14;  // Narrows 3.14 to 3 - WARNING but compiles
    std::cout << "Copy init: int b = 3.14  ->  b = " << b << " (narrowing allowed)" << std::endl;
    
    // 2. Direct initialization (parentheses)
    int c(5);
    int d(3.14);  // Also narrows - WARNING but compiles
    std::cout << "Direct init: int d(3.14)  ->  d = " << d << " (narrowing allowed)" << std::endl;
    
    // 3. Brace initialization (uniform initialization)
    int e{5};
    // int f{3.14};  // ERROR! Narrowing conversion not allowed
    std::cout << "Brace init: int e{5}  ->  e = " << e << " (prevents narrowing)" << std::endl;
    std::cout << "Brace init: int f{3.14}  ->  COMPILE ERROR (catches narrowing!)" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_compiler_behavior() {
    std::cout << "=== How Compiler Handles {} ===" << std::endl << std::endl;
    
    // STEP 1: Compiler sees braced-init-list
    // Token stream: int | x | { | 5 | } | ;
    //               ^^^   ^   ^^^^^^^^^
    //               type  id  braced-init-list
    
    int x{5};
    
    // STEP 2: Compiler checks what's being initialized
    // - Built-in type (int, double, etc.)? -> Direct initialization
    // - Class type? -> Look for constructor or aggregate initialization
    
    // STEP 3: For class types, overload resolution:
    // Priority order:
    //   1. std::initializer_list constructor (if exists)
    //   2. Regular constructor matching the arguments
    //   3. Aggregate initialization (if no constructors)
    
    std::cout << "1. Class with regular constructor:" << std::endl;
    Data d1(10, 3.14);  // Calls Data(int, double) - using () to avoid narrowing error
    
    std::cout << std::endl << "2. Class with initializer_list constructor:" << std::endl;
    Data d2{1, 2, 3};   // Calls Data(std::initializer_list<int>)
    // Note: Data d3{10, 3.14} would cause COMPILE ERROR due to narrowing double->int
    
    std::cout << std::endl;
}

// Aggregate type (no user-declared constructors)
struct Point {
    int x;
    int y;
};

// Complex aggregate
struct Line {
    Point start;
    Point end;
};

void demonstrate_aggregate() {
    std::cout << "=== Aggregate Initialization with {} ===" << std::endl << std::endl;
    
    // Compiler sees: struct with no constructors -> aggregate initialization
    // Members are initialized in declaration order
    Point p{10, 20};  // x=10, y=20
    std::cout << "Point p{10, 20}: (" << p.x << ", " << p.y << ")" << std::endl;
    
    // Nested braces for nested aggregates
    Line line{{0, 0}, {10, 10}};
    std::cout << "Line{{0,0}, {10,10}}: start(" << line.start.x << "," << line.start.y 
              << ") end(" << line.end.x << "," << line.end.y << ")" << std::endl;
    
    // Can omit inner braces (compiler figures it out)
    Line line2{0, 0, 10, 10};  // Same as above
    std::cout << "Line{0,0,10,10}: start(" << line2.start.x << "," << line2.start.y 
              << ") end(" << line2.end.x << "," << line2.end.y << ")" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_narrowing() {
    std::cout << "=== Narrowing Conversion Prevention ===" << std::endl << std::endl;
    
    // The compiler checks: does initializer fit in target type?
    
    int a{5};           // OK: 5 fits in int
    // int b{5.5};      // ERROR: double->int loses information
    // int c{300000000000L}; // ERROR: long value too big for int
    
    // Narrowing rules checked at COMPILE TIME
    double d = 3.14;
    // int e{d};        // ERROR: variable of type double (even if runtime value is 3.0)
    int f = d;          // WARNING: but compiles (not brace init)
    
    std::cout << "int a{5}             -> OK" << std::endl;
    std::cout << "int b{5.5}           -> COMPILE ERROR (narrowing)" << std::endl;
    std::cout << "int e{d} where d=3.0 -> COMPILE ERROR (type mismatch)" << std::endl;
    std::cout << "int f = d            -> WARNING but compiles (old style)" << std::endl;
    
    std::cout << std::endl;
}

void demonstrate_most_vexing_parse() {
    std::cout << "=== Solving 'Most Vexing Parse' ===" << std::endl << std::endl;
    
    // Classic problem:
    // Data d(Data());  // Looks like creating Data with temp Data()
    //                  // BUT: compiler thinks it's a FUNCTION DECLARATION!
    //                  // Function 'd' that returns Data and takes function pointer
    
    // Brace initialization solves this:
    Data d{Data(10, 3.14)};  // Clearly object initialization, not function decl
    
    std::cout << "Data d{Data(10, 3.14)} -> Clearly an object (not function decl)" << std::endl;
    std::cout << "Data d(Data())         -> Parsed as function declaration!" << std::endl;
    
    std::cout << std::endl;
}

// Compiler's internal process for {}:
//
// 1. LEXER: Tokenize { and } as LBRACE, RBRACE
//
// 2. PARSER: Build braced-init-list node in AST
//    AST node: BracedInitList { expressions: [5] }
//
// 3. SEMANTIC ANALYSIS:
//    a) Determine what's being initialized (built-in vs class)
//    b) For class: check for std::initializer_list constructor first
//    c) Check narrowing conversions (type safety)
//    d) Match to constructor or do aggregate init
//
// 4. CODE GENERATION:
//    - Built-in types: direct mov/store instruction
//    - Class types: call appropriate constructor
//    - Aggregates: individual member initialization

int main() {
    demonstrate_types();
    demonstrate_compiler_behavior();
    demonstrate_aggregate();
    demonstrate_narrowing();
    demonstrate_most_vexing_parse();
    
    std::cout << "=== Summary: How Compiler Handles {} ===" << std::endl << std::endl;
    std::cout << "1. GRAMMAR: braced-init-list is a distinct syntactic form" << std::endl;
    std::cout << "2. PARSING: Creates BracedInitList AST node" << std::endl;
    std::cout << "3. SEMANTIC: Checks narrowing conversions (type safety)" << std::endl;
    std::cout << "4. OVERLOAD: Prefers initializer_list constructor if exists" << std::endl;
    std::cout << "5. FALLBACK: Uses matching constructor or aggregate init" << std::endl;
    std::cout << "6. CODEGEN: Emits constructor call or direct initialization" << std::endl;
    std::cout << std::endl;
    std::cout << "KEY: {} is 'uniform initialization' - same syntax for everything!" << std::endl;
    
    return 0;
}
