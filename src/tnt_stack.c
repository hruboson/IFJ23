#include "rules.h"
#include "tnt_stack.h"

#include "stdlib.h"

void init_tnt_stack(TNTStack* stack) {
	if (!stack) {
		exit(99);
	}
	else {
		stack->size = -1;
		stack->capacity = STACK_SIZE;
		stack->tnts = malloc(sizeof(TNT*) * STACK_SIZE);
		if (!stack->tnts) {
			exit(99);
		}
	}
}
void clear_tnt_stack(TNTStack* stack) {
	if (stack->tnts) {
		free(stack->tnts);
		stack->tnts = NULL;
		stack->size = 0;
		stack->capacity = 0;
	}
}

void tnt_stack_push(TNTStack* stack, TNT* tnt) {
	if (stack->size >= stack->capacity - 1) {
		stack->capacity *= 2;
		stack->tnts = realloc(stack->tnts, sizeof(TNT*) * stack->capacity);
		if (!stack->tnts) exit(99);
	}

	if (stack->size == -1) {
		stack->size = 1;
	}
	else {
		stack->size++;
	}

	stack->tnts[stack->size - 1] = tnt;
}
void tnt_stack_pop(TNTStack* stack, TNT** tnt) {
	if (!tnt_stack_is_empty(stack)) {
		*tnt = stack->tnts[stack->size - 1];
		stack->size--;
	}
}

bool tnt_stack_is_empty(TNTStack* stack) {
	if (!stack) exit(99);
	return (stack->size == 0) ? true : false;
}