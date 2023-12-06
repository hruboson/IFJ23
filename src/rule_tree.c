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

	//todo naplnit rule

	if (node_index >= 0 && node_index < 3) {
		root->children_nodes[node_index] = rule;
	} else {
		exit(99);
	}
}

void rule_tree_postorder(Node* tree, ExpStack* stack) {
	if (!tree) return;

	for (size_t i = 0; i < 3; i++) {
		rule_tree_postorder((tree->children_nodes)[i], stack);
	}

	Node* c0 = tree->children_nodes[ 0 ];
	if ( c0 == NULL )
		return;
	if ( tree->isTerminal )
		return;
	if ( c0->isTerminal == false )
		return;

	Expression* e;
	e = malloc( sizeof( *e ) );
	if ( e == NULL )
		exit( 99 );


	// pokud je to operátor - vyber dva a dej je do jednoho expressionu
	if (
		c0->val->type == TOKENTYPE_QUESTIONMARK2 ||
		c0->val->type == TOKENTYPE_EQUALS2 ||
		c0->val->type == TOKENTYPE_NOT_EQUALS ||
		c0->val->type == TOKENTYPE_LESSER ||
		c0->val->type == TOKENTYPE_GREATER ||
		c0->val->type == TOKENTYPE_LESSER_OR_EQUAL ||
		c0->val->type == TOKENTYPE_GREATER_OR_EQUAL ||
		c0->val->type == TOKENTYPE_PLUS ||
		c0->val->type == TOKENTYPE_MINUS ||
		c0->val->type == TOKENTYPE_STAR ||
		c0->val->type == TOKENTYPE_SLASH
	) {
		Expression* e1;
		Expression* e2;
		exp_stack_pop(stack, &e2);
		exp_stack_pop(stack, &e1);
		e->ops[0] = e1;
		e->ops[1] = e2;
		e->ops[2] = NULL;
		exp_stack_push(stack, e);

		switch ( c0->val->type ) {
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
	else if (c0->val->type == TOKENTYPE_EXCLAMATION) {
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
		if (c0->val->type == TOKENTYPE_ID) {
			e->type = ET_ID;
			e->id = c0->val->value.id;
		}
		else if (c0->val->type == TOKENTYPE_INT) {
			e->type = ET_INT;
			e->int_ = c0->val->value.int_;
		}
		else if (c0->val->type == TOKENTYPE_DOUBLE) {
			e->type = ET_DOUBLE;
			e->double_ = c0->val->value.double_;
		}
		else if (c0->val->type == TOKENTYPE_STRING) {
			e->type = ET_STRING;
			e->str_ = c0->val->value.str_;
		} else {
			fprintf( stdout, "error: unexpected terminal: " );
			print_token( c0->val );
			fprintf( stdout, "\n" );
			exit( 99 );
		}
		exp_stack_push(stack, e);
	}
}
