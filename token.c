#include "ncc.h"

Token* token;
char* user_input;

bool start_with(char* p, char* q) {
    return memcmp(p, q, strlen(q)) == 0;
}

bool is_alnum_or_under(char c) {
    return isalnum(c) || c == '_';
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

        if (start_with(p, "if") && !is_alnum_or_under(p[2])) {
            cur = new_token(TK_RESERVED, 2, cur, p);
            p += 2;
            continue;
        }

        if (start_with(p, "else") && !is_alnum_or_under(p[4])) {
            cur = new_token(TK_RESERVED, 4, cur, p);
            p += 4;
            continue;
        }

        if (start_with(p, "while") && !is_alnum_or_under(p[5])) {
            cur = new_token(TK_RESERVED, 5, cur, p);
            p += 5;
            continue;
        }

        if (start_with(p, "for") && !is_alnum_or_under(p[3])) {
            cur = new_token(TK_RESERVED, 3, cur, p);
            p += 3;
            continue;
        }

        if (start_with(p, "return") && !is_alnum_or_under(p[6])) {
            cur = new_token(TK_RESERVED, 6, cur, p);
            p += 6;
            continue;
        }

        if (start_with(p, "<=") || start_with(p, ">=") ||
            start_with(p, "==") || start_with(p, "!=")) {
            cur = new_token(TK_RESERVED, 2, cur, p);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>=;{},&", *p)) {
            cur = new_token(TK_RESERVED, 1, cur, p);
            p += 1;
            continue;
        }

        if (isalpha(*p) || *p == '_') {
            int len = 0;
            cur = new_token(TK_INDENT, 1, cur, p);
            while (isalnum(*p) || *p == '_') {
                len++;
                p += 1;
            }
            cur->len = len;
            // if (find_lvar(cur) == NULL) {
            //     new_lvar(cur);
            // }
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
    if (token->kind == TK_NUMBER || token->kind == TK_INDENT) {
        int val = token->val;
        token = token->next;
        return val;
    } else {
        unexpected_token_error();
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

Token* consume_ident() {
    if (token->kind == TK_INDENT) {
        Token* tmp = token;
        token = token->next;
        return tmp;
    }
    return NULL;
}

bool consume_tk(TokenKind tk) {
    if (token->kind == tk) {
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
    ncc_error("Unexpected Token `%d'\nToken name \"%s\", len=%d", token->kind, token->str, token->len);
}

char* token_str(Token* tk) {
    char* str = (char*) calloc(1, tk->len+1);
    return (char*) memcpy(str, tk->str, tk->len);
    // TODO: avoid memory leaking
}
