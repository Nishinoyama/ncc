#include "ncc.h"

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
    if (consume_tk(TK_RETURN)) {
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
        Node* node = calloc(1, sizeof(struct Node));
        node->kind = ND_LVR;
        node->offset = find_lvar(tok)->offset;
        return node;
    }
    return new_node_num(expect_number());
}

void gen(Node* node) {
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

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

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
        case ND_NUM:
        case ND_LVR:
        default:
            ncc_error("Non-implemented node: %d", node->kind);
            break;
    }

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
