#include <iostream>
#include <string>
#include <memory>

// ============================================================================
// CONCRETE TYPE vs ABSTRACT TYPE
// ============================================================================

// CONCRETE TYPE: Representation is part of the definition
// You can see ALL the data members - you know the exact size at compile time
class Vector {
private:
    double* elem;  // Pointer to elements (stored elsewhere on heap)
    int sz;        // Size - stored IN the object
    
    // Representation = elem pointer + sz integer
    // Size = sizeof(double*) + sizeof(int) = 8 + 4 = 12 bytes (plus padding)
    // Even though elements are on heap, the POINTER is in the object!
    
public:
    Vector(int s) : elem{new double[s]}, sz{s} {}
    ~Vector() { delete[] elem; }
    
    int size() const { return sz; }
    double& operator[](int i) { return elem[i]; }
};

// Another CONCRETE TYPE
class Point {
public:
    int x;         // Stored IN the object
    int y;         // Stored IN the object
    
    // Representation = two ints
    // Size = sizeof(int) + sizeof(int) = 4 + 4 = 8 bytes
    
    Point() : x{0}, y{0} {}  // Default constructor
    Point(int x, int y) : x{x}, y{y} {}
};

// ABSTRACT TYPE: Representation is NOT in the definition
// Uses pure virtual functions - can't know size without knowing derived class
class Shape {  // Abstract base class
public:
    virtual void draw() = 0;  // Pure virtual - no implementation here
    virtual double area() = 0;
    virtual ~Shape() {}
    
    // You DON'T know what data is in Shape!
    // Different derived classes have different representations
    // Can only use pointers/references to Shape, never direct objects
};

class Circle : public Shape {
    double radius;  // Circle's representation
public:
    Circle(double r) : radius{r} {}
    void draw() override { std::cout << "Drawing circle" << std::endl; }
    double area() override { return 3.14159 * radius * radius; }
};

// ============================================================================
// CHARACTERISTIC 1: Place objects on stack, static memory, or in other objects
// ============================================================================

void demonstrate_placement() {
    std::cout << "=== 1. Object Placement ===" << std::endl << std::endl;
    
    // ON THE STACK (automatic storage)
    // Because compiler knows exact size of Vector (pointer + int)
    Vector v1(5);  // Lives on stack - automatically destroyed when scope ends
    Point p1(10, 20);  // Also on stack
    
    std::cout << "Stack objects: Vector size=" << sizeof(Vector) 
              << " bytes, Point size=" << sizeof(Point) << " bytes" << std::endl;
    
    // IN STATIC MEMORY (global/static lifetime) = DATA
    static Vector v2(3);  // Allocated in static memory at program start
    static Point p2(0, 0);
    
    std::cout << "Static objects created (live for entire program)" << std::endl;
    
    // INSIDE OTHER OBJECTS (composition)
    class Line {
        Point start;  // Point object embedded INSIDE Line
        Point end;    // Another Point object embedded INSIDE Line
    public:
        Line(Point s, Point e) : start{s}, end{e} {}
    };
    
    Line line(Point(0, 0), Point(10, 10));
    std::cout << "Line contains 2 Point objects: size=" << sizeof(Line) << " bytes" << std::endl;
    std::cout << "  (2 Points × " << sizeof(Point) << " bytes each)" << std::endl;
    
    // CONTRAST WITH ABSTRACT TYPE - must use pointer/reference
    // Shape s;  // ERROR! Can't create Shape object - it's abstract
    Circle c(5.0);
    Shape* s_ptr = &c;  // Can only use POINTER to abstract type
    
    std::cout << std::endl << "Abstract types require pointers/references" << std::endl;
    std::cout << "  Circle object size: " << sizeof(Circle) << " bytes" << std::endl;
    std::cout << "  Shape pointer size: " << sizeof(Shape*) << " bytes" << std::endl;
    
    std::cout << std::endl;
}

// ============================================================================
// CHARACTERISTIC 2: Refer to objects directly (not through pointers)
// ============================================================================

void demonstrate_direct_reference() {
    std::cout << "=== 2. Direct Object Reference ===" << std::endl << std::endl;
    
    // CONCRETE TYPE: Direct object
    Vector v(10);
    v[0] = 3.14;  // Direct access - no pointer dereferencing needed
    
    Point p(5, 5);
    p.x = 10;     // Direct member access
    
    std::cout << "Concrete types: direct access without pointers" << std::endl;
    std::cout << "  Vector v; v[0] = 3.14;  <- v is the actual object" << std::endl;
    std::cout << "  Point p; p.x = 10;      <- p is the actual object" << std::endl;
    
    // ABSTRACT TYPE: Must use pointer/reference
    Circle c(5.0);
    Shape& s_ref = c;     // Reference to abstract type
    Shape* s_ptr = &c;    // Pointer to abstract type
    
    // Shape s = c;       // ERROR! Can't copy abstract type directly
    
    std::cout << std::endl << "Abstract types: must use pointer/reference" << std::endl;
    std::cout << "  Shape* s_ptr = &c;  <- need pointer" << std::endl;
    std::cout << "  Shape& s_ref = c;   <- or reference" << std::endl;
    
    std::cout << std::endl;
}

// ============================================================================
// CHARACTERISTIC 3: Initialize immediately and completely
// ============================================================================

