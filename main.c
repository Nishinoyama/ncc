#include <stdio.h>
#include "ncc.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invaild argument. (except 1 argument(s))");
        return 1;
    }

    char* p = argv[1];
    tokenize(p);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("    mov rax, %d\n", expect_number());

    while (!at_eof_token())
    {
        // fprintf(stderr, "TOKEN:: %d: %c\n", token->kind, *token->str);
        if (consume('+')) {
            printf("    add rax, %d\n", expect_number());
            continue;
        }
        if (consume('-')) {
            printf("    sub rax, %d\n", expect_number());
            continue;
        }
        unexpected_token_error();
    }

    printf("    ret\n");

    return 0;
}
