#pragma once

#include "expression.h"
#include "rule_stack.h"

typedef struct RuleNode {
    RuleNode* left;
    RuleNode* right;
    Expression* exp;
} RuleNode;

void init_rule_tree(RuleNode* tree);
void dispose_rule_tree(RuleNode* tree);
void rule_tree_insert_left(RuleNode* root, RuleNode* rule);
void rule_tree_insert_right(RuleNode* root, RuleNode* rule);
void rule_tree_postorder(RuleNode* tree, RuleStack* stack);