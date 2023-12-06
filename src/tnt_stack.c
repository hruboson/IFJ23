#include "rules.h"
#include "tnt_stack.h"

#include "stdlib.h"
#include <stdio.h>

extern TNT T_end, NT_exp;

void init_tnt_stack(TNTStack* stack) {
	if (!stack)
		exit(99);

	stack->size = 0;
	stack->capacity = TNT_STACK_SIZE;
	stack->tnts = malloc(sizeof(*stack->tnts) * TNT_STACK_SIZE);
	if (!stack->tnts) {
		exit(99);
	}

	tnt_stack_push( stack, (TS_Item){ &T_end, NULL } );
}
void clear_tnt_stack(TNTStack* stack) {
	if (stack->tnts) {
		free(stack->tnts);
		stack->tnts = NULL;
		stack->size = 0;
		stack->capacity = 0;
	}
}

void tnt_stack_push(TNTStack* stack, TS_Item tnt) {
	if (stack->size >= stack->capacity - 1) {
		stack->capacity *= 2;
		stack->tnts = realloc(stack->tnts, sizeof(TNT*) * stack->capacity);
		if (!stack->tnts) exit(99);
	}

	stack->size++;

	stack->tnts[stack->size - 1] = tnt;
}
void tnt_stack_pop(TNTStack* stack, TS_Item* tnt) {
	if (!tnt_stack_is_empty(stack)) {
		*tnt = stack->tnts[stack->size - 1];
		stack->size--;
	}
}

bool tnt_stack_is_empty(TNTStack* stack) {
	if (!stack) exit(99);
	return (stack->size == 0);
}
