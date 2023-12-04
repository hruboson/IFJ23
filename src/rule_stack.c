#include "rule_stack.h"

#include "stdlib.h"

void init_rule_stack(RuleStack* stack) {
    if (!stack) {
        exit(99);
    } else {
        stack->size = -1;
        stack->rules = malloc(sizeof(Rule*) * STACK_SIZE);
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
    if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->rules = realloc(stack->rules, sizeof(Rule*) * stack->capacity);
        if (!stack->rules) exit(99);
    }

    stack->size++;
    stack->rules[stack->size - 1] = rule;
}
void rule_stack_pop(RuleStack* stack, Rule* rule) {
    if (!rule_stack_is_empty(stack)) {
        rule = stack->rules[stack->size - 1];
        stack->size--;
    }
}

bool rule_stack_is_empty(RuleStack* stack) {
    if (!stack) exit(99);
    return (stack->size == 0) ? true : false;
}