#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl << std::endl;
    
    // THE ISSUE: When you explicitly call .operator<<() with const char*,
    // the compiler might choose the void* overload (showing hex address)
    // instead of the const char* overload (showing text).
    
    // SOLUTION 1: Use the normal << operator (recommended!)
    std::cout << "Solution 1 - Normal usage: ";
    std::cout << "text" << std::endl;
    
    // SOLUTION 2: Create a helper function that forces the correct overload
    std::cout << std::endl << "Solution 2 - Helper function: ";
    auto print_string = [](const char* str) {
        std::cout << str;  // This uses the normal << which resolves correctly
    };
    print_string("text");
    std::cout << std::endl;
    
    // SOLUTION 3: Dereference and take address to force const char* type
    std::cout << std::endl << "Solution 3 - Explicit operator<< call: ";
    const char text[] = "text";  // Array, not pointer
    std::cout.operator<<(&text[0]) << std::endl;
    
    // COMPARISON: What happens with void* overload
    std::cout << std::endl << "Void* overload (hex address): ";
    const char* ptr = "text";
    std::cout.operator<<(static_cast<const void*>(ptr)) << std::endl;
    
    std::cout << std::endl << "Text value using normal <<: ";
    std::cout << ptr << std::endl;
    
    return 0;
}
