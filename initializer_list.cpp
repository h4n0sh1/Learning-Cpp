#include <iostream>

class Vector {
private:
    double* elem;  // pointer to elements
    int sz;        // number of elements

public:
    // Constructor with member initializer list
    // Syntax: Constructor(params) : member1{value1}, member2{value2} { body }
    Vector(int s)
        : elem{new double[s]}, sz{s}  // <-- Member initializer list
    {
        // Constructor body (can be empty or do additional work)
        std::cout << "Vector of size " << sz << " created" << std::endl;
    }
    
    // This is DIFFERENT from assignment in the body:
    // Vector(int s) {
    //     elem = new double[s];  // This is ASSIGNMENT, not initialization
    //     sz = s;                // This is ASSIGNMENT, not initialization
    // }
    
    ~Vector() {
        delete[] elem;  // Clean up
    }
    
    int size() const { return sz; }
    
    double& operator[](int i) { return elem[i]; }
};

// Let's see why initializer lists matter with more examples:

class Point {
private:
    const int x;     // const member - MUST be initialized, can't be assigned
    int& y_ref;      // reference member - MUST be initialized
    int y_value;     // regular member

public:
    // This ONLY works with initializer list:
    Point(int x_val, int& y_val)
        : x{x_val}, y_ref{y_val}, y_value{y_val}  // Initialization
    {
        // x = x_val;        // ERROR! Can't assign to const
        // y_ref = y_val;    // ERROR! Reference already bound
        y_value = 100;       // OK - assignment works for regular members
    }
    
    void print() const {
        std::cout << "x=" << x << ", y_ref=" << y_ref 
                  << ", y_value=" << y_value << std::endl;
    }
};

class Base {
public:
    Base(int val) {
        std::cout << "Base constructor: " << val << std::endl;
    }
};

class Derived : public Base {
public:
    // Must use initializer list to call base class constructor
    Derived(int val) 
        : Base{val}  // Call base class constructor
    {
        std::cout << "Derived constructor" << std::endl;
    }
};

int main() {
    std::cout << "=== Member Initializer Lists ===" << std::endl << std::endl;
    
    // Example 1: Vector with initializer list
    std::cout << "1. Vector example:" << std::endl;
    Vector v(5);
    std::cout << std::endl;
    
    // Example 2: Point with const and reference members
    std::cout << "2. Point with const/reference (requires initializer list):" << std::endl;
    int y = 42;
    Point p(10, y);
    p.print();
    std::cout << std::endl;
    
    // Example 3: Derived class calling base constructor
    std::cout << "3. Derived class (initializer list calls base):" << std::endl;
    Derived d(99);
    std::cout << std::endl;
    
    // Key differences between initialization and assignment:
    std::cout << "=== Initialization vs Assignment ===" << std::endl;
    std::cout << "Initializer list:  : elem{new double[s]}, sz{s}" << std::endl;
    std::cout << "  - Direct initialization (like declaring: int x{5};)" << std::endl;
    std::cout << "  - Happens BEFORE constructor body runs" << std::endl;
    std::cout << "  - More efficient (one operation)" << std::endl;
    std::cout << "  - REQUIRED for const, references, base classes" << std::endl;
    std::cout << std::endl;
    std::cout << "Assignment in body: elem = new double[s]; sz = s;" << std::endl;
    std::cout << "  - Members first default-constructed" << std::endl;
    std::cout << "  - Then assigned new values (two operations)" << std::endl;
    std::cout << "  - Less efficient" << std::endl;
    std::cout << "  - Doesn't work for const/references" << std::endl;
    
    return 0;
}
