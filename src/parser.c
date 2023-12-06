#include "parser.h"
#include "rule_stack.h"
#include "tnt_stack.h"
#include "rule_tree.h"
#include "rules.h"

#include "AST.h"
#include "semantic.h"
#include "token_stack.h"
#include "stdlib.h"
#include "vartable_stack.h"
#include "symtable.h"
#include "table.h"

// newline check
// pars: Input* input, SymbolTable* symtab, Token* token
#define PARSE_POTENTIAL_NEWLINE(input, symtab, token) do { \
	if (token->type == TOKENTYPE_NEWLINE) { \
		get_token(input, symtab, token); \
	} else { \
		break; \
	} \
} while (1)

// #define DEBUG //TODO: ODDELAT

#ifdef DEBUG
#define PRINT_LINE printf("LINE: %d\n", __LINE__)
#define PRINT_TEXT(text) printf("LINE: %s, \n", __LINE__, text)
#define PRINT_ERROR(expected, found) fprintf( stderr, "PARSER ERROR: EXPECTED '%i' FOUND '%i'\n\t%s:%i\n", expected, found, __FILE__, __LINE__ )
#else
#define PRINT_LINE
#define PRINT_TEXT
#define PRINT_ERROR
#endif

bool do_semantic_analysis = true;

int parse(Input* input, AST* ast) {

	TNT exp = {
		.is_terminal = false,
		.non_terminal = NT_EXP
	};

	TNT exp_ = {
		.is_terminal = false,
		.non_terminal = NT_EXP_
	};

	TNT exp1 = {
		.is_terminal = false,
		.non_terminal = NT_EXP1
	};

	TNT exp1_ = {
		.is_terminal = true,
		.non_terminal = NT_EXP2
	};

	TNT exp2 = {
		.is_terminal = false,
		.non_terminal = NT_EXP2
	};

	TNT exp2_ = {
		.is_terminal = false,
		.non_terminal = NT_EXP2_
	};

	TNT exp3 = {
		.is_terminal = false,
		.non_terminal = NT_EXP3
	};

	TNT exp3_ = {
		.is_terminal = false,
		.non_terminal = NT_EXP3_
	};

	TNT exp4 = {
		.is_terminal = false,
		.non_terminal = NT_EXP4
	};

	TNT exp4_ = {
		.is_terminal = false,
		.non_terminal = NT_EXP4_
	};

	TNT exp5 = {
		.is_terminal = false,
		.non_terminal = NT_EXP5
	};

	TNT double_question_mark = {
		.is_terminal = true,
		.terminal = T_NIL_TEST
	};

	TNT equal = {
		.is_terminal = true,
		.terminal = T_EQUAL
	};

	TNT n_equal = {
		.is_terminal = true,
		.terminal = T_N_EQUAL
	};

	TNT lt = {
		.is_terminal = true,
		.terminal = T_LT
	};

	TNT gt = {
		.is_terminal = true,
		.terminal = T_GT
	};

	TNT lte = {
		.is_terminal = true,
		.terminal = T_LTE
	};

	TNT gte = {
		.is_terminal = true,
		.terminal = T_GTE
	};

	TNT add = {
		.is_terminal = true,
		.terminal = T_ADD
	};

	TNT sub = {
		.is_terminal = true,
		.terminal = T_SUB
	};

	TNT mult = {
		.is_terminal = true,
		.terminal = T_MULT
	};

	TNT div = {
		.is_terminal = true,
		.terminal = T_DIV
	};

	TNT exc = {
		.is_terminal = true,
		.terminal = T_NT_EXCLAMATION
	};

	TNT left_par = {
		.is_terminal = true,
		.terminal = T_PAR_L
	};

	TNT right_par = {
		.is_terminal = true,
		.terminal = T_PAR_R
	};

	TNT t_int = {
		.is_terminal = true,
		.terminal = T_INT
	};

	TNT t_string = {
		.is_terminal = true,
		.terminal = T_STRING
	};

	TNT t_double = {
		.is_terminal = true,
		.terminal = T_DOUBLE
	};

	TNT id = {
		.is_terminal = true,
		.terminal = T_ID
	};

	TNT args = {
		.is_terminal = false,
		.non_terminal = NT_ARGS
	};

	TNT arg_list = {
		.is_terminal = false,
		.non_terminal = NT_ARGS_LIST
	};

	TNT arg_list_n = {
		.is_terminal = false,
		.non_terminal = NT_ARG_LIST_N
	};

	TNT e_id = {
		.is_terminal = false,
		.non_terminal = NT_E_ID
	};

	TNT exp_id = {
		.is_terminal = false,
		.non_terminal = NT_EXP_ID
	};

	TNT comma = {
		.is_terminal = true,
		.terminal = T_COMMA
	};

	Rule no_match;
	no_match.valid = false; // syntactic error

	Rule eps;
	eps.valid = true;

	Rule one;
	one.valid = true;
	one.expand_to[0] = &exp1;
	one.expand_to[1] = &exp_;

	Rule two;
	two.valid = true;
	two.expand_to[0] = &double_question_mark;
	two.expand_to[1] = &exp;

	Rule three;
	three.valid = true;
	three.expand_to[0] = &exp2;
	three.expand_to[1] = &exp1_;
	three.expand_to[2] = NULL;

	Rule four_equals;
	four_equals.valid = true;
	four_equals.expand_to[0] = &equal;
	four_equals.expand_to[1] = &exp1;
	four_equals.expand_to[2] = NULL;

	Rule four_n_equals;
	four_n_equals.valid = true;
	four_n_equals.expand_to[0] = &n_equal;
	four_n_equals.expand_to[0] = &exp1;

	Rule four_lt;
	four_lt.valid = true;
	four_lt.expand_to[0] = &lt;
	four_lt.expand_to[1] = &exp1;

	Rule four_gt;
	four_gt.valid = true;
	four_gt.expand_to[0] = &gt;
	four_gt.expand_to[1] = &exp1;


	Rule four_gte;
	four_gte.valid = true;
	four_gte.expand_to[0] = &gte;
	four_gte.expand_to[1] = &exp1;


	Rule four_lte;
	four_lte.valid = true;
	four_lte.expand_to[0] = &lte;
	four_lte.expand_to[1] = &exp1;

	Rule five;
	five.valid = true;
	five.expand_to[0] = &exp3;
	five.expand_to[1] = &exp2_;

	Rule six_add;
	six_add.valid = true;
	six_add.expand_to[0] = &add;
	six_add.expand_to[1] = &exp2;

	Rule six_sub;
	six_sub.valid = true;
	six_sub.expand_to[0] = &sub;
	six_sub.expand_to[1] = &exp2;

	Rule seven;
	seven.valid = true;
	seven.expand_to[0] = &exp4;
	seven.expand_to[1] = &exp3_;

	Rule eight_mult;
	eight_mult.valid = true;
	eight_mult.expand_to[0] = &mult;
	eight_mult.expand_to[1] = &exp3;

	Rule eight_div;
	eight_div.valid = true;
	eight_div.expand_to[0] = &div;
	eight_div.expand_to[1] = &exp3;

	Rule nine;
	nine.valid = true;
	nine.expand_to[0] = &exp5;
	nine.expand_to[1] = &exp4_;

	Rule ten;
	ten.valid = true;
	ten.expand_to[0] = &exc;

	Rule eleven_exp;
	eleven_exp.valid = true;
	eleven_exp.expand_to[0] = &left_par;
	eleven_exp.expand_to[1] = &exp;
	eleven_exp.expand_to[2] = &right_par;

	Rule eleven_t_int;
	eleven_t_int.valid = true;
	eleven_t_int.expand_to[0] = &t_int;

	Rule eleven_t_string;
	eleven_t_string.valid = true;
	eleven_t_string.expand_to[0] = &t_string;

	Rule eleven_t_double;
	eleven_t_double.valid = true;
	eleven_t_double.expand_to[0] = &t_double;

	//! todo rule eleven_id_args => id <args>
	//! todo rules for id()
	Rule eleven_id_args;
	eleven_id_args.valid = true;
	eleven_id_args.expand_to[0] = &id;
	eleven_id_args.expand_to[1] = &args;

	Rule twelve;
	twelve.valid = true;
	twelve.expand_to[0] = &left_par;
	twelve.expand_to[1] = &arg_list;
	twelve.expand_to[2] = &right_par;

	Rule fourteen_id;
	fourteen_id.valid = true;
	fourteen_id.expand_to[0] = &e_id;
	fourteen_id.expand_to[1] = &arg_list_n;

	Rule fourteen_else; // int, double, string
	fourteen_else.valid = true;
	fourteen_else.expand_to[0] = &exp;
	fourteen_else.expand_to[1] = &arg_list_n;

	Rule sixteen;
	sixteen.valid = true;
	sixteen.expand_to[0] = &id;
	sixteen.expand_to[0] = &exp_id;

	Rule eighteen;
	eighteen.valid = true;
	eighteen.expand_to[0] = &comma;
	eighteen.expand_to[1] = &arg_list;

	Rule twentythree;
	eighteen.valid = true;
	eighteen.expand_to[0] = &left_par;
	eighteen.expand_to[1] = &arg_list;
	eighteen.expand_to[2] = &right_par;

	// tabulka v "../LL-table.csv"
	RuleTable rule_table;
	rule_table.table[0][13] = &one;
	rule_table.table[0][14] = &one;
	rule_table.table[0][15] = &one;
	rule_table.table[0][16] = &one;
	rule_table.table[0][17] = &one;

	rule_table.table[1][0] = &two;
	rule_table.table[1][1] = &eps;
	rule_table.table[1][2] = &eps;
	rule_table.table[1][3] = &eps;
	rule_table.table[1][4] = &eps;
	rule_table.table[1][5] = &eps;
	rule_table.table[1][6] = &eps;
	rule_table.table[1][7] = &eps;
	rule_table.table[1][8] = &eps;
	rule_table.table[1][9] = &eps;
	rule_table.table[1][10] = &eps;
	rule_table.table[1][11] = &eps;
	rule_table.table[1][12] = &eps;
	rule_table.table[1][13] = &eps;
	rule_table.table[1][14] = &eps;
	rule_table.table[1][15] = &eps;
	rule_table.table[1][16] = &eps;
	rule_table.table[1][17] = &eps;
	rule_table.table[1][18] = &eps;
	rule_table.table[1][T_END] = &eps;

	rule_table.table[2][13] = &three;
	rule_table.table[2][14] = &three;
	rule_table.table[2][15] = &three;
	rule_table.table[2][16] = &three;
	rule_table.table[2][17] = &three;
	rule_table.table[2][T_END] = &eps;

	rule_table.table[3][0] = &eps;
	rule_table.table[3][1] = &four_equals;
	rule_table.table[3][2] = &four_n_equals;
	rule_table.table[3][3] = &four_lt;
	rule_table.table[3][4] = &four_gt;
	rule_table.table[3][5] = &four_lte;
	rule_table.table[3][6] = &four_gte;
	rule_table.table[3][7] = &eps;
	rule_table.table[3][8] = &eps;
	rule_table.table[3][9] = &eps;
	rule_table.table[3][10] = &eps;
	rule_table.table[3][11] = &eps;
	rule_table.table[3][12] = &eps;
	rule_table.table[3][13] = &eps;
	rule_table.table[3][14] = &eps;
	rule_table.table[3][15] = &eps;
	rule_table.table[3][16] = &eps;
	rule_table.table[3][17] = &eps;
	rule_table.table[3][18] = &eps;
	rule_table.table[3][T_END] = &eps;

	rule_table.table[4][13] = &five;
	rule_table.table[4][14] = &five;
	rule_table.table[4][15] = &five;
	rule_table.table[4][16] = &five;
	rule_table.table[4][17] = &five;
	rule_table.table[4][T_END] = &eps;

	rule_table.table[5][0] = &eps;
	rule_table.table[5][1] = &eps;
	rule_table.table[5][2] = &eps;
	rule_table.table[5][3] = &eps;
	rule_table.table[5][4] = &eps;
	rule_table.table[5][5] = &eps;
	rule_table.table[5][6] = &eps;
	rule_table.table[5][7] = &six_add;
	rule_table.table[5][8] = &six_sub;
	rule_table.table[5][9] = &eps;
	rule_table.table[5][10] = &eps;
	rule_table.table[5][11] = &eps;
	rule_table.table[5][12] = &eps;
	rule_table.table[5][13] = &eps;
	rule_table.table[5][14] = &eps;
	rule_table.table[5][15] = &eps;
	rule_table.table[5][16] = &eps;
	rule_table.table[5][17] = &eps;
	rule_table.table[5][18] = &eps;
	rule_table.table[5][T_END] = &eps;

	rule_table.table[6][13] = &seven;
	rule_table.table[6][14] = &seven;
	rule_table.table[6][15] = &seven;
	rule_table.table[6][16] = &seven;
	rule_table.table[6][17] = &seven;
	rule_table.table[6][T_END] = &eps;

	rule_table.table[7][0] = &eps;
	rule_table.table[7][1] = &eps;
	rule_table.table[7][2] = &eps;
	rule_table.table[7][3] = &eps;
	rule_table.table[7][4] = &eps;
	rule_table.table[7][5] = &eps;
	rule_table.table[7][6] = &eps;
	rule_table.table[7][7] = &eps;
	rule_table.table[7][8] = &eps;
	rule_table.table[7][9] = &eight_mult;
	rule_table.table[7][10] = &eight_div;
	rule_table.table[7][11] = &eps;
	rule_table.table[7][12] = &eps;
	rule_table.table[7][13] = &eps;
	rule_table.table[7][14] = &eps;
	rule_table.table[7][15] = &eps;
	rule_table.table[7][16] = &eps;
	rule_table.table[7][17] = &eps;
	rule_table.table[7][18] = &eps;
	rule_table.table[7][T_END] = &eps;

	rule_table.table[8][13] = &nine;
	rule_table.table[8][14] = &nine;
	rule_table.table[8][15] = &nine;
	rule_table.table[8][16] = &nine;
	rule_table.table[8][17] = &nine;
	rule_table.table[8][T_END] = &eps;

	rule_table.table[9][0] = &eps;
	rule_table.table[9][1] = &eps;
	rule_table.table[9][2] = &eps;
	rule_table.table[9][3] = &eps;
	rule_table.table[9][4] = &eps;
	rule_table.table[9][5] = &eps;
	rule_table.table[9][6] = &eps;
	rule_table.table[9][7] = &eps;
	rule_table.table[9][8] = &eps;
	rule_table.table[9][9] = &eps;
	rule_table.table[9][10] = &eps;
	rule_table.table[9][11] = &ten;
	rule_table.table[9][12] = &eps;
	rule_table.table[9][13] = &eps;
	rule_table.table[9][14] = &eps;
	rule_table.table[9][15] = &eps;
	rule_table.table[9][16] = &eps;
	rule_table.table[9][17] = &eps;
	rule_table.table[9][18] = &eps;
	rule_table.table[9][T_END] = &eps;

	rule_table.table[10][13] = &eleven_exp;
	rule_table.table[10][14] = &eleven_t_int;
	rule_table.table[10][15] = &eleven_t_string;
	rule_table.table[10][16] = &eleven_t_double;
	rule_table.table[10][17] = &eleven_id_args;
	rule_table.table[10][T_END] = &eps;

	rule_table.table[11][12] = &twelve;
	rule_table.table[11][T_END] = &eps;

	rule_table.table[12][12] = &eleven_id_args; //! check this
	rule_table.table[12][13] = &eleven_id_args; //! check this
	rule_table.table[12][14] = &eleven_id_args; //! check this
	rule_table.table[12][15] = &eleven_id_args; //! check this
	rule_table.table[12][16] = &eleven_id_args; //! check this
	rule_table.table[12][T_END] = &eps;

	rule_table.table[13][16] = &sixteen;
	rule_table.table[14][17] = &eighteen;
	rule_table.table[15][12] = &twentythree;


	int ret = 0;

	VarTableStack var_table_stack;
	init_vartable_stack(&var_table_stack);
	VarTable var_table;
	init_var_table(&var_table);
	vartable_stack_push(&var_table_stack, &var_table);

	FuncTable func_table;
	init_func_table(&func_table);

	insert_builtin_functions(&func_table, &ast->symtab);

	ret = parse_statement_list(input,
		&ast->symtab, &ast->statement,
		&var_table_stack, &func_table,
		NULL, NULL, 0, &rule_table
	);

	if (ret == -1) {
		ret = 0;
	}
	else {

		// TODO: clear structs
		// TODO: dodělat řešení, jestli se ti vrátila sémantická chyba, nebo ne
		// TODO: předělat, aby nevracelo vždy 2 (například, pokud je if a semantic vrátí error 9, pak se vypíše v testech, že se vrací 2)
		return ret;
	}

	if (do_semantic_analysis) {
		// TODO: ret = semantic_check_if_called_functions_were_declared()
		// TODO: if (ret != 0) return ret;	
	}

	return ret;
}

