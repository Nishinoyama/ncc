#ifndef NCC_LVAR_H
#define NCC_LVAR_H

typedef struct LVar LVar;

struct LVar {
    LVar* next;
    char* name;
    int len;
    int offset;
};

extern LVar* local_vars;

LVar* find_lvar(Token*);
void new_lvar(Token*);

#endif //NCC_LVAR_H
