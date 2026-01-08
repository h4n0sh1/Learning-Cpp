#include <iostream>

// The "operator" keyword allows you to define how operators work with your types

class Number {
private:
    int value;

public:
    Number(int v) : value(v) {}
    
    // From a grammar perspective, "operator+" is a FUNCTION NAME
    // The compiler treats it as: functionName = "operator" + operatorSymbol
    Number operator+(const Number& other) const {
        return Number(value + other.value);
    }
    
    // operator<< is a function named "operator<<"
    // For cout, it must be a free function (friend) to have cout on the left
    friend std::ostream& operator<<(std::ostream& os, const Number& num) {
        os << num.value;
        return os;
    }
    
    // operator[] - array subscript
    int& operator[](int index) {
        // Simplified - just return value for demo
        return value;
    }
    
    // operator() - function call operator (makes object callable)
    int operator()(int x, int y) {
        return value + x + y;
    }
    
    // Prefix ++
    Number& operator++() {
        ++value;
        return *this;
    }
    
    // Postfix ++ (dummy int parameter distinguishes it)
    Number operator++(int) {
        Number temp = *this;
        value++;
        return temp;
    }
    
    int getValue() const { return value; }
};

// Grammar perspective: This is how the compiler parses it
// 
// function_declaration:
//     return_type function_name ( parameters ) qualifiers { body }
//
// When function_name starts with "operator":
//     function_name: "operator" operator_symbol
//     operator_symbol: one of + - * / % == != < > <= >= [] () << >> & | ^ ...

int main() {
    std::cout << "=== Understanding the 'operator' Keyword ===" << std::endl << std::endl;
    
    Number a(10), b(20);
    
    // When you write: a + b
    // The compiler transforms this to: a.operator+(b)
    Number c = a + b;
    std::cout << "a + b = " << c << "  (calls a.operator+(b))" << std::endl;
    
    // When you write: std::cout << c
    // The compiler transforms this to: operator<<(std::cout, c)
    std::cout << "cout << c calls: operator<<(cout, c)" << std::endl << std::endl;
    
    // operator[] example
    std::cout << "a[0] = " << a[0] << "  (calls a.operator[](0))" << std::endl;
    
    // operator() makes the object callable like a function
    std::cout << "a(5, 3) = " << a(5, 3) << "  (calls a.operator()(5, 3))" << std::endl << std::endl;
    
    // Prefix vs Postfix
    Number d(100);
    std::cout << "d = " << d << std::endl;
    std::cout << "++d = " << ++d << "  (calls d.operator++())" << std::endl;
    std::cout << "d++ = " << d++ << "  (calls d.operator++(0), dummy int)" << std::endl;
    std::cout << "d now = " << d << std::endl << std::endl;
    
    // KEY INSIGHT: You can call operator functions directly!
    Number e(50), f(30);
    Number g = e.operator+(f);  // Same as: e + f
    std::cout << "e.operator+(f) = " << g << "  (explicit call)" << std::endl;
    
    std::cout << std::endl << "=== How the Compiler Sees It ===" << std::endl;
    std::cout << "Expression: a + b" << std::endl;
    std::cout << "Step 1: Identify 'a' is a Number type" << std::endl;
    std::cout << "Step 2: Look for 'operator+' member function or free function" << std::endl;
    std::cout << "Step 3: Transform to function call: a.operator+(b)" << std::endl;
    std::cout << "Step 4: Generate machine code for that function call" << std::endl;
    
    return 0;
}
