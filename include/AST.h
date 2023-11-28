#pragma once

#include "statement.h"

typedef struct AST {
    Statement* statement;
} AST;

void init_ast(AST*);
void clear_ast(AST*);
void ast_first(AST*, Statement*);
void ast_append(AST*, Statement*);
void ast_get(AST*, Statement*);
void ast_delete_last(AST*);

void print_ast(AST*);
