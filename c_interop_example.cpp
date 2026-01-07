// C++ calling C functions - Example of binary compatibility
#include <iostream>

// Declare C functions using extern "C" to prevent name mangling
extern "C" {
    // These functions are defined in c_functions.c
    int add_numbers(int a, int b);
    double calculate_average(double* array, int size);
    void modify_struct(struct Point* p);
    
    // C struct definition (same layout in both languages)
    struct Point {
        int x;
        int y;
    };
}

int main() {
    std::cout << "=== C++ and C Interoperability Example ===" << std::endl;
    std::cout << std::endl;
    
    // Call C function directly - no conversion needed
    int sum = add_numbers(10, 20);
    std::cout << "C function add_numbers(10, 20) = " << sum << std::endl;
    
    // Pass array to C function - same memory layout
    double numbers[] = {1.5, 2.5, 3.5, 4.5, 5.5};
    double avg = calculate_average(numbers, 5);
    std::cout << "C function calculate_average() = " << avg << std::endl;
    
    // Pass struct to C function - binary compatible
    struct Point p = {10, 20};
    std::cout << std::endl << "Before C function: Point(" << p.x << ", " << p.y << ")" << std::endl;
    modify_struct(&p);
    std::cout << "After C function: Point(" << p.x << ", " << p.y << ")" << std::endl;
    
    std::cout << std::endl << "No overhead: same calling convention, same data layout!" << std::endl;
    
    return 0;
}
