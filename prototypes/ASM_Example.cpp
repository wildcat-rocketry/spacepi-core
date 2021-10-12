#include <stdio.h> // probably unneeded
int src = 1;
int dst;
void setup() {
    src = 4;
    dst = 5;
    __asm__(             // Assembly block start // OP RESULT PARAM
        "mov %0, %1\n"   // ASM intruction to move from register 1 (src) to 0 (dst)
        "addi %0, %1, 2" // Add 2 to register 1 (src) and store it in 0 (dst)
        : "=r"(dst)      //  %0  // non-constant register to c var
        : "r"(src)       //   %1  // constant register to c var
    );                   // end of ASM
    printf("%d\n", dst); // Print DST then a new line
}
void loop() {
}