#pragma once

#include "rules.h"
#include "stdbool.h"

#define STACK_SIZE 128

typedef struct RuleStack {
    Rule** rules;
    int capacity;
    int size;
} RuleStack;

void init_rule_stack(RuleStack* stack);
void clear_rule_stack(RuleStack* stack);

void rule_stack_push(RuleStack* stack, Rule* rule);
void rule_stack_pop(RuleStack* stack, Rule* rule);

bool rule_stack_is_empty(RuleStack* stack);