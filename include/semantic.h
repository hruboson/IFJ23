#pragma once

#include "AST.h"
#include <stdio.h>
#include "vartable_stack.h"

// int semantic( Input*, AST* );

bool semantic_variable(VarTableStack* stack, FuncTable* table, Statement* statement);
bool semantic_assignment(VarTableStack *stack, FuncTable *table, Statement *statement);
bool semantic_if(VarTableStack *stack, FuncTable *table, Statement *statement);
bool semantic_while(VarTableStack *stack, FuncTable *table, Statement *statement);
bool semantic_return(VarTableStack *stack, FuncTable *table, Statement *statement);
bool semantic_expression(VarTableStack *stack, FuncTable *table, Statement *statement);
bool semantic_function(VarTableStack *stack, FuncTable *table, Statement *statement);