void demonstrate_initialization() {
    std::cout << "=== 3. Immediate and Complete Initialization ===" << std::endl << std::endl;
    
    // CONCRETE TYPE: Constructed completely in one step
    Vector v1(5);  // Constructor runs, object is fully initialized
    // After this line, v1 is 100% ready to use
    
    Point p1(10, 20);  // Fully initialized with constructor
    // p1.x and p1.y both have values - object is complete
    
    std::cout << "Concrete types initialized completely:" << std::endl;
    std::cout << "  Vector v(5);        <- fully ready to use" << std::endl;
    std::cout << "  Point p(10, 20);    <- all members initialized" << std::endl;
    
    // No need for two-step initialization:
    // Vector v;        // Step 1: create
    // v.initialize(5); // Step 2: initialize (BAD PATTERN)
    
    // ABSTRACT TYPE: Can't construct directly
    // Shape s;  // ERROR! Abstract type
    
    // Must construct derived class, then refer through base pointer
    Circle* c_ptr = new Circle(5.0);  // Two-step process in a way
    Shape* s_ptr = c_ptr;              // Indirection needed
    
    std::cout << std::endl << "Abstract types need indirection:" << std::endl;
    std::cout << "  Circle* c = new Circle(5.0);  <- heap allocation" << std::endl;
    std::cout << "  Shape* s = c;                 <- indirect reference" << std::endl;
    
    delete c_ptr;
    std::cout << std::endl;
}

// ============================================================================
// CHARACTERISTIC 4: Copy objects
// ============================================================================

void demonstrate_copying() {
    std::cout << "=== 4. Copy Objects ===" << std::endl << std::endl;
    
    // CONCRETE TYPE: Can copy directly
    Point p1(10, 20);
    Point p2 = p1;     // Copy construction - creates independent copy
    Point p3(0, 0);
    p3 = p1;           // Copy assignment - copies values
    
    std::cout << "Concrete types can be copied:" << std::endl;
    std::cout << "  Point p2 = p1;  <- copy construction" << std::endl;
    std::cout << "  p3 = p1;        <- copy assignment" << std::endl;
    
    p2.x = 99;  // Modifying p2 doesn't affect p1 - they're independent
    std::cout << "  After p2.x = 99: p1.x=" << p1.x << ", p2.x=" << p2.x 
              << " (independent)" << std::endl;
    
    // Vector can also be copied (if copy constructor/assignment defined)
    Vector v1(5);
    v1[0] = 3.14;
    // Vector v2 = v1;  // Would need proper copy constructor (deep copy)
    
    // ABSTRACT TYPE: Can't copy directly
    Circle c1(5.0);
    // Shape s = c1;    // ERROR! Can't slice abstract type
    
    Shape* s_ptr = &c1;
    // Shape s2 = *s_ptr;  // ERROR! Still can't copy abstract type
    
    std::cout << std::endl << "Abstract types can't be copied directly:" << std::endl;
    std::cout << "  Shape s = circle;  <- ERROR (object slicing)" << std::endl;
    
    std::cout << std::endl;
}

// ============================================================================
// WHY THIS MATTERS: Efficiency
// ============================================================================

void demonstrate_efficiency() {
    std::cout << "=== Why Concrete Types Are Efficient ===" << std::endl << std::endl;
    
    // MEMORY LAYOUT
    std::cout << "Memory layout comparison:" << std::endl;
    std::cout << "  Concrete Vector: [elem_ptr][sz] <- 16 bytes on stack" << std::endl;
    std::cout << "  Abstract Shape*: [vtable_ptr][derived_data] <- heap allocation needed" << std::endl;
    std::cout << std::endl;
    
    // STACK vs HEAP
    std::cout << "Allocation comparison:" << std::endl;
    
    // Concrete: Stack allocation (fast!)
    Point p(10, 20);  // Just move stack pointer - nanoseconds
    std::cout << "  Point p(10,20);              <- stack (very fast)" << std::endl;
    
    // Abstract: Heap allocation (slower)
    Circle* c = new Circle(5.0);  // System call, memory management overhead
    std::cout << "  Circle* c = new Circle(5);   <- heap (slower, fragmentation)" << std::endl;
    delete c;
    
    std::cout << std::endl;
    
    // ARRAY EXAMPLE
    std::cout << "Array efficiency:" << std::endl;
    
    // Concrete: Contiguous memory, cache-friendly
    Point points[1000];  // All 1000 Points in one contiguous block
    std::cout << "  Point points[1000];          <- contiguous, cache-friendly" << std::endl;
    
    // Abstract: Array of pointers, scattered memory
    Shape* shapes[1000];  // 1000 pointers, actual objects scattered on heap
    std::cout << "  Shape* shapes[1000];         <- pointers, scattered data" << std::endl;
}

int main() {
    std::cout << "=== Concrete Types: Definition and Characteristics ===" << std::endl;
    std::cout << std::endl;
    std::cout << "A CONCRETE TYPE has its representation in its definition." << std::endl;
    std::cout << "Even if data is stored elsewhere (heap), the POINTER is in the object." << std::endl;
    std::cout << "Compiler knows exact size at compile time." << std::endl;
    std::cout << std::endl;
    
    demonstrate_placement();
    demonstrate_direct_reference();
    demonstrate_initialization();
    demonstrate_copying();
    demonstrate_efficiency();
    
    std::cout << "=== Summary ===" << std::endl;
    std::cout << "Concrete types allow:" << std::endl;
    std::cout << "  1. Stack/static/embedded allocation (no heap required)" << std::endl;
    std::cout << "  2. Direct object usage (no pointer indirection)" << std::endl;
    std::cout << "  3. Complete initialization at construction time" << std::endl;
    std::cout << "  4. Value semantics (copying works naturally)" << std::endl;
    std::cout << "  5. Optimal efficiency (minimal overhead)" << std::endl;
    
    return 0;
}
