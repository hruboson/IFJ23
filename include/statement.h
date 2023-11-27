#pragma once

#include "expression.h"
#include "scanner.h"

typedef enum StatementType {
    ST_VAR,
    ST_IF,
    ST_ASSIGN,
    ST_WHILE,
    ST_RETURN,
    ST_EXP,
    ST_FUNC
} StatementType;

typedef struct Statement {
    StatementType type;
    union {
        struct {
            bool modifiable;
            SymbolRecord* id;
            Expression* exp;
            Keyword data_type;  // if KEYWORD_NIL -> not set
            bool allow_nil;
        } var;
        struct {
            SymbolRecord* id;
            Expression* exp;
        } assign;
        struct {
            Expression* exp;
            bool check_nil;
            struct Statement* body;
            struct Statement* else_;
        } if_;
        struct {
            Expression* exp;
            struct Statement* body;
        } while_;
        struct {
            Expression* exp;
        } return_;
    };
    struct Statement* next;
} Statement;
