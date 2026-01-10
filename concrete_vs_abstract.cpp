#include <iostream>

// ============================================================================
// VISUAL EXPLANATION: What "representation is part of definition" means
// ============================================================================

// CONCRETE TYPE: You can see the full representation
class Vector {
private:
    double* elem;  // 8 bytes - pointer is IN the Vector object
    int sz;        // 4 bytes - size is IN the Vector object
    
    // Total object size on stack: 16 bytes (with padding)
    // The POINTER is part of the object, even though it points to heap data
    
    // MEMORY LAYOUT:
    // Stack:               Heap:
    // ┌────────────┐      ┌────────────┐
    // │ elem (ptr) │────→ │  elem[0]   │
    // ├────────────┤      ├────────────┤
    // │  sz (int)  │      │  elem[1]   │
    // └────────────┘      ├────────────┤
    //   Vector object     │    ...     │
    //   (12-16 bytes)     └────────────┘
    //                     (sz * 8 bytes)
    
public:
    Vector(int s) : elem{new double[s]}, sz{s} {
        std::cout << "Vector object created at: " << this 
                  << " (size: " << sizeof(*this) << " bytes)" << std::endl;
        std::cout << "  elem pointer: " << elem 
                  << " (points to heap)" << std::endl;
        std::cout << "  sz value: " << sz << std::endl;
    }
    
    ~Vector() { delete[] elem; }
    int size() const { return sz; }
};

// ============================================================================
// WHY "REPRESENTATION IN DEFINITION" MATTERS
// ============================================================================

void show_memory_layout() {
    std::cout << "=== Memory Layout Demonstration ===" << std::endl << std::endl;
    
    // CONCRETE TYPE: Full size known at compile time
    std::cout << "Concrete type Vector:" << std::endl;
    std::cout << "  sizeof(Vector) = " << sizeof(Vector) << " bytes" << std::endl;
    std::cout << "  sizeof(double*) = " << sizeof(double*) << " bytes (elem pointer)" << std::endl;
    std::cout << "  sizeof(int) = " << sizeof(int) << " bytes (sz)" << std::endl;
    std::cout << std::endl;
    
    // Create Vector on stack
    std::cout << "Creating: Vector v(100);" << std::endl;
    Vector v(100);
    std::cout << std::endl;
    
    // The key insight:
    std::cout << "KEY INSIGHT:" << std::endl;
    std::cout << "  The Vector object (16 bytes) is on the STACK" << std::endl;
    std::cout << "  The elements (800 bytes) are on the HEAP" << std::endl;
    std::cout << "  But the POINTER to elements is INSIDE the Vector object!" << std::endl;
    std::cout << "  Compiler knows Vector is always 16 bytes." << std::endl;
    std::cout << std::endl;
}

// ============================================================================
// CONTRAST: Abstract type (representation NOT in definition)
// ============================================================================

class Shape {  // Abstract type
public:
    virtual void draw() = 0;  // Pure virtual
    virtual ~Shape() {}
    
    // You DON'T know the size of Shape!
    // Different derived classes have different sizes
    // sizeof(Shape) doesn't tell you the full story
};

class Circle : public Shape {
    double radius;  // Circle adds 8 bytes
public:
    Circle(double r) : radius{r} {}
    void draw() override { std::cout << "Circle" << std::endl; }
};

class Rectangle : public Shape {
    double width;   // Rectangle adds 16 bytes
    double height;
public:
    Rectangle(double w, double h) : width{w}, height{h} {}
    void draw() override { std::cout << "Rectangle" << std::endl; }
};

void show_abstract_type() {
    std::cout << "=== Abstract Type (No Fixed Representation) ===" << std::endl << std::endl;
    
    std::cout << "Abstract Shape:" << std::endl;
    std::cout << "  sizeof(Shape) = undefined (abstract class)" << std::endl;
    std::cout << "  sizeof(Circle) = " << sizeof(Circle) << " bytes" << std::endl;
    std::cout << "  sizeof(Rectangle) = " << sizeof(Rectangle) << " bytes" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Different derived classes have DIFFERENT sizes!" << std::endl;
    std::cout << "Can't allocate Shape on stack - don't know how much space needed." << std::endl;
    std::cout << std::endl;
    
    // Must use pointers/references
    Circle c(5.0);
    Rectangle r(10.0, 20.0);
    Shape* s1 = &c;  // Pointer to Shape
    Shape* s2 = &r;  // Same pointer type, different object sizes!
    
    std::cout << "Shape* s1 = &circle;     <- pointer is " << sizeof(Shape*) << " bytes" << std::endl;
    std::cout << "Shape* s2 = &rectangle;  <- pointer is " << sizeof(Shape*) << " bytes" << std::endl;
    std::cout << "But circle is " << sizeof(Circle) << " bytes, rectangle is " << sizeof(Rectangle) << " bytes!" << std::endl;
    std::cout << std::endl;
}

