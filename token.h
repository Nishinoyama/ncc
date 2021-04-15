#include <stdbool.h>
typedef enum {
    TK_RESERVED,
    TK_NUMBER,
    TK_EOF, // end of token
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token* next;
    int val;
    char* str;
};

Token* token;

Token* tokenize(char*);
int expect_number();
bool consume(char);
bool at_eof_token();
void unexpected_token_error();
