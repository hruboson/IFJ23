/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
 * xsatin03 Ondřej Šatinský
 * xjestr04 Radek Jestřabík
 * xborek12 Dominik Borek
*/

#pragma once

#include "statement.h"
#include "symtable.h"

typedef struct AST {
	SymbolTable symtab;
	Statement* statement;
} AST;

void print_exp(const Expression*);

void init_ast(AST*);
void clear_ast(AST*);
void ast_first(AST*, Statement*);
void ast_append(AST*, Statement*);
void ast_get(AST*, Statement*);
void ast_delete_last(AST*);

void print_ast(AST*);
void print_statement(Statement*, size_t);
