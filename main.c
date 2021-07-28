#include "ncc.h"

int lvars_size(LVar* lvar) {
    if( !lvar ) return 0;
    int size = -1;
    for (LVar* var = lvar; var; var = var->next) {
        if ( size < var->offset ) {
            size = (var->offset + 16) / 16 * 16;
        }
    }
    return size;
}

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

    int i = 0;
    while (code[i] != NULL) {
        printf("%s:\n", token_str(code[i]->token));

        printf("    endbr64\n");
        printf("    push rbp\n");
        printf("    mov rbp, rsp\n");
        if (lvars_size(code[i]->lvars) > 0 )
            printf("    sub rsp, %d\n", lvars_size(code[i]->lvars));

        for (int j = 0; j < code[i]->para; j++ ) {
            printf("    mov [rbp-%d], %s\n", j*8+8, &("rdi\0rsi\0rdx\0rcx\0r8 \0r9 \0"[j*4]));
        }

        for (int j = 0; code[i]->nodes[j]; j++) {
            gen(code[i]->nodes[j]);
            printf("    pop rax\n");
        }
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret\n");
        i++;
    }


    return 0;
}
