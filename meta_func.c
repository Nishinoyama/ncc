#include <stdio.h>
int foo() {
    printf("calling foo() is OK\n");
    return 0;
}

int return_3_function() {
    return 3;
}

int bar(int a, int b) {
    printf("%d - %d = %d\n", a, b, a - b);
    return a - b;
}

int vor() {
    return bar(6, 3);
}

int barbar(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int vorvor() {
    return barbar(1, 2, 3, 4, 5, 6);
}

