#pragma once

#define arraysize( x ) ( sizeof( x ) / sizeof( x[0] ) )

#include "scanner.h"
#include "AST.h"
#include "rules.h"
#include "rule_tree.h"

void print_token( const Token* );
const char* nonterminal_to_string( NonTerminal );
const char* terminal_to_string( Terminal );
void print_formatted_string( FILE*, const String* );
void print_keyword( Keyword );
const char* tnt_to_string( const TNT* );
void print_rule_tree( const Node*, size_t indent );
