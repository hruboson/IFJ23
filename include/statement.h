/**
 * Implementace překladače imperativního jazyka IFJ23
 * xhrubo01 Ondřej Hruboš
 * xsatin03 Ondřej Šatinský
 * xjestr04 Radek Jestřabík
 * xborek12 Dominik Borek
*/

#pragma once

#include "expression.h"
#include "scanner.h"
#include "table.h"

typedef enum StatementType {
	ST_VAR,
	ST_IF,
	ST_ASSIGN,
	ST_WHILE,
	ST_RETURN,
	ST_EXP,
	ST_FUNC
} StatementType;

typedef struct IdPrefix {
	SymbolRecord* func_id;
	size_t block_counter;
} IdPrefix;

typedef struct Statement {
	StatementType type;
	union {
		struct {
			bool modifiable;
			SymbolRecord* id;
			Expression* exp;
			DataType data_type;  // if VARTYPE_VOID -> not set
			bool used;
			IdPrefix id_prefix;
			SymbolRecord* unique_id;
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
		struct {
			Expression* exp;
		} exp;
		struct {
			SymbolRecord* id;
			size_t param_count;
			Parameter* parameters;
			DataType return_type;
			struct Statement* body;
			bool used;
		} func;
	};
	struct Statement* next;
} Statement;
