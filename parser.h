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
    ND_ASN, // ASSIGN
    ND_LVR, // LOCAL VARIABLE
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node* lhs;
    Node* rhs;
    int val;
    int offset;
};

Node* code[100];

/**
 * program    = stmt*
 * stmt       = expr ";"
 * expr       = assign
 * assign     = equality ("=" assign)?
 * equality   = relational ("==" relational | "!=" relational)*
 * relational = add ("<" add | ">" add | "<=" add | ">=" add)*
 * add        = mul ("+" mul | "-" mul )*
 * mul        = unary ("*" unary | "/" unary "-")*
 * unary      = ("+" | "-")? "*" primary | "/" primary "-")*
 * primary    = num | ident | "(" expr ")"
 */

void program();
Node* stmt();
Node* expr();
Node* assign();
Node* equality();
Node* relational();
Node* add();
Node* mul();
Node* unary();
Node* primary();

void gen(Node*);
void gen_local_val(Node*);

#endif //NCC_PARSER_H
