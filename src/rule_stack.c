/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
*/

#include "rule_stack.h"

#include "stdlib.h"

void init_rule_stack(RuleStack* stack) {
    if (!stack) {
        exit(99);
    } else {
        stack->size = -1;
		stack->capacity = RULE_STACK_SIZE;
        stack->rules = malloc(sizeof(Rule*) * RULE_STACK_SIZE);
        if (!stack->rules) {
            exit(99);
        }
    }
}
void clear_rule_stack(RuleStack* stack) {
    if (stack->rules) {
        free(stack->rules);
        stack->rules = NULL;
        stack->size = 0;
        stack->capacity = 0;
    }
}

void rule_stack_push(RuleStack* stack, Rule* rule) {
    if (stack->size >= stack->capacity - 1) {
        stack->capacity *= 2;
        stack->rules = realloc(stack->rules, sizeof(Rule*) * stack->capacity);
        if (!stack->rules) exit(99);
    }

    if (stack->size == -1) {
        stack->size = 1;
    } else {
        stack->size++;
    }

    stack->rules[stack->size - 1] = rule;
}
void rule_stack_pop(RuleStack* stack, Rule** rule) {
    if (!rule_stack_is_empty(stack)) {
        *rule = stack->rules[stack->size - 1];
        stack->size--;
    }
}

bool rule_stack_is_empty(RuleStack* stack) {
    if (!stack) exit(99);
    return (stack->size == 0) ? true : false;
}