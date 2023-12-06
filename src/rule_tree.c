#include "rule_tree.h"

#include "exp_stack.h"
#include "rules.h"
#include "stdlib.h"

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

	//todo naplnit rule

	if (node_index >= 0 && node_index < 3) {
		rule->children_nodes[node_index] = rule;
	}
	else {
		exit(99);
	}
}

void rule_tree_postorder(Node* tree, ExpStack* stack) {
	if (!tree) return;

	for (size_t i = 0; i < 3; i++) {
		rule_tree_postorder((tree->children_nodes)[i], stack);
	}

	if (tree->children_nodes[0] != NULL) {
		if (!(tree->isTerminal) && tree->children_nodes[0]->isTerminal) {
			Expression e;
			// pokud je to operátor - vyber dva a dej je do jednoho expressionu
			if (tree->children_nodes[0]->val->type == TOKENTYPE_QUESTIONMARK2 ||
				tree->children_nodes[0]->val->type == TOKENTYPE_EQUALS2 ||
				tree->children_nodes[0]->val->type == TOKENTYPE_NOT_EQUALS ||
				tree->children_nodes[0]->val->type == TOKENTYPE_LESSER ||
				tree->children_nodes[0]->val->type == TOKENTYPE_GREATER ||
				tree->children_nodes[0]->val->type == TOKENTYPE_LESSER_OR_EQUAL ||
				tree->children_nodes[0]->val->type == TOKENTYPE_GREATER_OR_EQUAL ||
				tree->children_nodes[0]->val->type == TOKENTYPE_PLUS ||
				tree->children_nodes[0]->val->type == TOKENTYPE_MINUS ||
				tree->children_nodes[0]->val->type == TOKENTYPE_STAR ||
				tree->children_nodes[0]->val->type == TOKENTYPE_SLASH) {
				Expression* e1;
				Expression* e2;
				exp_stack_pop(stack, &e1);
				exp_stack_pop(stack, &e2);
				e.ops[0] = e1;
				e.ops[1] = e2;
				exp_stack_push(stack, &e);
			}
			// pokud je to '!'
			else if (tree->children_nodes[0]->val->type == TOKENTYPE_EXCLAMATION) {
				e.type = ET_EXCLAMATION;
				Expression* e1;
				exp_stack_pop(stack, &e1);
				e.ops[0] = e1;
				exp_stack_push(stack, &e);
			}
			// pokud je to id / int / double / string
			else {
				if (tree->children_nodes[0]->val->type == TOKENTYPE_ID) {
					e.type = ET_ID;
					e.id = tree->children_nodes[0]->val->value.id;
				}
				else if (tree->children_nodes[0]->val->type == TOKENTYPE_INT) {
					e.type = ET_INT;
					e.int_ = tree->children_nodes[0]->val->value.int_;
				}
				else if (tree->children_nodes[0]->val->type == TOKENTYPE_DOUBLE) {
					e.type = ET_DOUBLE;
					e.double_ = tree->children_nodes[0]->val->value.double_;
				}
				else if (tree->children_nodes[0]->val->type == TOKENTYPE_STRING) {
					e.type = ET_STRING;
					e.str_ = tree->children_nodes[0]->val->value.str_;
				}
				exp_stack_push(stack, &e);
			}
		}
	}
}