// ============================================================================
// THE 4 BENEFITS EXPLAINED WITH MEMORY DIAGRAMS
// ============================================================================

void show_four_benefits() {
    std::cout << "=== The 4 Benefits of Concrete Types ===" << std::endl << std::endl;
    
    // BENEFIT 1: Stack allocation
    std::cout << "1. STACK ALLOCATION:" << std::endl;
    std::cout << "   Vector v(5);  <- compiler allocates 16 bytes on stack" << std::endl;
    std::cout << "   Stack: [elem_ptr][sz] <- Fixed size, no heap needed for object" << std::endl;
    std::cout << std::endl;
    
    // BENEFIT 2: Direct reference
    std::cout << "2. DIRECT REFERENCE:" << std::endl;
    std::cout << "   Vector v(5);" << std::endl;
    std::cout << "   v[0] = 3.14;   <- 'v' is the actual object, not a pointer" << std::endl;
    std::cout << "   No need for: v->operator[](0)" << std::endl;
    std::cout << std::endl;
    
    // BENEFIT 3: Immediate initialization
    std::cout << "3. IMMEDIATE INITIALIZATION:" << std::endl;
    std::cout << "   Vector v(5);   <- Object is 100% ready after this line" << std::endl;
    std::cout << "   No separate .init() or .setup() needed" << std::endl;
    std::cout << "   Constructor guarantees invariants" << std::endl;
    std::cout << std::endl;
    
    // BENEFIT 4: Copy objects
    std::cout << "4. COPY OBJECTS:" << std::endl;
    std::cout << "   Vector v1(5);" << std::endl;
    std::cout << "   Vector v2 = v1;  <- Creates independent copy" << std::endl;
    std::cout << "   Both objects are self-contained" << std::endl;
    std::cout << std::endl;
}

// ============================================================================
// EFFICIENCY COMPARISON
// ============================================================================

void show_efficiency() {
    std::cout << "=== Efficiency: Concrete vs Abstract ===" << std::endl << std::endl;
    
    std::cout << "Array of 1000 Points (concrete type):" << std::endl;
    std::cout << "  Point points[1000];  <- ONE allocation, contiguous memory" << std::endl;
    std::cout << "  Size: " << (sizeof(int) * 2 * 1000) << " bytes in one block" << std::endl;
    std::cout << "  Cache-friendly: CPU can prefetch next elements" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Array of 1000 Shapes (abstract type):" << std::endl;
    std::cout << "  Shape* shapes[1000];  <- 1000 pointer allocations" << std::endl;
    std::cout << "  Size: " << (sizeof(Shape*) * 1000) << " bytes for pointers" << std::endl;
    std::cout << "        + scattered heap allocations for actual objects" << std::endl;
    std::cout << "  Not cache-friendly: objects scattered in memory" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Performance difference: 10-100x faster for concrete types!" << std::endl;
    std::cout << "  - One allocation vs thousands" << std::endl;
    std::cout << "  - Contiguous vs scattered memory" << std::endl;
    std::cout << "  - Direct access vs pointer indirection" << std::endl;
}

int main() {
    show_memory_layout();
    show_abstract_type();
    show_four_benefits();
    show_efficiency();
    
    std::cout << std::endl;
    std::cout << "=== SUMMARY ===" << std::endl;
    std::cout << "\"Representation is part of definition\" means:" << std::endl;
    std::cout << "  - Compiler knows EXACT size at compile time" << std::endl;
    std::cout << "  - All data members visible in class definition" << std::endl;
    std::cout << "  - Even if data is elsewhere, POINTERS are in the object" << std::endl;
    std::cout << "  - Enables stack allocation, direct access, copying" << std::endl;
    std::cout << "  - Results in optimal efficiency (time & space)" << std::endl;
    
    return 0;
}