void insert_builtin_functions(FuncTable* func_table, SymbolTable* symtab) {

	//func readString() -> String?
	String str_read_string;
	init_string(&str_read_string);
	string_append(&str_read_string, "readString");

	SymbolRecord* sr_str;
	symboltable_insert(symtab, &str_read_string, &sr_str);

	DataType return_type = { .nil_allowed = true, .type = VARTYPE_STRING };
	Function read_str = { .id = sr_str, .is_defined = true, .is_write = false, .param_count = 0, .parameters = NULL, .return_type = return_type };

	func_table_insert(func_table, read_str);

	// func readInt() -> Int?
	String str_read_int;
	init_string(&str_read_int);
	string_append(&str_read_int, "readInt");

	SymbolRecord* sr_read_int;
	symboltable_insert(symtab, &str_read_int, &sr_read_int);

	DataType return_type_read_int = { .nil_allowed = true, .type = VARTYPE_INT };
	Function read_int = { .id = sr_read_int, .is_defined = true, .is_write = false, .param_count = 0, .parameters = NULL, .return_type = return_type_read_int };

	func_table_insert(func_table, read_int);


	// func readDouble() -> Double?
	String str_read_double;
	init_string(&str_read_double);
	string_append(&str_read_double, "readDouble");

	SymbolRecord* sr_read_double;
	symboltable_insert(symtab, &str_read_double, &sr_read_double);

	DataType return_type_read_double = { .nil_allowed = true, .type = VARTYPE_DOUBLE };
	Function read_double = { .id = sr_read_double, .is_defined = true, .is_write = false, .param_count = 0, .parameters = NULL, .return_type = return_type_read_double };

	func_table_insert(func_table, read_double);

	// func write ( term_1 , term_2 , …, term_n )
	String str_write;
	init_string(&str_write);
	string_append(&str_write, "write");

	SymbolRecord* sr_write;
	symboltable_insert(symtab, &str_write, &sr_write);

	DataType return_type_write = { .nil_allowed = true, .type = VARTYPE_VOID };
	Function write = { .id = sr_write, .is_defined = true, .is_write = true, .param_count = 0, .parameters = NULL, .return_type = return_type_write };

	func_table_insert(func_table, write);


	// func Int2Double(_ term ∶ Int) -> Double
	String str_int_to_double;
	init_string(&str_int_to_double);
	string_append(&str_int_to_double, "Int2Double");

	SymbolRecord* sr_int_to_double;
	symboltable_insert(symtab, &str_int_to_double, &sr_int_to_double);

	DataType return_type_int_to_double = { .nil_allowed = false, .type = VARTYPE_DOUBLE };

	String str_param_int_to_double;
	init_string(&str_param_int_to_double);
	string_append(&str_param_int_to_double, "Int2Double");

	SymbolRecord* sr_param_int_to_double;
	symboltable_insert(symtab, &str_int_to_double, &sr_param_int_to_double);

	DataType dt_param_int_to_double = { .nil_allowed = false, .type = VARTYPE_INT };

	Parameter param_term = { .extern_id = NULL, .intern_id = sr_param_int_to_double, .type = dt_param_int_to_double };

	Parameter* param_list_int_to_double = (Parameter*)malloc(sizeof(Parameter) * 1);
	if (param_list_int_to_double == NULL) {
		exit(99);
	}

	param_list_int_to_double[0] = param_term;

	Function int_to_double = { .id = sr_int_to_double, .is_defined = true, .is_write = false, .param_count = 0, .parameters = param_list_int_to_double, .return_type = return_type_int_to_double };

	func_table_insert(func_table, int_to_double);


	// func Double2Int(_ term ∶ Double) -> Int
	String str_double_to_int;
	init_string(&str_double_to_int);
	string_append(&str_double_to_int, "Int2Double");

	SymbolRecord* sr_double_to_int;
	symboltable_insert(symtab, &str_double_to_int, &sr_double_to_int);

	DataType return_type_double_to_int = { .nil_allowed = false, .type = VARTYPE_INT };

	String str_param_double_to_int;
	init_string(&str_param_double_to_int);
	string_append(&str_param_double_to_int, "term");

	SymbolRecord* sr_param_double_to_int;
	symboltable_insert(symtab, &str_double_to_int, &sr_param_double_to_int);

	DataType dt_param_double_to_int = { .nil_allowed = false, .type = VARTYPE_DOUBLE };

	Parameter param_double_term = { .extern_id = NULL, .intern_id = sr_param_double_to_int, .type = dt_param_double_to_int };

	Parameter* param_list_double_to_int = (Parameter*)malloc(sizeof(Parameter) * 1);
	if (param_list_double_to_int == NULL) {
		exit(99);
	}

	param_list_double_to_int[0] = param_double_term;

	Function double_to_int = { .id = sr_double_to_int, .is_defined = true, .is_write = false, .param_count = 0, .parameters = param_list_double_to_int, .return_type = return_type_double_to_int };

	func_table_insert(func_table, double_to_int);


	// func length(_ s : String) -> Int
	String str_length;
	init_string(&str_length);
	string_append(&str_length, "length");

	SymbolRecord* sr_length;
	symboltable_insert(symtab, &str_length, &sr_length);

	DataType return_type_length = { .nil_allowed = false, .type = VARTYPE_INT };

	String str_param_length;
	init_string(&str_param_length);
	string_append(&str_param_length, "s");

	SymbolRecord* sr_param_length;
	symboltable_insert(symtab, &str_length, &sr_param_length);

	DataType dt_param_length = { .nil_allowed = false, .type = VARTYPE_STRING };

	Parameter param_s = { .extern_id = NULL, .intern_id = sr_param_length, .type = dt_param_length };

	Parameter* param_list_length = (Parameter*)malloc(sizeof(Parameter) * 1);
	if (param_list_length == NULL) {
		exit(99);
	}

	param_list_length[0] = param_s;

	Function length = { .id = sr_length, .is_defined = true, .is_write = false, .param_count = 1, .parameters = param_list_length, .return_type = return_type_length };

	func_table_insert(func_table, length);



	// func substring(of s : String, startingAt i : Int, endingBefore j : Int) -> String?
	String str_substring;
	init_string(&str_substring);
	string_append(&str_substring, "substring");

	SymbolRecord* sr_substring;
	symboltable_insert(symtab, &str_substring, &sr_substring);

	DataType return_type_substring = { .nil_allowed = true, .type = VARTYPE_STRING };

	// param of
	String str_param_of;
	init_string(&str_param_of);
	string_append(&str_param_of, "of");

	SymbolRecord* sr_param_of;
	symboltable_insert(symtab, &str_param_of, &sr_param_of);

	String str_param_s;
	init_string(&str_param_s);
	string_append(&str_param_s, "s");

	SymbolRecord* sr_param_s;
	symboltable_insert(symtab, &str_param_s, &sr_param_s);

	DataType dt_param_of = { .nil_allowed = false, .type = VARTYPE_STRING };
	Parameter param_1 = { .extern_id = sr_param_of, .intern_id = sr_param_s, .type = dt_param_of };

	// param startingAt
	String str_param_starting_at;
	init_string(&str_param_starting_at);
	string_append(&str_param_starting_at, "startingAt");

	SymbolRecord* sr_param_starting_at;
	symboltable_insert(symtab, &str_param_starting_at, &sr_param_starting_at);

	String str_param_i;
	init_string(&str_param_i);
	string_append(&str_param_i, "i");

	SymbolRecord* sr_param_i;
	symboltable_insert(symtab, &str_param_i, &sr_param_i);

	DataType dt_param_starting_at = { .nil_allowed = false, .type = VARTYPE_INT };
	Parameter param_2 = { .extern_id = sr_param_starting_at, .intern_id = sr_param_i, .type = dt_param_starting_at };

	// param endingBefore
	String str_param_ending_before;
	init_string(&str_param_ending_before);
	string_append(&str_param_ending_before, "endingBefore");

	SymbolRecord* sr_param_ending_before;
	symboltable_insert(symtab, &str_param_ending_before, &sr_param_ending_before);

	String str_param_j;
	init_string(&str_param_j);
	string_append(&str_param_j, "j");

	SymbolRecord* sr_param_j;
	symboltable_insert(symtab, &str_param_j, &sr_param_j);

	DataType dt_param_ending_before = { .nil_allowed = false, .type = VARTYPE_INT };

	Parameter param_3 = { .extern_id = sr_param_ending_before, .intern_id = sr_param_j, .type = dt_param_ending_before };

	Parameter* param_list_substring = (Parameter*)malloc(sizeof(Parameter) * 3);
	if (param_list_substring == NULL) {
		exit(99);
	}

	param_list_substring[0] = param_1;
	param_list_substring[1] = param_2;
	param_list_substring[2] = param_3;

	Function substring = { .id = sr_substring, .is_defined = true, .is_write = false, .param_count = 3, .parameters = param_list_substring, .return_type = return_type_substring };
	func_table_insert(func_table, substring);



	// func ord(_ c : String) -> Int
	String str_ord;
	init_string(&str_ord);
	string_append(&str_ord, "ord");

	SymbolRecord* sr_ord;
	symboltable_insert(symtab, &str_ord, &sr_ord);

	DataType return_type_ord = { .nil_allowed = false, .type = VARTYPE_INT };

	String str_param_ord;
	init_string(&str_param_ord);
	string_append(&str_param_ord, "c");

	SymbolRecord* sr_param_ord;
	symboltable_insert(symtab, &str_ord, &sr_param_ord);

	DataType dt_param_ord = { .nil_allowed = false, .type = VARTYPE_STRING };

	Parameter param_c = { .extern_id = NULL, .intern_id = sr_param_ord, .type = dt_param_ord };

	Parameter* param_list_ord = (Parameter*)malloc(sizeof(Parameter) * 1);
	if (param_list_ord == NULL) {
		exit(99);
	}

	param_list_ord[0] = param_c;

	Function ord = { .id = sr_ord, .is_defined = true, .is_write = false, .param_count = 1, .parameters = param_list_ord, .return_type = return_type_ord };

	func_table_insert(func_table, ord);


	// func chr(_ i : Int) -> String
	String str_chr;
	init_string(&str_chr);
	string_append(&str_chr, "chr");

	SymbolRecord* sr_chr;
	symboltable_insert(symtab, &str_chr, &sr_chr);

	DataType return_type_chr = { .nil_allowed = false, .type = VARTYPE_INT };

	String str_param_chr;
	init_string(&str_param_chr);
	string_append(&str_param_chr, "i");

	SymbolRecord* sr_param_chr;
	symboltable_insert(symtab, &str_chr, &sr_param_chr);

	DataType dt_param_chr = { .nil_allowed = false, .type = VARTYPE_STRING };

	Parameter param_i = { .extern_id = NULL, .intern_id = sr_param_chr, .type = dt_param_chr };

	Parameter* param_list_chr = (Parameter*)malloc(sizeof(Parameter) * 1);
	if (param_list_chr == NULL) {
		exit(99);
	}

	param_list_chr[0] = param_i;

	Function chr = { .id = sr_chr, .is_defined = true, .is_write = false, .param_count = 1, .parameters = param_list_chr, .return_type = return_type_chr };

	func_table_insert(func_table, chr);
}

