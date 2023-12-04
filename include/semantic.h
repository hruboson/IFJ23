#pragma once

#include "AST.h"
#include <stdio.h>
#include "vartable_stack.h"

// int semantic( Input*, AST* );

int semantic_variable(VarTableStack* stack, Statement* statement, SymbolTable *table);
int semantic_assignment(VarTableStack *stack, FuncTable *table, Statement *statement);
int semantic_if(VarTableStack *stack, FuncTable *table, Statement *statement);
int semantic_while(VarTableStack *stack, FuncTable *table, Statement *statement);
int semantic_return(VarTableStack *stack, FuncTable *table, Statement *statement, SymbolRecord *funcId);
int semantic_expression(VarTableStack *stack, FuncTable *table, Statement *statement);
int semantic_function(VarTableStack *stack, FuncTable *table, Statement *statement);