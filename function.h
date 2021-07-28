#ifndef NCC_FUNCTION_H
#define NCC_FUNCTION_H

typedef struct {
    Token* token;
    LVar* lvars;
    Node** nodes;
} Function;

extern Function* function;
extern Function* code[100];

Function* new_function(Token*);
void function_new_lvar(Function*, Token*);

#endif //NCC_FUNCTION_H