int parse_statement(
	Input* input, SymbolTable* symtab, Statement** statement,
	VarTableStack* var_table_stack, FuncTable* func_table,
	Statement* current_function, size_t block_counter, RuleTable* rule_table
) {



	Token _token;
	Token* token = &_token;

	Token out_token;
	bool out_token_returned;

	Expression* exp;

	int ret;

	get_token(input, symtab, token);

	PARSE_POTENTIAL_NEWLINE(input, symtab, token);

	if (token->type == TOKENTYPE_EOF) {
		return -1;
	}

	else if (token->type == TOKENTYPE_BRACE_R) {
		return -2; // -2 oznacuje "}"
	}

	*statement = (Statement*)malloc(sizeof(Statement));

	if (*statement == NULL) {
		exit(99);
	}

	(*statement)->next = NULL;

	// <statement> -> if [\n] let [\n] <id> [\n] { [\n] <statementList> [\n] } [\n] [else [\n] { [\n] <statementList> [\n] }]
	// <statement> -> if [\n] <exp> [\n] { [\n] <statementList> [\n] } [\n] [else [\n] { [\n] <statementList> [\n] }]
	if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_IF) {
		(*statement)->type = ST_IF;

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_LET) {

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_ID) {
				PRINT_ERROR(TOKENTYPE_ID, token->type);
				return 2;
			}

			(*statement)->if_.check_nil = true;

		}
		else {
			(*statement)->if_.check_nil = false;
		}

		ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned, rule_table);

		if (ret != 0) {
			return ret;
		}

		(*statement)->if_.exp = exp;

		if (do_semantic_analysis) {
			ret = semantic_if(var_table_stack, func_table, *statement);// return value = jestli prosla
			if (ret != 0) {
				return ret;
			}
		}

		if (out_token_returned) {
			token = &out_token;
		}
		else {
			get_token(input, symtab, token);
		}

		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type != TOKENTYPE_BRACE_L) {
			PRINT_ERROR(TOKENTYPE_BRACE_L, token->type);
			return 2;
		}

		// parse if body
		ret = parse_statement_list(input,
			symtab, &(*statement)->if_.body,
			var_table_stack, func_table,
			*statement, current_function, ++block_counter, rule_table
		);

		if (ret != -2) { // -2 == skoncilo s '}' to je spravne
			PRINT_TEXT("parse list neskoncilo s }");
			return ret;
		}

		// rozsireni nepovinny else
		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_ELSE) {

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_BRACE_L) {
				PRINT_ERROR(TOKENTYPE_BRACE_L, token->type);
				return 2;
			}

			// zacatek scope
			VarTable var_table;
			init_var_table(&var_table);
			vartable_stack_push(var_table_stack, &var_table);

			// parse else body
			ret = parse_statement_list(input,
				symtab, &(*statement)->if_.else_,
				var_table_stack, func_table,
				*statement, current_function, ++block_counter, rule_table
			);

			if (ret != -2) {
				PRINT_TEXT("parse list neskoncilo s }");
				return ret;
			}

			vartable_stack_pop(var_table_stack, NULL);

		}
		else {
			(*statement)->if_.else_ = NULL;
		}

		return 0;
	}

	// <statement> -> while [\n] <exp> [\n] { [\n] <statementList> [\n] }
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_WHILE) {
		(*statement)->type = ST_WHILE;

		Expression* exp;

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned, rule_table);

		if (do_semantic_analysis) {
			//TODO: ret = semantic_condition(VarTableStack, FunctionTable, Statement) return value = jestli prosla
		}

		(*statement)->while_.exp = exp;

		if (ret != 0) {
			return ret;
		}

		if (out_token_returned) {
			token = &out_token;
		}
		else {
			get_token(input, symtab, token);
		}

		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_BRACE_L) {
			// zacatek scope
			VarTable vartable;
			init_var_table(&vartable);
			vartable_stack_push(var_table_stack, &vartable);

			ret = parse_statement_list(input,
				symtab, &(*statement)->while_.body,
				var_table_stack, func_table,
				*statement, current_function, ++block_counter, rule_table
			);

			if (ret != -2) {
				PRINT_TEXT("parse list neskoncilo s }");
				return ret;
			}

			vartable_stack_pop(var_table_stack, NULL);

			return 0;
		}

		PRINT_ERROR(TOKENTYPE_BRACE_L, token->type);
		return 2;
	}

	// <statement> -> let [\n] <id> [\n] [: <type>] [\n] = [\n] <exp> \n
	// <statement> -> var [\n] <id> [\n] [: <type>] [\n]  = [\n] <exp> \n
	// <statement> -> var [\n] <id> [\n] : <type> [\n] [= [\n] <exp>] \n
	else if (token->type == TOKENTYPE_KEYWORD && (token->value.keyword == KEYWORD_LET || token->value.keyword == KEYWORD_VAR)) {
		(*statement)->type = ST_VAR;

		// pridani hodnoty zanoreni a nazvu funkce do var.id_prefix
		(*statement)->var.id_prefix.block_counter = block_counter;
		if (current_function == NULL) {
			(*statement)->var.id_prefix.func_id = NULL;
		}
		else {
			(*statement)->var.id_prefix.func_id = current_function->func.id;
		}

		(*statement)->var.modifiable = token->value.keyword == KEYWORD_VAR;

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_ID) {

			(*statement)->var.id = token->value.id;

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type == TOKENTYPE_COLON) {
				get_token(input, symtab, token);
				PARSE_POTENTIAL_NEWLINE(input, symtab, token);

				if (token->type == TOKENTYPE_KEYWORD) {

					switch (token->value.keyword) {
					case KEYWORD_DOUBLE:
						(*statement)->var.data_type.type = VARTYPE_DOUBLE;
						break;
					case KEYWORD_INT:
						(*statement)->var.data_type.type = VARTYPE_INT;
						break;
					case KEYWORD_STRING:
						(*statement)->var.data_type.type = VARTYPE_STRING;
						break;
					default:
						PRINT_ERROR(KEYWORD_DOUBLE, token->value.keyword);
						return 2;
					}

					get_token(input, symtab, token);

					if (token->type == TOKENTYPE_QUESTIONMARK) {
						//TODO: Double \n ? byt nemuze ne?
						(*statement)->var.data_type.nil_allowed = true;
						get_token(input, symtab, token);
					}
					else {
						(*statement)->var.data_type.nil_allowed = false;
					}

					// var a : Int \n => correct
					if ((*statement)->var.modifiable && token->type == TOKENTYPE_NEWLINE) {
						return 0;

					}
					else if (!(*statement)->var.modifiable && token->type == TOKENTYPE_NEWLINE) {
						// let a : Int \n => error
						PRINT_TEXT("NASTALO let a : Int \n");
						return 2;
					}
				}
				else {
					PRINT_ERROR(TOKENTYPE_KEYWORD, token->type);
					return 2;
				}
			}
			else {
				(*statement)->var.data_type.type = VARTYPE_VOID;
			}

			if (token->type == TOKENTYPE_EQUALS) {
				get_token(input, symtab, token);
				PARSE_POTENTIAL_NEWLINE(input, symtab, token);

				ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned, rule_table);

				if (ret != 0) {
					return ret;
				}

				(*statement)->var.exp = exp;

				if (do_semantic_analysis) {
					ret = semantic_variable(var_table_stack, *statement, symtab, func_table); //return value = jestli prosla

					if (ret != 0)
						return ret;
				}

				if (out_token_returned) {
					token = &out_token;
				}
				else {
					get_token(input, symtab, token);
				}

				if (token->type == TOKENTYPE_NEWLINE) {
					return 0;
				}
			}
		}
		PRINT_ERROR(TOKENTYPE_ID, token->type);
		return 2;
	}

	// <statement> -> return [<exp>] \n
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_RETURN) {
		(*statement)->type = ST_RETURN;

		get_token(input, symtab, token);

		if (token->type == TOKENTYPE_NEWLINE) {
			(*statement)->return_.exp = NULL;
			if (do_semantic_analysis) {
				//TODO: ret = semantic_return(VarTableStack, FunctionTable, Statement) return value = jestli prosla
			}
			return 0;
		}

		ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned, rule_table);

		if (do_semantic_analysis) {
			//TODO: ret = semantic_return(VarTableStack, FunctionTable, Statement) return value = jestli prosla
		}
		(*statement)->return_.exp = exp;

		if (ret != 0) {
			return ret;
		}

		if (out_token_returned) {
			token = &out_token;
		}
		else {
			get_token(input, symtab, token);
		}

		if (token->type == TOKENTYPE_NEWLINE) {
			return 0;
		}

		PRINT_ERROR(TOKENTYPE_NEWLINE, token->type);
		return 2;
	}

	// <statement> -> <id> [\n] = [\n] <exp> \n
	else if (token->type == TOKENTYPE_ID) {
		(*statement)->type = ST_ASSIGN;

		(*statement)->assign.id = token->value.id;

		Token* token_1 = token;

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_PAR_L) {

			ret = parse_expression(input, symtab, &exp, token_1, token, &out_token, &out_token_returned, rule_table);

			(*statement)->assign.exp = exp;

			if (ret != 0) {
				return ret;
			}

			return 0;
		}

		else if (token->type == TOKENTYPE_EQUALS) {

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned, rule_table);

			if (do_semantic_analysis) {
				//TODO: ret = semantic_assignment(VarTableStack, FunctionTable, Statement) return value = jestli prosla
			}

			(*statement)->assign.exp = exp;

			if (ret != 0) {
				return ret;
			}

			if (out_token_returned) {
				token = &out_token;
			}
			else {
				get_token(input, symtab, token);
			}

			if (token->type == TOKENTYPE_NEWLINE) {
				return 0;
			}

			PRINT_ERROR(TOKENTYPE_NEWLINE, token->type);
			return 2;
		}

		PRINT_TEXT("OCEKAVANO ( NEBO == ");
		return 2;
	}

	// <func> -> func <id> ( [<id> <id> : <type>] ) [-> <type>] { <statementList> }
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_FUNC) {

		(*statement)->type = ST_FUNC;

		get_token(input, symtab, token);

		// zacatek scope
		VarTable vartable;
		init_var_table(&vartable);
		vartable_stack_push(var_table_stack, &vartable);

		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_ID) {

			(*statement)->func.id = token->value.id;

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_PAR_L) {
				PRINT_ERROR(TOKENTYPE_PAR_L, token->type);
				return 2;
			}

			ret = parse_parameters(input, symtab, *statement);

			if (ret != -3) { // -3 == skoncilo ')'
				PRINT_TEXT("NESKONCILO S )");
				return 2;
			}

			get_token(input, symtab, token);

			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_ARROW && token->type != TOKENTYPE_BRACE_L) {
				PRINT_TEXT("OCEKAVNO -> NEBO {");
				return 2;
			}

			if (token->type == TOKENTYPE_ARROW) {

				get_token(input, symtab, token);
				PARSE_POTENTIAL_NEWLINE(input, symtab, token);

				if (token->type != TOKENTYPE_KEYWORD) {
					PRINT_ERROR(TOKENTYPE_KEYWORD, token->type);
					return 2;
				}

				DataType dt = { .type = VARTYPE_VOID, .nil_allowed = false };

				switch (token->value.keyword) {
				case KEYWORD_DOUBLE:
					dt.type = VARTYPE_DOUBLE;
					break;
				case KEYWORD_INT:
					dt.type = VARTYPE_INT;
					break;
				case KEYWORD_STRING:
					dt.type = VARTYPE_STRING;
					break;
				default:
					PRINT_TEXT("OCEKAVANO DOUBLE, INT, STRING");
					return 2;
				}



				get_token(input, symtab, token);
				if (token->type == TOKENTYPE_QUESTIONMARK) {
					dt.nil_allowed = true;
					get_token(input, symtab, token);
				}
				else {
					dt.nil_allowed = false;
				}

				PARSE_POTENTIAL_NEWLINE(input, symtab, token);

				(*statement)->func.return_type = dt;

			}

			if (token->type == TOKENTYPE_BRACE_L) {
				ret = parse_statement_list(input,
					symtab, &(*statement)->func.body,
					var_table_stack, func_table,
					*statement, *statement, ++block_counter, rule_table
				);


				if (ret != -2) {

					return ret;
				}

				vartable_stack_pop(var_table_stack, NULL);

				return 0;
			}
		}
		PRINT_ERROR(TOKENTYPE_ID, token->type);
		return 2;
	}

	PRINT_TEXT("NEMATCHNULO NIC");
	return 2;
}

