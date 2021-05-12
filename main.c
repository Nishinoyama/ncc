#include "ncc.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid argument. (except 1 argument(s))");
        return 1;
    }

    char* p = argv[1];
    tokenize(p);
    program();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 208\n");

    int i = 0;
    while (code[i] != NULL) {
        gen(code[i++]);
        printf("    pop rax\n");
    }

    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");

    return 0;
}
