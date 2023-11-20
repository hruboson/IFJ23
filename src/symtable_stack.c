#include "symtable_stack.h"

#include "stdlib.h"

void init_symtable_stack(SymbolTableStack* stack) {
    if (stack == NULL) {
        exit(99);
    } else {
        stack->size = -1;
        stack->symtables = malloc(sizeof(SymbolTable*) * STACK_SIZE);
        if (stack->symtables == NULL) {
            exit(99);
        }
    }
}

void clear_symtable_stack(SymbolTableStack* stack) {
    if (stack->symtables != NULL) {
        free(stack->symtables);
        stack->symtables = NULL;
        stack->size = 0;
        stack->cap = 0;
        stack = NULL;  //! only clear or delete ?
    }
}

void symtable_stack_push(SymbolTableStack* stack, SymbolTable* table) {
    if (stack->size == stack->cap) { // max cap reached
        stack->cap *= 2;
        stack->symtables = realloc(stack->symtables, sizeof(SymbolTable*) * stack->cap);
        if (!stack->symtables) exit(99);
    }
    
    stack->size++;
    stack->symtables[stack->size - 1] = table;
}

void symtable_stack_pop(SymbolTableStack* stack, SymbolTable* table) {
    if (!symtable_stack_is_empty(stack)) {
        table = stack->symtables[stack->size - 1];
        stack->size--;
    }
}

void symtable_stack_peek(SymbolTableStack* stack, SymbolTable* table) {
    if (!symtable_stack_is_empty(stack)) {
        table = stack->symtables[stack->size - 1];
    }
}

bool symtable_stack_is_empty(SymbolTableStack* stack) {
    if (!stack) exit(99);
    return stack->size == 0;
}