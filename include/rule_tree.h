#pragma once

#include "expression.h"
#include "rule_stack.h"

typedef struct RuleNode RuleNode;

struct RuleNode {
    RuleNode* left;
    RuleNode* right;
    Expression* exp;
};

void init_rule_tree(RuleNode** tree);
void dispose_rule_tree(RuleNode* tree);
void rule_tree_insert_left(RuleNode* root, Expression* exp);
void rule_tree_insert_right(RuleNode* root, Expression* exp);
void rule_tree_postorder(RuleNode* tree, RuleStack* stack);