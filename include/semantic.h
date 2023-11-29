#pragma once

#include "AST.h"
#include <stdio.h>
#include "vartable_stack.h"

// int semantic( Input*, AST* );

bool semantic_variable(VarTableStack* stack, FuncTable* table, Statement* statement);
