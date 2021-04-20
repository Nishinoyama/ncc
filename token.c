#include "ncc.h"

Token* new_token(TokenKind kind, Token* cur, char* str) {
    Token* tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

Token* tokenize(char* p) {
    Token head;
    head.next = NULL;
    Token* cur = &head;
    user_input = p;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
            cur = new_token(TK_RESERVED, cur, p);
            p++;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUMBER, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        ncc_error("Failed to tokenize, got `%c'", *p);
    }

    new_token(TK_EOF, cur, p);
    return token = head.next;
}

int expect_number() {
    if (token->kind == TK_NUMBER) {
        int val = token->val;
        token = token->next;
        return val;
    } else {
        unexpected_token_error();
    }
}

void expect(char op) {
    if (token->kind != TK_RESERVED || *token->str != op) {
        unexpected_token_error();
    }
    token = token->next;
}

bool consume(char op) {
    if (token->kind == TK_RESERVED && *token->str == op) {
        token = token->next;
        return true;
    }
    return false;
}

bool at_eof_token() {
    return token->kind == TK_EOF;
}

void unexpected_token_error() {
    int pos = token->str - user_input;
    fprintf(stderr, "%s\n", user_input);
    if(pos > 0) fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^\n");
    ncc_error("Unexpected Token `%d'\n", token->kind);
}
