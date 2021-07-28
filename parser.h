#ifndef NCC_PARSER_H
#define NCC_PARSER_H

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
    ND_FNC, // FUNCTION
    ND_RET, // return
    ND_IFJ, // if then jump
    ND_IFE, // if else
    ND_WHL, // while
    ND_FOR,
    ND_BLK, // block
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node* lhs;
    Node* rhs;
    int val;
    int offset;
    Token* token;
};

extern int if_stmt_cnt;

/**
 * program    = declare*
 * declare    = ident "()" "{" stmt* "}"
 * stmt       = expr ";"
 *              | "{" stmt* "}"
 *              | "if" "(" expr ")" stmt ("else" stmt)?
 *              | "while" "(" expr ")" stmt
 *              | "for" "(" expr? ";" expr? ";" expr? ")" stmt
 *              | "return" expr ";"
 * expr       = assign
 * assign     = equality ("=" assign)?
 * equality   = relational ("==" relational | "!=" relational)*
 * relational = add ("<" add | ">" add | "<=" add | ">=" add)*
 * add        = mul ("+" mul | "-" mul )*
 * mul        = unary ("*" unary | "/" unary "-")*
 * unary      = ("+" | "-")? "*" primary | "/" primary "-")*
 * primary    = num | ident ("(" (expr ("," expr)*)? ")")? | "(" expr ")"
 */

void program();
void declare();
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
