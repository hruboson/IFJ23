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
            Statement* body;
            Statement* else_;
        } if_;
        struct {
            Expression* exp;
            Statement* body;
        } while_;
        struct {
            Expression* exp
        } return_;
    };
    Statement* next;
} Statement;