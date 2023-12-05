#pragma once

#include "expression.h"
#include "stdbool.h"

#define STACK_SIZE 64

typedef struct ExpStack {
    Expression** exps;
    int capacity;
    int size;
} ExpStack;

void init_exp_stack(ExpStack* stack);
void clear_exp_stack(ExpStack* stack);

void exp_stack_push(ExpStack* stack, Expression* exp);
void exp_stack_pop(ExpStack* stack, Expression** exp);

bool exp_stack_is_empty(ExpStack* stack);