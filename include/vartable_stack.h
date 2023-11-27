#pragma once

#include "vartable_stack.h"
#include "table.h"

#define STACK_SIZE 64

// todo redo for VarTable
typedef struct VarTableStack {
    VarTable** vartables;
    int size;
    int cap;
} VarTableStack;

void init_vartable_stack(VarTableStack* stack);
void clear_vartable_stack(VarTableStack* stack);

void vartable_stack_push(VarTableStack* stack, VarTable* vartable);
void vartable_stack_pop(VarTableStack* stack, VarTable* vartable);
void vartable_stack_peek(VarTableStack* stack, VarTable* vartable);

//TODO: implementovat:
Variable* var_table_stack_get_var(VarTableStack* stack, SymbolRecord* id); // -> throws error -> fix

bool vartable_stack_is_empty(VarTableStack* stack);