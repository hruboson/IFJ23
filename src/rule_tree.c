#include "rule_tree.h"

#include "exp_stack.h"
#include "rules.h"
#include "stdlib.h"

void init_rule_tree(Node** tree) {
    (*tree) = (Node*) malloc(sizeof(Node));
    (*tree)->val = NULL;
    //(*tree)->node_list = NULL;
}
void dispose_rule_tree(Node* tree) {
	//! fix last clear doesnt free the node (should probably be Node** tree)
    if (!tree) return;
	for(size_t i = 0; i < 3; i++){
		dispose_rule_tree((tree->node_list)[i]);
	}
    free(tree);
}
void rule_tree_insert(Node* root, size_t node_index, Token* val) {
    Node* rule = malloc(sizeof(Node));
    if (!rule) exit(99);
    
	//todo naplnit rule

	if(node_index >= 0 && node_index < 3){
		rule->node_list[node_index] = rule;
	}
}

void rule_tree_postorder(Node* tree, ExpStack* stack) {
    if (!tree) return;

	for(size_t i = 0; i < 3; i++){
		rule_tree_postorder(&(tree->node_list)[i], stack);
	}

	// todo kdy pushovat
    exp_stack_push(stack, tree);
}