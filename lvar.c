#include "ncc.h"

LVar* local_vars;

LVar* find_lvar(Token* tok) {
    for (LVar* var = local_vars; var; var = var->next) {
        if (var->len == tok->len && !memcmp(var->name, tok->str, var->len)) {
            return var;
        }
    }
    new_lvar(tok);
    return local_vars;
}

void new_lvar(Token* tok) {
    LVar* var = calloc(1, sizeof(LVar));
    var->next = local_vars;
    var->name = tok->str;
    var->len = tok->len;
    var->offset = local_vars == NULL ? 8 : local_vars->offset+8;
    local_vars = var;
}

int lvar_size(LVar** lvar) {
    int count = 0;
    for (LVar* var = local_vars; var; var = var->next) {
        count++;
    }
    return count;
}
