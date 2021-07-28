#include "ncc.h"

Function* new_function(Token* tok) {
    Function* func = calloc(1, sizeof(Function));
    func->token = tok;
    func->lvars = NULL;
    func->nodes = NULL;
    return func;
}
