#include <stdio.h>
#include "ncc.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid argument. (except 1 argument(s))");
        return 1;
    }

    char* p = argv[1];
    tokenize(p);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    gen(expr());

    printf("    pop rax\n");
    printf("    ret\n");

    return 0;
}
