#pragma once

typedef enum NonTerminal
{
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

typedef enum Terminal
{
    T_NIL_TEST, // '??' token
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
    T_END
} Terminal;

typedef struct ExpandTo {
    bool is_terminal;
    union {
        Terminal terminal;
        NonTerminal non_terminal;
    };
} ExpandTo;

// (valid, expand_to) =>
    // (1, neprazdne) == standardni pravidlo
    // (1, prazdne) == epsilon pravidlo
    // (0, jakekoliv) == epsilon pravidlo
typedef struct Rule {
    bool valid;
    ExpandTo* expand_to;
} Rule;


// TODO: 2D pole pravidel
Rule RuleTable[][] = {
    {{.valid = false}, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {10, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
    {00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17},
};
