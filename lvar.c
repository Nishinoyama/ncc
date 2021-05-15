#include "ncc.h"

LVar* local_vars;

LVar* find_lvar(Token* tok) {
    for (LVar* var = local_vars; var; var = var->next) {
        if (var->len == tok->len && !memcmp(var->name, tok->str, var->len)) {
            return var;
        }
    }
    return NULL;
}

void new_lvar(Token* tok) {
    LVar* var = calloc(1, sizeof(LVar));
    var->next = local_vars;
    var->name = tok->str;
    var->len = tok->len;
    var->offset = local_vars == NULL ? 0 : local_vars->offset+8;
    local_vars = var;
}
