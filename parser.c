#include "ncc.h"

Node* code[100];
int if_stmt_cnt = 0;

Node* new_node(NodeKind kind, Node* lhs, Node* rhs) {
    Node* node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node* new_node_num(int val) {
    Node* node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

void program() {
    int i = 0;
    while (!at_eof_token()) {
        code[i++] = stmt();
    }
    code[i] = NULL;
}

Node* stmt() {
    Node* node;
    if (consume("{")) {
        Node* seek = node = new_node(ND_BLK, NULL, NULL);
        while (1) {
            if (consume("}")) {
                return node;
            }
            seek->lhs = stmt();
            seek->rhs = new_node(ND_BLK, NULL, NULL);
            seek = seek->rhs;
        }
    } else if (consume("if")) {
        expect("(");
        node = new_node(ND_IFJ, expr(), NULL);
        expect(")");
        node->rhs = stmt();
        if (consume("else")) {
            node->rhs = new_node(ND_IFE, node->rhs, stmt());
        }
        return node;
    } else if (consume("while")) {
        expect("(");
        node = new_node(ND_WHL, expr(), NULL);
        expect(")");
        node->rhs = stmt();
        return node;
    } else if (consume("for")) {
        expect("(");
        node = new_node(ND_FOR, new_node(ND_FOR, NULL, NULL), NULL);
        if (!consume(";")) {
            node->lhs = new_node(ND_FOR, expr(), NULL);
            expect(";");
        }
        node->lhs->rhs = new_node(ND_FOR, NULL, NULL);
        if (!consume(";")) {
            node->lhs->rhs->lhs = expr();
            expect(";");
        }
        if (!consume(")")) {
            node->lhs->rhs->rhs = expr();
            expect(")");
        }
        node->rhs = stmt();
        return node;
    } else if (consume("return")) {
        node = new_node(ND_RET, expr(), NULL);
    } else {
        node = expr();
    }
    expect(";");
    return node;
}

Node* expr() {
    return assign();
}

Node* assign() {
    Node* node = equality();
    if (consume("=")) {
       node = new_node(ND_ASN, node, assign());
    }
    return node;
}

Node* equality() {
    Node* node = relational();

    while (1) {
        if (consume("=="))
            node = new_node(ND_EQU, node, relational());
        else if (consume("!="))
            node = new_node(ND_NEQ, node, relational());
        else
            return node;
    }
}

Node* relational() {
    Node* node = add();

    while (1) {
        if (consume("<="))
            node = new_node(ND_LEQ, node, add());
        else if (consume(">="))
            node = new_node(ND_LEQ, add(), node);
        else if (consume("<"))
            node = new_node(ND_LES, node, add());
        else if (consume(">"))
            node = new_node(ND_LES, add(), node);
        else
            return node;
    }
}

Node* add() {
    Node* node = mul();

    while (1) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

Node* mul() {
    Node* node = unary();

    while (1) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

Node* unary() {
    if (consume("+"))
        return primary();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), primary());
    return primary();
}

Node* primary() {
    if (consume("(")) {
        Node* node = expr();
        expect(")");
        return node;
    }
    Token* tok = consume_ident();
    if (tok) {
        Node* node = calloc(1, sizeof(Node));
        if (consume("(")) {
            node->token = tok;
            node->kind = ND_FNC;
            Node* seek = node;
            while (!consume(")")) {
                seek->lhs = expr();
                seek->rhs = calloc(1, sizeof(Node));
                seek = seek->rhs;
                if (!consume(",")) {
                    expect(")");
                    break;
                }
            }
        } else {
            node->offset = find_lvar(tok)->offset;
            node->kind = ND_LVR;
        }
        return node;
    }
    return new_node_num(expect_number());
}

void gen(Node* node) {

    // leaf node
    if (node->kind == ND_NUM) {
        printf("    push %d\n", node->val);
        return;
    }
    if (node->kind == ND_LVR) {
        gen_local_val(node);
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    }
    if (node->kind == ND_FNC) {
        Node* seek = node;
        int i;
        for ( i = 0; seek->rhs; i++, seek = seek->rhs ) {
            gen(seek->lhs);
        }
        for ( int j = 1; j <= i; j++ ) {
            printf("    pop %s\n", &("rdi\0rsi\0rdx\0rcx\0r8 \0r9 \0"[(i-j)*4]));
        }
        printf("    call %s\n", token_str(node->token));
        printf("    push rax\n");
        return;
    }
    if (node->kind == ND_ASN) {
        gen_local_val(node->lhs);
        gen(node->rhs);
        printf("    pop rdi\n");
        printf("    pop rax\n");
        printf("    mov [rax], rdi\n");
        printf("    push rdi\n");
        return;
    }
    if (node->kind == ND_RET) {
        gen(node->lhs);
        printf("    pop rax\n");
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret\n");
        return;
    }
    // leaf node end

    // statement node
    if (node->kind == ND_IFJ) {
        int cnt = if_stmt_cnt++;
        gen(node->lhs);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        if (node->rhs->kind == ND_IFE) {
            Node* else_node = node->rhs;
            printf("    je  .Lelse%03d\n", cnt);
            gen(else_node->lhs);
            printf("    jmp .Lend%03d\n", cnt);
            printf(".Lelse%03d:\n", cnt);
            gen(else_node->rhs);
        } else {
            printf("    je  .Lend%03d\n", cnt);
            gen(node->rhs);
        }
        printf(".Lend%03d:\n", cnt);
        return;
    }
    if (node->kind == ND_WHL) {
        int cnt = if_stmt_cnt++;
        printf(".Lbegin%03d:\n", cnt);
        gen(node->lhs);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je  .Lend%03d\n", cnt);
        gen(node->rhs);
        printf("    pop rax\n");
        printf("    jmp .Lbegin%03d\n", cnt);
        printf(".Lend%03d:\n", cnt);
        return;
    }
    if (node->kind == ND_FOR) {
        int cnt = if_stmt_cnt++;
        Node* init_clause = node->lhs->lhs;
        Node* cond_expression = node->lhs->rhs->lhs;
        Node* iteration_expression = node->lhs->rhs->rhs;
        if(init_clause) {
            gen(init_clause);
            printf("    pop rax\n");
        }
        printf(".Lbegin%03d:\n", cnt);
        if(cond_expression)
            gen(cond_expression);
        else
            printf("    push 1\n");
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je  .Lend%03d\n", cnt);
        gen(node->rhs);
        printf("    pop rax\n");
        if (iteration_expression) {
            gen(iteration_expression);
            printf("    pop rax\n");
        }
        printf("    jmp .Lbegin%03d\n", cnt);
        printf(".Lend%03d:\n", cnt);
        return;
    }
    if (node->kind == ND_BLK) {
        if ( node->lhs == NULL ) {
            return;
        }
        gen(node->lhs);
        printf("    pop rax\n");
        gen(node->rhs);
        printf("    push 0\n");
        return;
    }
    // statement node end

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    // stem node
    switch (node->kind) {
        case ND_ADD:
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
        case ND_EQU:
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_NEQ:
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LES:
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LEQ:
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_ASN:
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
        default:
            ncc_error("Non-implemented node: %d", node->kind);
            break;
    }
    // stem node end

    printf("    push rax\n");
}

void gen_local_val(Node* node) {
    if (node->kind != ND_LVR) {
        ncc_error("the lhs is not modifiable lvalue");
    }
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}
