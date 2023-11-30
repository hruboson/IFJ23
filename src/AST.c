#include "AST.h"

#include "stdlib.h";

void init_ast(AST* ast) {
    ast->statement = NULL;
    init_symboltable(&ast->symtab);
}

void clear_ast(AST* ast) {
    if (ast->statement) {
        /*Statement* curr = ast->statement;
        while (curr) {
            Statement* tmp = curr->next;
            free(curr);
            curr = tmp;
        }*/
        ast->statement = NULL;
    }
    clear_symboltable(&ast->symtab);
}

void ast_append(AST* ast, Statement* st) {
    if (!(ast->statement)) { // adding first statement
        ast->statement = st;
        return;
    }

    Statement* curr = ast->statement;
    while (curr->next) {
        curr = curr->next;
    }

    curr->next = st;
}

void ast_get(AST* ast, Statement* st) {
    // todo
}

void print_ast(AST* ast) {
    // todo
    Statement* stm = ast->statement;
    while (stm) {
        stm = ast->statement->next;
    }
}
