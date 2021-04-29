#ifndef NCC_TOKEN_H
#define NCC_TOKEN_H

#include <stdbool.h>
typedef enum {
    TK_RESERVED,
    TK_INDENT,
    TK_NUMBER,
    TK_EOF, // end of token
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token* next;
    int val;
    char* str;
    int len; // token strlen
};

Token* token;
char* user_input;

Token* tokenize(char*);
int expect_number();
void expect(char*);
bool consume(char*);
Token* consume_ident();
bool at_eof_token();
void unexpected_token_error();

#endif //NCC_TOKEN_H
