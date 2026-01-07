// C functions that will be called from C++
// No special syntax needed - C doesn't do name mangling

struct Point {
    int x;
    int y;
};

int add_numbers(int a, int b) {
    return a + b;
}

double calculate_average(double* array, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}

void modify_struct(struct Point* p) {
    // Directly manipulate the struct - same binary layout
    p->x *= 2;
    p->y *= 2;
}
