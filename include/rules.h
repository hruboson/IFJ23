#pragma once

#include "stdbool.h"

typedef enum NonTerminal {
	NT_EXP,
	NT_EXP1,
	NT_EXP1_,
	NT_EXP2,
	NT_EXP2_,
	NT_EXP3,
	NT_EXP3_,
	NT_EXP4,
	NT_EXP4_,
	NT_EXP5,
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
	T_COMMA,
	T_EPS,
	T_END
} Terminal;

typedef struct TNT { // Terminal / Non Terminal
	bool is_terminal;
	union {
		Terminal terminal;
		NonTerminal non_terminal;
	};
} TNT;

// (valid, expand_to) =>
// (1, neprazdne) == standardni pravidlo
// (1, prazdne) == epsilon pravidlo
// (0, jakekoliv) == chyba syntaktické analýzy
typedef struct Rule {
	bool valid;
	TNT expand_to[3];
} Rule;

// TODO: 2D pole pravidel
// tabulka v "../LL-table.csv"
Rule rule_table[NT_END+1][T_END+1] = {
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = true, .expand_to = {{.is_terminal = true, .terminal = T_NIL_TEST}, {.is_terminal = false, .non_terminal = NT_EXP}}}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true}, {.valid = true} },
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}},
	{{.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = false}, {.valid = true}}
};
