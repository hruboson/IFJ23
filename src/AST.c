#include "AST.h"
#include "stdlib.h";

void init_ast(AST* ast) {
    ast->statement = NULL;
}

void clear_ast(AST* ast) {
    if (ast->statement) {
        Statement* next = ast->statement;
        while (next) {
            Statement* tmp = next->next;
            free(next);
            next = tmp;
        }
    }
}

void ast_append(AST* ast, Statement* st) {
    Statement* curr = ast->statement;
    while (curr->next) {
        curr = curr->next;
    }

    curr->next = st;
}

void ast_first(AST* ast, Statement* st) {
    st = ast->statement;
}

void ast_get(AST* ast, Statement* st) {
    // todo
}

void ast_delete_last(AST* ast) {
    if (ast->statement) {
        Statement* curr = ast->statement;
        while (curr->next) {
            curr = curr->next;
        }
        free(curr);
    }
}

void print_ast(AST* ast) {
    // todo
    Statement* stm = ast->statement;
    while (stm) {
        stm = ast->statement->next;
    }
}
