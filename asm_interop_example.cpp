// C++ calling Assembly functions - Zero overhead interoperability
#include <iostream>

// Declare assembly functions using extern "C"
extern "C" {
    // These functions are defined in asm_functions.s
    int asm_add(int a, int b);
    long asm_multiply(long a, long b);
    void asm_swap(int* a, int* b);
}

int main() {
    std::cout << "=== C++ and Assembly Interoperability Example ===" << std::endl;
    std::cout << std::endl;
    
    // Call assembly function - arguments passed in registers (x86_64 calling convention)
    int sum = asm_add(15, 25);
    std::cout << "Assembly asm_add(15, 25) = " << sum << std::endl;
    
    // Call assembly multiply - uses 64-bit registers
    long product = asm_multiply(123456, 789);
    std::cout << "Assembly asm_multiply(123456, 789) = " << product << std::endl;
    
    // Call assembly swap - passes pointers directly
    int x = 100, y = 200;
    std::cout << std::endl << "Before swap: x=" << x << ", y=" << y << std::endl;
    asm_swap(&x, &y);
    std::cout << "After swap: x=" << x << ", y=" << y << std::endl;
    
    std::cout << std::endl << "Zero overhead: direct register usage, no conversions!" << std::endl;
    
    return 0;
}
