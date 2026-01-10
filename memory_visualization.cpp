#include <iostream>
#include <iomanip>

// Demonstrating "representation is part of definition" with actual memory addresses

class Vector {
private:
    double* elem;
    int sz;
public:
    Vector(int s) : elem{new double[s]}, sz{s} {}
    ~Vector() { delete[] elem; }
    
    void show_memory() {
        std::cout << "Vector object itself:" << std::endl;
        std::cout << "  Address of Vector object: " << this << std::endl;
        std::cout << "  Size of Vector object:    " << sizeof(*this) << " bytes" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Members inside Vector object:" << std::endl;
        std::cout << "  Address of elem member:   " << &elem << std::endl;
        std::cout << "  Value of elem (pointer):  " << elem << " <- points to heap" << std::endl;
        std::cout << "  Address of sz member:     " << &sz << std::endl;
        std::cout << "  Value of sz:              " << sz << std::endl;
        std::cout << std::endl;
        
        std::cout << "Heap data (pointed to by elem):" << std::endl;
        std::cout << "  elem[0] at: " << &elem[0] << std::endl;
        std::cout << "  elem[1] at: " << &elem[1] << std::endl;
        std::cout << "  elem[2] at: " << &elem[2] << std::endl;
        
        // Calculate distance between Vector object and heap data
        long long distance = reinterpret_cast<char*>(elem) - reinterpret_cast<char*>(this);
        std::cout << std::endl;
        std::cout << "Distance from Vector object to heap data: " 
                  << std::abs(distance) << " bytes" << std::endl;
        std::cout << "They're in DIFFERENT memory regions!" << std::endl;
    }
};

int main() {
    std::cout << "=== Visualizing 'Representation in Definition' ===" << std::endl << std::endl;
    
    Vector v(5);
    v.show_memory();
    
    std::cout << std::endl << std::endl;
    std::cout << "=== What This Shows ===" << std::endl;
    std::cout << std::endl;
    std::cout << "The Vector object (16 bytes):" << std::endl;
    std::cout << "  ┌─────────────────┐  <- On STACK" << std::endl;
    std::cout << "  │ elem (pointer)  │──────┐" << std::endl;
    std::cout << "  ├─────────────────┤      │" << std::endl;
    std::cout << "  │ sz (integer)    │      │" << std::endl;
    std::cout << "  └─────────────────┘      │" << std::endl;
    std::cout << "                           │" << std::endl;
    std::cout << "                           ▼" << std::endl;
    std::cout << "  ┌─────────────────┐  <- On HEAP (far away!)" << std::endl;
    std::cout << "  │   elem[0]       │" << std::endl;
    std::cout << "  ├─────────────────┤" << std::endl;
    std::cout << "  │   elem[1]       │" << std::endl;
    std::cout << "  ├─────────────────┤" << std::endl;
    std::cout << "  │   elem[2]       │" << std::endl;
    std::cout << "  ├─────────────────┤" << std::endl;
    std::cout << "  │      ...        │" << std::endl;
    std::cout << "  └─────────────────┘" << std::endl;
    std::cout << std::endl;
    std::cout << "KEY POINT:" << std::endl;
    std::cout << "• The POINTER 'elem' is part of the Vector representation" << std::endl;
    std::cout << "• The Vector object size is fixed: always 16 bytes" << std::endl;
    std::cout << "• Compiler knows this at compile time" << std::endl;
    std::cout << "• Elements can be anywhere on heap, but pointer is in object" << std::endl;
    std::cout << "• This is what 'representation is part of definition' means!" << std::endl;
    
    return 0;
}
