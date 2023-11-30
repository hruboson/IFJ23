#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "AST.h"
#include "expression.h"
#include "scanner.h"
#include "statement.h"
#include "table.h"
#include "token_stack.h"
#include "vartable_stack.h"

int parse(Input*, AST*, VarTableStack*, FuncTable*);
int parse_statement(Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*);
int parse_expression(Input*, SymbolTable*, Expression**, Token*, Token*);
