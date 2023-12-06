/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
 * xsatin03 Ondřej Šatinský
*/

#pragma once

#include "expression.h"
#include "exp_stack.h"
#include "rule_stack.h"

#include "stdbool.h"
#include "scanner.h"

typedef struct Node Node;

struct Node {
	Node* children_nodes[3];
	bool isTerminal;
	union {
		Token* val;
		NonTerminal nt;
	};
};

void init_rule_tree(Node** tree);
void dispose_rule_tree(Node* tree);
void rule_tree_insert(Node* root, size_t index, Token* val);
void rule_tree_postorder(Node* tree, ExpStack* stack);

// pomocná funkce
size_t rule_tree_fn_args(Node* root, ExpStack* stack);
