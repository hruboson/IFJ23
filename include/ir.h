#pragma once

#include "AST.h"
#include "symtable.h"

typedef enum IR_Type {
	IRT_label,
	IRT_goto,
	IRT_branch_nil,
	IRT_branch_false,

	IRT_call,
	IRT_return,

	IRT_add,
	IRT_sub,
	IRT_mul,
	IRT_div,
	IRT_idiv,

	IRT_equal,
	IRT_nequal,
	IRT_lt, IRT_lte,
	IRT_gt, IRT_gte,

	IRT_asgn,

	IRT_asgn_int,
	IRT_asgn_double,
	IRT_asgn_string,
	IRT_asgn_nil,
} IR_Type;

typedef struct IR_Inst {
	IR_Type type;
	union {
		SymbolRecord* label;
		SymbolRecord* id;
		SymbolRecord* ops[3];
		struct {
			int v;
			SymbolRecord* id;
		} a_i;
		struct {
			double v;
			SymbolRecord* id;
		} a_d;
		struct {
			String v;
			SymbolRecord* id;
		} a_s;
		struct {
			SymbolRecord* fn_id;

			SymbolRecord** args;
			size_t arg_count;

			SymbolRecord* ret_id;
		} fn_call;
	};
} IR_Inst;

typedef struct IR_Body {
	IR_Inst* inst;
	size_t count, cap;

	// compiler variable
	size_t cv_i;
	// compiler lable
	size_t cl_i;
} IR_Body;

typedef struct IR_Func {
	SymbolRecord* id;

	// parameters
	size_t param_count;
	SymbolRecord** params;

	IR_Body body;
} IR_Func;

typedef struct IR {
	SymbolTable symtab;

	IR_Body main;

	IR_Func* funcs;
	size_t func_count, func_cap;
} IR;

void init_ir( IR* );
void clear_ir( IR* );

int convert_to_ir( const AST*, IR* );
