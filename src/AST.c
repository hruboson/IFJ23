#include "AST.h"

#include "stdlib.h";

void init_ast(AST* ast) {
    ast->statement = NULL;
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
}

void ast_append(AST* ast, Statement* st) {
    Statement* curr = ast->statement;

    if (!curr) {
        ast->statement = st;
        return;
    }
	
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
        Statement* prev = ast->statement;
        while (curr->next) {
			prev = curr;
            curr = curr->next;
        }
		prev->next = NULL;
    }
}

void print_ast(AST* ast) {
    // todo
    Statement* stm = ast->statement;
    while (stm) {
        stm = ast->statement->next;
    }
}
