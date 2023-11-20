#pragma once

#include "symtable.h"

#define STACK_SIZE 64

typedef struct SymbolTableStack{
    SymbolTable** symtables;
    int size;
    int cap;
}SymbolTableStack;

void init_symtable_stack(SymbolTableStack* stack);
void clear_symtable_stack(SymbolTableStack* stack);

void symtable_stack_push(SymbolTableStack* stack, SymbolTable* symtable);
void symtable_stack_pop(SymbolTableStack* stack, SymbolTable* symtable);
void symtable_stack_peek(SymbolTableStack* stack, SymbolTable* symtable);

bool symtable_stack_is_empty(SymbolTableStack* stack);