// pokud jsem v bloku, nemuzu udelat return ani definici funkce
// pokud jsem ve funkci, nemuzu udelat definici funkce
// current_scope == NULL => jsme v globalni
// out_token => vraci prvni precteny token, ktery uz nepatri statement listu (napr. EOF, {, Int, Double, String,...)
int parse_statement_list(Input* input, SymbolTable* symtab, Statement** statement,
	VarTableStack* var_table_stack, FuncTable* func_table,
	Statement* current_scope, Statement* current_function, size_t block_counter, RuleTable* rule_table
) {
	int ret = 0;



	Statement* st;
	Statement** next_st = statement;

	while (ret == 0) {
		st = NULL;
		ret = parse_statement(input,
			symtab, &st,
			var_table_stack,
			func_table,
			current_function, block_counter, rule_table
		);

		if (ret)
			break;

		if (st->type == ST_FUNC && current_scope != NULL) {
			return 2;
		}
		else if (st->type == ST_RETURN && !current_function) {
			return 2;
		}

		*next_st = st;
		next_st = &st->next;
	}

	if (ret == -1 && current_scope != NULL) { // ret == -1 znamena EOF
		return 2;

	}
	else if (ret == -2 && current_scope == NULL) { // ret == -2 znamena '}'
		return 2;
	}

	return ret;
}

