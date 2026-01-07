// Assembly functions for ARM64 (Apple Silicon)
// On ARM64: arguments in x0-x7 (64-bit) or w0-w7 (32-bit)
// Return value in x0 (or w0 for 32-bit)

.global _asm_add          // macOS prepends underscore to symbol names
.global _asm_multiply
.global _asm_swap

.text
.align 2

// int asm_add(int a, int b)
// a is in w0 (32-bit register)
// b is in w1 (32-bit register)
_asm_add:
    add     w0, w0, w1    // Add w0 = w0 + w1
    ret                    // Return (value in w0)

// long asm_multiply(long a, long b)
// a is in x0 (64-bit register)
// b is in x1 (64-bit register)
_asm_multiply:
    mul     x0, x0, x1    // Multiply x0 = x0 * x1
    ret                    // Return (value in x0)

// void asm_swap(int* a, int* b)
// a is in x0 (pointer)
// b is in x1 (pointer)
_asm_swap:
    ldr     w2, [x0]      // Load value at *a into w2
    ldr     w3, [x1]      // Load value at *b into w3
    str     w3, [x0]      // Store w3 to *a
    str     w2, [x1]      // Store w2 to *b
    ret
