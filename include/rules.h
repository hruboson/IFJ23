/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
 * xsatin03 Ondřej Šatinský
 * xjestr04 Radek Jestřabík
*/

#pragma once

#include "stdbool.h"

typedef enum NonTerminal {
	NT_EXP,
	NT_EXP_,
	NT_EXP1,
	NT_EXP1_,
	NT_EXP2,
	NT_EXP2_,
	NT_EXP3,
	NT_EXP3_,
	NT_EXP4,
	NT_EXP4_,
	NT_EXP5,
	NT_ARGS,
	NT_ARGS_LIST,
	NT_E_ID,
	NT_ARG_LIST_N,
	NT_EXP_ID,

	NT_END
} NonTerminal;

typedef enum Terminal {
	T_NIL_TEST,  // '??' token
	T_EQUAL,
	T_N_EQUAL,
	T_LT,
	T_GT,
	T_LTE,
	T_GTE,
	T_ADD,
	T_SUB,
	T_MULT,
	T_DIV,
	T_NT_EXCLAMATION,
	T_PAR_L,
	T_INT,
	T_STRING,
	T_DOUBLE,
	T_ID,
	T_NIL,
	T_COMMA,
	T_PAR_R,
	T_COLON,
	T_END,

	T_END__
} Terminal;

typedef struct TNT { // Terminal / Non Terminal
	bool is_terminal;
	union {
		Terminal terminal;
		NonTerminal non_terminal;
	};
} TNT;

extern TNT NT_exp;

// (valid, expand_to) =>
// (1, neprazdne) == standardni pravidlo
// (1, prazdne) == epsilon pravidlo
// (0, jakekoliv) == chyba syntaktické analýzy
typedef struct Rule {
	TNT* expand_to[5];
} Rule;

typedef struct RuleTable {
	Rule* table[NT_END][T_END__];
} RuleTable;

Rule* get_rule( NonTerminal, Terminal );