// vraci int 0 pokud je ten exp validni
// vraci expression pointer
// vraci ukazatel na to kde skoncila
// in_token Token, ktery muze dostat od parse_statement 
// out_token Token, ktery muze vratit parse_statementu
int parse_expression(Input* input, SymbolTable* symtab, Expression** exp, Token* in_token, Token* out_token, Token* in_token_2, bool* out_token_returned, RuleTable* rule_table) {
	Token _token;
	Token* token = &_token;

	if (in_token != NULL) {
		token = in_token;
	}
	else {
		get_token(input, symtab, token);
	}

	//TODO: pokud prijdou in_token a in_token_2 prvni je dat do listu tokenu a pak teprve nacitat

	*exp = (Expression*)malloc(sizeof(Expression));
	if (*exp == NULL) {
		exit(99);
	}

	// Naplnit pole tokenů, které patří do expressionu(načítáš tokeny do tokenu, který nemůže být v expressionu)
	#define MAX_EXP_LEN 256

	bool token_accepted = true;
	Token token_list[MAX_EXP_LEN];
	size_t token_index = 0;


	if (in_token) {
		token_list[token_index++] = *in_token;
		if (in_token_2) {
			token_list[token_index++] = *in_token_2;
		}
	}

	// while (token_accepted) {
	// 	get_token(input, symtab, token);
	// 	if (token->type != TOKENTYPE_QUESTIONMARK2 ||
	// 		token->type != TOKENTYPE_EQUALS2 ||
	// 		token->type != TOKENTYPE_NOT_EQUALS ||
	// 		token->type != TOKENTYPE_LESSER ||
	// 		token->type != TOKENTYPE_GREATER ||
	// 		token->type != TOKENTYPE_LESSER_OR_EQUAL ||
	// 		token->type != TOKENTYPE_GREATER_OR_EQUAL ||
	// 		token->type != TOKENTYPE_PLUS ||
	// 		token->type != TOKENTYPE_MINUS ||
	// 		token->type != TOKENTYPE_SLASH ||
	// 		token->type != TOKENTYPE_EXCLAMATION ||
	// 		token->type != TOKENTYPE_INT ||
	// 		token->type != TOKENTYPE_STRING ||
	// 		token->type != TOKENTYPE_DOUBLE ||
	// 		token->type != TOKENTYPE_ID ||
	// 		token->type != TOKENTYPE_COMMA) {
	// 		*out_token = *token; //! correct ?
	// 		*out_token_returned = true;
	// 		token_accepted = false;
	// 	}
	// 	token_list[token_index++] = *token;
	// }

	// Máš nějaký stack terminálů a neterminálů, na spodu je terminál "$" nad tím je neterminál <exp>
	TNTStack tnt_stack_;
	TNTStack* tnt_stack = &tnt_stack_;
	init_tnt_stack(tnt_stack);
	TNT tnt_end = { .is_terminal = true, .terminal = T_END };
	tnt_stack_push(tnt_stack, &tnt_end);

	TNT tnt_exp = { .is_terminal = false, .non_terminal = NT_EXP }; // počáteční <exp>
	tnt_stack_push(tnt_stack, &tnt_exp);

	// Tvoříš strom, pomocí pomocí pravidel zjištěných z tabulky (2D pole) - Enumy na řádky a na sloupce
	ExpStack exp_stack_;
	ExpStack* exp_stack = &exp_stack_;
	Node* tree;
	init_rule_tree(&tree);
	rule_tree_insert(tree, 0, NULL);

	token_index = 0;
	// while (!(tnt_stack_is_empty(tnt_stack))) { // stack není prázdný
	// 	TNT* top;
	// 	tnt_stack_pop(tnt_stack, &top);

	// 	if (!(top->is_terminal)) { // není terminál
	// 		// podivej se do tabulky, rozepiš na stack, přidej do stromu
	// 		size_t column = 0;
	// 		if (token->type == TOKENTYPE_QUESTIONMARK2) column = T_NIL_TEST;
	// 		if (token->type == TOKENTYPE_EQUALS2) column = T_EQUAL;
	// 		if (token->type == TOKENTYPE_NOT_EQUALS) column = T_N_EQUAL;
	// 		if (token->type == TOKENTYPE_LESSER) column = T_LT;
	// 		if (token->type == TOKENTYPE_GREATER) column = T_GT;
	// 		if (token->type == TOKENTYPE_LESSER_OR_EQUAL) column = T_LTE;
	// 		if (token->type == TOKENTYPE_GREATER_OR_EQUAL) column = T_GTE;
	// 		if (token->type == TOKENTYPE_PLUS) column = T_ADD;
	// 		if (token->type == TOKENTYPE_MINUS) column = T_SUB;
	// 		if (token->type == TOKENTYPE_STAR) column = T_MULT;
	// 		if (token->type == TOKENTYPE_SLASH) column = T_DIV;
	// 		if (token->type == TOKENTYPE_EXCLAMATION) column = T_NT_EXCLAMATION;
	// 		if (token->type == TOKENTYPE_INT) column = T_INT;
	// 		if (token->type == TOKENTYPE_STRING) column = T_STRING;
	// 		if (token->type == TOKENTYPE_DOUBLE) column = T_DOUBLE;
	// 		if (token->type == TOKENTYPE_ID) column = T_ID;
	// 		Rule* r = rule_table->table[top->non_terminal][column];
	// 		for (size_t i = 0; i < 3; i++) {
	// 			if (r->valid) {
	// 				if (r->expand_to[i] != NULL) {
	// 					// pushni všechna nová pravidla na stack
	// 					tnt_stack_push(tnt_stack, r->expand_to[i]);
	// 					rule_tree_insert(tree, i, NULL);
	// 					tree = tree->children_nodes[0]; // pokaždé se posunu doleva
	// 				}
	// 			}
	// 			else {
	// 				// syntaktická chyba
	// 			}
	// 		}
	// 	}
	// 	else {
	// 		// porovnej a přidej do stromu
	// 		//! tady se to musí dávat jinak
	// 		for (size_t i = 0; i < 3; i++) {
	// 			rule_tree_insert(tree, i, NULL);
	// 		}
	// 		tree = tree->children_nodes[0]; // pokaždé se posunu doleva
	// 	}


	//}
return 0;
}

