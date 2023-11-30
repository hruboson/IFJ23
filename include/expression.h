#pragma once

#include "symtable.h"
#include "string_.h"
#include "table.h" // for DataType

typedef enum ExpressionType {
    ET_EXCLAMATION,
    ET_MULT,
    ET_DIV,
    ET_ADD,
    ET_SUB,
    ET_EQUAL,
    ET_N_EQUAL,
    ET_LT,
    ET_GT,
    ET_LTE,
    ET_GTE,
    ET_NIL_TEST, // '??' token
    ET_ID,
    ET_INT,
    ET_DOUBLE,
    ET_STRING,
    ET_FUNC,
    ET_NIL,
} ExpressionType;

typedef struct Expression Expression;

typedef struct Argument {
    SymbolRecord* id;
    Expression* exp;
} Argument;

struct Expression {
    ExpressionType type;
    DataType data_type;
    union {
        String str_;
        double double_;
        int int_;
        SymbolRecord* id;
        Expression* ops[2];
        struct {
            SymbolRecord* id;
            Argument* args;
            size_t arg_count, arg_cap;
        } fn_call;
    };
};
