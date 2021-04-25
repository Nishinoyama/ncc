#include "ncc.h"

bool start_with(char* p, char* q) {
    return memcmp(p, q, strlen(q)) == 0;
}

Token* new_token(TokenKind kind, int len, Token* cur, char* str) {
    Token* tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
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

        if (start_with(p, "<=") || start_with(p, ">=") ||
            start_with(p, "==") || start_with(p, "!=")) {
            cur = new_token(TK_RESERVED, 2, cur, p);
            p+=2;
            continue;
        }

        if (strchr("+-*/()<>", *p)) {
            cur = new_token(TK_RESERVED, 1, cur, p);
            p+=1;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUMBER, 0, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        ncc_error("Failed to tokenize, got `%c'", *p);
    }

    new_token(TK_EOF, 0, cur, p);
    return token = head.next;
}

int expect_number() {
    if (token->kind == TK_NUMBER) {
        int val = token->val;
        token = token->next;
        return val;
    } else {
        unexpected_token_error();
        return -1;
    }
}

void expect(char* op) {
    if (!consume(op)) {
        unexpected_token_error();
    }
}

bool consume(char* op) {
    if (token->kind == TK_RESERVED &&
        strlen(op) == token->len &&
        memcmp(token->str, op, token->len) == 0) {
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
