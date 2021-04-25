#ifndef NCC_PARSER_H
#define NCC_PARSER_H

#include "ncc.h"

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQU,
    ND_NEQ,
    ND_LES, // ND_GRT, = opposite ND_LES
    ND_LEQ, // ND_GEQ, = opposite ND_LEQ
    ND_NUM,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node* lhs;
    Node* rhs;
    int val;
};

/**
 * expr       = equality
 * equality   = relational ("==" relational | "!=" relational)*
 * relational = add ("<" add | ">" add | "<=" add | ">=" add)*
 * add        = mul ("+" mul | "-" mul )*
 * mul        = unary ("*" unary | "/" unary "-")*
 * unary      = ("+" | "-")? "*" primary | "/" primary "-")*
 * primary    = num | "(" expr ")"
 */

Node* expr();
Node* equality();
Node* relational();
Node* add();
Node* mul();
Node* unary();
Node* primary();

void gen();

#endif //NCC_PARSER_H
