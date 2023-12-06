/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
 * xsatin03 Ondřej Šatinský
*/

#include "rule_tree.h"

#include "exp_stack.h"
#include "rules.h"
#include "stdlib.h"

#include "utils.h"

void init_rule_tree(Node** tree) {
	(*tree) = (Node*)malloc(sizeof(Node));
	(*tree)->val = NULL;

	for (size_t i = 0; i < 3; i++) {
		(*tree)->children_nodes[i] = NULL;
	}
}
void dispose_rule_tree(Node* tree) {
	//! fix last clear doesnt free the node (should probably be Node** tree)
	if (!tree) return;
	for (size_t i = 0; i < 3; i++) {
		dispose_rule_tree((tree->children_nodes)[i]);
	}
	free(tree);
}
void rule_tree_insert(Node* root, size_t node_index, Token* val) {
	Node* rule = malloc(sizeof(Node));
	if (!rule) exit(99);

	if (node_index >= 0 && node_index < 3) {
		root->children_nodes[node_index] = rule;
		for (size_t i = 0; i < 3; i++) {
			rule->children_nodes[i] = NULL;
		}
	}
	else {
		exit(99);
	}
}

// pomocná funkce pro sečtení argumentů volání funkce
size_t rule_tree_fn_args(Node* root, ExpStack* stack) {
	Node* c0 = root->children_nodes[0];
	Node* c1 = root->children_nodes[1];
	if (c0 == NULL) return 0;
	switch (c0->nt) {
	case NT_EXP:
		rule_tree_postorder(c0, stack);
		break;
	case NT_E_ID:
		
		break;
	default:
		break;
	}

	if (c1->children_nodes[0] == NULL) {
		return 1;
	}
	return 1 + rule_tree_fn_args(c1->children_nodes[1], stack);
}

void rule_tree_postorder(Node* tree, ExpStack* stack) {
	if (!tree) return;

	Node* c0 = tree->children_nodes[0];
	Node* c1 = tree->children_nodes[1];

	// zpracování (<arg_list>)
	if (tree->nt == NT_EXP5 && c0->val->type == TOKENTYPE_ID && c1->children_nodes[0] != NULL) {
		Expression* e;
		e = malloc(sizeof(*e));
		if (e == NULL)
			exit(99);

		e->type = ET_FUNC;
		e->fn_call.id = c0->val->value.id;
		e->fn_call.arg_count = 0;

		Node* c110 = c1->children_nodes[1]->children_nodes[0];
		if (c110 == NULL) {
			exp_stack_push(stack, e);
			return;
		}

		e->fn_call.arg_count = rule_tree_fn_args(c1->children_nodes[1], stack);
		//printf("ARG CUNT = %lu\n", e->fn_call.arg_count); //! remove
		e->fn_call.args = malloc(sizeof(Argument) * e->fn_call.arg_count);
		if (e->fn_call.args == NULL) exit(99);
		for (int i = e->fn_call.arg_count - 1; i >= 0; i--) {
			exp_stack_pop(stack, &e->fn_call.args[i].exp);
			e->fn_call.args[i].id = NULL;
		}
		exp_stack_push(stack, e);
		return;
	}


	for (size_t i = 0; i < 3; i++) {
		rule_tree_postorder((tree->children_nodes)[i], stack);
	}

	if (c0 == NULL)
		return;
	if (tree->isTerminal)
		return;
	if (c0->isTerminal == false)
		return;
	if (tree->nt == NT_EXP5 && c0->val->type == TOKENTYPE_PAR_L)
		return;

	Expression* e;
	e = malloc(sizeof(*e));
	if (e == NULL)
		exit(99);

	Token* c0t = c0->val;

	if (tree->nt == NT_EXP5 && tree->children_nodes[1]) {

	}

	// pokud je to operátor - vyber dva a dej je do jednoho expressionu
	if (
		c0t->type == TOKENTYPE_QUESTIONMARK2 ||
		c0t->type == TOKENTYPE_EQUALS2 ||
		c0t->type == TOKENTYPE_NOT_EQUALS ||
		c0t->type == TOKENTYPE_LESSER ||
		c0t->type == TOKENTYPE_GREATER ||
		c0t->type == TOKENTYPE_LESSER_OR_EQUAL ||
		c0t->type == TOKENTYPE_GREATER_OR_EQUAL ||
		c0t->type == TOKENTYPE_PLUS ||
		c0t->type == TOKENTYPE_MINUS ||
		c0t->type == TOKENTYPE_STAR ||
		c0t->type == TOKENTYPE_SLASH
		) {
		Expression* e1;
		Expression* e2;
		exp_stack_pop(stack, &e2);
		exp_stack_pop(stack, &e1);
		e->ops[0] = e1;
		e->ops[1] = e2;
		e->ops[2] = NULL;
		exp_stack_push(stack, e);

		switch (c0t->type) {
		case TOKENTYPE_QUESTIONMARK2: e->type = ET_NIL_TEST; break;
		case TOKENTYPE_EQUALS2: e->type = ET_EQUAL; break;
		case TOKENTYPE_NOT_EQUALS: e->type = ET_N_EQUAL; break;
		case TOKENTYPE_LESSER: e->type = ET_LT; break;
		case TOKENTYPE_GREATER: e->type = ET_GT; break;
		case TOKENTYPE_LESSER_OR_EQUAL: e->type = ET_LTE; break;
		case TOKENTYPE_GREATER_OR_EQUAL: e->type = ET_GTE; break;
		case TOKENTYPE_PLUS: e->type = ET_ADD; break;
		case TOKENTYPE_MINUS: e->type = ET_SUB; break;
		case TOKENTYPE_STAR: e->type = ET_MULT; break;
		case TOKENTYPE_SLASH: e->type = ET_DIV; break;
		}
	}
	// pokud je to '!'
	else if (c0t->type == TOKENTYPE_EXCLAMATION) {
		e->type = ET_EXCLAMATION;
		Expression* e1;
		exp_stack_pop(stack, &e1);
		e->ops[0] = e1;
		e->ops[1] = e->ops[2] = NULL;
		e->type = ET_EXCLAMATION;
		exp_stack_push(stack, e);
	}
	// pokud je to id / int / double / string
	else {
		if (c0t->type == TOKENTYPE_ID) {
			e->type = ET_ID;
			e->id = c0->val->value.id;
		}
		else if (c0t->type == TOKENTYPE_INT) {
			e->type = ET_INT;
			e->int_ = c0->val->value.int_;
		}
		else if (c0t->type == TOKENTYPE_DOUBLE) {
			e->type = ET_DOUBLE;
			e->double_ = c0->val->value.double_;
		}
		else if (c0t->type == TOKENTYPE_STRING) {
			e->type = ET_STRING;
			e->str_ = c0->val->value.str_;
		}
		else if (c0t->type == TOKENTYPE_KEYWORD && c0t->value.keyword == KEYWORD_NIL) {
			e->type = ET_NIL;
		}
		else if (1) {
			fprintf(stdout, "error: unexpected terminal: ");
			print_token(c0->val);
			fprintf(stdout, "\n");
			exit(99);
		}
		exp_stack_push(stack, e);
	}
}
