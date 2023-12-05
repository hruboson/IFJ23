#pragma once

#include "rules.h"
#include "stdbool.h"

#define STACK_SIZE 128

typedef struct TNTStack {
	TNT** tnts;
	int capacity;
	int size;
} TNTStack;

void init_tnt_stack(TNTStack* stack);
void clear_tnt_stack(TNTStack* stack);

void tnt_stack_push(TNTStack* stack, TNT* rule);
void tnt_stack_pop(TNTStack* stack, TNT** rule);

bool tnt_stack_is_empty(TNTStack* stack);