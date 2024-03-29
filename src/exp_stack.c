/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
*/

#include "exp_stack.h"

#include "stdlib.h"

void init_exp_stack(ExpStack* stack) {
    if (!stack) {
        exit(99);
    } else {
        stack->size = -1;
        stack->capacity = STACK_SIZE;
        stack->exps = malloc(sizeof(Expression*) * STACK_SIZE);
        if (!stack->exps) {
            exit(99);
        }
    }
}
void clear_exp_stack(ExpStack* stack) {
    if (stack->exps) {
        free(stack->exps);
        stack->exps = NULL;
        stack->size = 0;
        stack->capacity = 0;
        stack = NULL;
    }
}

void exp_stack_push(ExpStack* stack, Expression* exp) {
    if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->exps = realloc(stack->exps, sizeof(Expression*) * stack->capacity);
        if (!stack->exps) exit(99);
    }

    if (stack->size == -1) {
        stack->size = 1;
    } else {
        stack->size++;
    }

    stack->exps[stack->size - 1] = exp;
}
void exp_stack_pop(ExpStack* stack, Expression** exp) {
    if (!exp_stack_is_empty(stack)) {
        *exp = stack->exps[stack->size - 1];
        stack->size--;
    }
}

bool exp_stack_is_empty(ExpStack* stack) {
    if (!stack) exit(99);
    return (stack->size == 0) ? true : false;
}