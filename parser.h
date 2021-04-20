#ifndef NCC_PARSER_H
#define NCC_PARSER_H

#include "ncc.h"

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
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
 * expr    = mul ("+" mul | "-" mul )*
 * mul     = primary ("*" primary | "/" primary "-")*
 * primary = num | "(" expr ")"
 */

Node* expr();
Node* mul();
Node* primary();

void gen();

#endif //NCC_PARSER_H
