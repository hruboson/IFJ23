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

int parse(Input*, AST*);
int parse_statement(Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*, bool);
int parse_expression(Input*, SymbolTable*, Expression**, Token*, Token*, bool*);
int parse_statement_list(Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*, Statement *, bool);
int parse_parameters(Input*, SymbolTable*, Statement**);