// return -3 : ukoncujici: ')'
// return 2 : syntakticka chyba
int parse_parameters(Input* input, SymbolTable* symtab, Statement* func_statement) {
	Token token_;
	Token* token = &token_;

	func_statement->func.param_count = 0;

	size_t param_cap = 128;

	func_statement->func.parameters = (Parameter*)malloc(sizeof(Parameter) * param_cap);
	if (func_statement->func.parameters == NULL) {
		exit(99);
	}

	Parameter param;


	//TODO: free_params()

	do {
		get_token(input, symtab, token);

		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (func_statement->func.param_count == 0 &&
			token->type == TOKENTYPE_PAR_R) {
			return -3;
		}

		// extern
		if (token->type == TOKENTYPE_UNDERSCORE) {
			param.extern_id = NULL;
		}
		else if (token->type == TOKENTYPE_ID) {
			param.extern_id = token->value.id;
		}
		else {
			return 2;
		}

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		//intern
		if (token->type == TOKENTYPE_ID) {
			param.intern_id = token->value.id;
		}
		else {
			return 2;
		}

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		//colon
		if (token->type != TOKENTYPE_COLON) {
			return 2;
		}

		DataType dt;

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_KEYWORD) {
			switch (token->value.keyword) {
			case KEYWORD_DOUBLE:
				dt.type = VARTYPE_DOUBLE;
				break;
			case KEYWORD_INT:
				dt.type = VARTYPE_INT;
				break;
			case KEYWORD_STRING:
				dt.type = VARTYPE_STRING;
				break;
			default:
				return 2;
			}


			get_token(input, symtab, token);

			if (token->type == TOKENTYPE_QUESTIONMARK) {
				//TODO: Double \n ? nemuze byt ne?
				dt.nil_allowed = true;
				get_token(input, symtab, token);
			}
			else {
				PARSE_POTENTIAL_NEWLINE(input, symtab, token);
				dt.nil_allowed = false;
			}

			param.type = dt;
		}

		if (func_statement->func.param_count >= param_cap) {
			param_cap *= 2;
			func_statement->func.parameters = realloc(func_statement->func.parameters, param_cap);

			if (func_statement->func.parameters == NULL) {
				exit(99);
			}
		}

		func_statement->func.parameters[func_statement->func.param_count] = param;
		func_statement->func.param_count++;

		if (token->type == TOKENTYPE_PAR_R) {
			return -3;
		}
		else if (token->type == TOKENTYPE_COMMA) {
			continue;
		}
		else {
			return 2;
		}

	} while (1);
}