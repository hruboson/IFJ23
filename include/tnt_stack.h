/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
*/

#pragma once

#include "rules.h"
#include "stdbool.h"
#include <stddef.h>

#include "rule_tree.h"

#define TNT_STACK_SIZE 128

typedef struct TS_Item {
	TNT* tnt;
	Node* node;
} TS_Item;

typedef struct TNTStack {
	TS_Item* tnts;
	size_t capacity;
	size_t size;
} TNTStack;

void init_tnt_stack(TNTStack* stack);
void clear_tnt_stack(TNTStack* stack);

void tnt_stack_push(TNTStack* stack, TS_Item rule);
void tnt_stack_pop(TNTStack* stack, TS_Item* rule);

bool tnt_stack_is_empty(TNTStack* stack);
