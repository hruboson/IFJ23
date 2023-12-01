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
int parse_statement(Token*, Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*, bool);
int parse_expression(Input*, SymbolTable*, Expression**, Token*, Token*, bool*);
int parse_statement_list(Token *,Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*, Statement *, bool);
