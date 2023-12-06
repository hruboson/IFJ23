#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "AST.h"
#include "expression.h"
#include "scanner.h"
#include "vartable_stack.h"
#include "statement.h"
#include "token_stack.h"
#include "table.h"
#include "rules.h"

extern bool do_semantic_analysis;

int parse(Input*, AST*);
int parse_statement(Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*, Statement*, size_t);
int parse_expression(Input*, SymbolTable*, Expression**, Token* in[2], Token* out_token, bool* out_token_returned);
int parse_statement_list(Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*, Statement*, Statement*, size_t);
int parse_parameters(Input*, SymbolTable*, Statement*);
void insert_builtin_functions(FuncTable*, SymbolTable*);
