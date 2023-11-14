#pragma once

#include <stdio.h>

#include "AST.h"
#include "expression.h"
#include "scanner.h"
#include "statement.h"
#include "token_stack.h"

int parse(FILE*, AST*);
int parse_statement(FILE*, TokenStack*, Statement**);
int parse_expression(FILE*, TokenStack*, Expression**);
