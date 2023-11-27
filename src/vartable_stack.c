#include "vartable_stack.h"

#include "stdlib.h"

void init_vartable_stack(VarTableStack* stack) {
    if (stack == NULL) {
        exit(99);
    } else {
        stack->size = -1;
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
    if (stack->size == stack->cap) { // max cap reached
        stack->cap *= 2;
        stack->vartables = realloc(stack->vartables, sizeof(VarTable*) * stack->cap);
        if (!stack->vartables) exit(99);
    }
    
    stack->size++;
    stack->vartables[stack->size - 1] = table;
}

void vartable_stack_pop(VarTableStack* stack, VarTable* table) {
    if (!vartable_stack_is_empty(stack)) {
        table = stack->vartables[stack->size - 1];
        stack->size--;
    }
}

void vartable_stack_peek(VarTableStack* stack, VarTable* table) {
    if (!vartable_stack_is_empty(stack)) {
        table = stack->vartables[stack->size - 1];
    }
}

bool vartable_stack_is_empty(VarTableStack* stack) {
    if (!stack) exit(99);
    return stack->size == 0;
}