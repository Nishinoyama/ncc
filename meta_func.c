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
