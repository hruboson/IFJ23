#include "vartable_stack.h"

#include "stdlib.h"

void init_vartable_stack(VarTableStack* stack) {
    if (stack == NULL) {
        exit(99);
    } else {
        stack->size = -1;
        stack->cap = STACK_SIZE;
        stack->vartables = malloc(sizeof(VarTable*) * STACK_SIZE);
        if (stack->vartables == NULL) {
            exit(99);
        }
    }
}

void clear_vartable_stack(VarTableStack* stack) {
    if (stack->vartables != NULL) {
        free(stack->vartables);
        stack->vartables = NULL;
        stack->size = 0;
        stack->cap = 0;
        stack = NULL;  //! only clear or delete ?
    }
}

void vartable_stack_push(VarTableStack* stack, VarTable* table) {
    if (stack->size == stack->cap - 1) {  // max cap reached
        stack->cap *= 2;
        stack->vartables = realloc(stack->vartables, sizeof(VarTable*) * stack->cap);
        if (!stack->vartables) exit(99);
    }

    if (stack->size == -1) {
        stack->size = 1;
    } else {
        stack->size++;
    }
    stack->vartables[stack->size - 1] = table;
}

void vartable_stack_pop(VarTableStack* stack, VarTable** table) {
    if (!vartable_stack_is_empty(stack)) {
        *table = stack->vartables[stack->size - 1];
        stack->size--;
    }
}

void vartable_stack_peek(VarTableStack* stack, VarTable** table) {
    if (!vartable_stack_is_empty(stack)) {
        *table = stack->vartables[stack->size - 1];
    }
}

bool vartable_stack_is_empty(VarTableStack* stack) {
    if (!stack) exit(99);
    return stack->size == 0;
}

Variable* var_table_stack_get_var(VarTableStack* stack, SymbolRecord* id) {
    VarTable* curr;
    Variable* var = NULL;
    size_t i = stack->size;

    // this is wacky af but it works
    while (i > 0 && var == NULL) {
        curr = stack->vartables[i - 1];
        var = var_table_get(curr, id);
        i--;
    }

    return var;
}
