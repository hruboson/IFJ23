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

#define DEBUG //TODO: ODDELAT

#ifdef DEBUG
#define PRINT_LINE printf("LINE: %d\n", __LINE__)
#else
#define PRINT_LINE
#endif

bool do_semantic_analysis = true;

int parse(Input* input, AST* ast) {
	int ret = 0;

	VarTableStack var_table_stack;
	init_vartable_stack(&var_table_stack);

	FuncTable func_table;
	init_func_table(&func_table);

	insert_builtin_functions(&func_table, &ast->symtab);

	ret = parse_statement_list(input,
		&ast->symtab, &ast->statement,
		&var_table_stack, &func_table,
		NULL, NULL, 0
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

	SymbolRecord *sr_str;
	symboltable_insert(symtab, &str_read_string, &sr_str);

	DataType return_type = {.nil_allowed = true, .type = VARTYPE_STRING };
	Function read_str = {.id = sr_str, .is_defined = true, .is_write = false, .param_count = 0, .parameters = NULL, .return_type = return_type };

	func_table_insert(func_table, read_str);

	// func readInt() -> Int?
	String str_read_int;
	init_string(&str_read_int);
	string_append(&str_read_int, "readInt");

	SymbolRecord *sr_read_int;
	symboltable_insert(symtab, &str_read_int, &sr_read_int);
	
	DataType return_type_read_int = { .nil_allowed = true, .type = VARTYPE_INT };
	Function read_int = {.id = sr_read_int, .is_defined = true, .is_write = false, .param_count = 0, .parameters = NULL, .return_type = return_type_read_int };

	func_table_insert(func_table, read_int);


	// func readDouble() -> Double?
	String str_read_double;
	init_string(&str_read_double);
	string_append(&str_read_double, "readDouble");

	SymbolRecord *sr_read_double;
	symboltable_insert(symtab, &str_read_double, &sr_read_double);
	
	DataType return_type_read_double = {.nil_allowed = true, .type = VARTYPE_DOUBLE };
	Function read_double = {.id = sr_read_double, .is_defined = true, .is_write = false, .param_count = 0, .parameters = NULL, .return_type = return_type_read_double };

	func_table_insert(func_table, read_double);

	// func write ( term_1 , term_2 , …, term_n )
	String str_write;
	init_string(&str_write);
	string_append(&str_write, "write");

	SymbolRecord *sr_write;
	symboltable_insert(symtab, &str_write, &sr_write);
	
	DataType return_type_write = {.nil_allowed = true, .type = VARTYPE_VOID };
	Function write = {.id = sr_write, .is_defined = true, .is_write = true, .param_count = 0, .parameters = NULL, .return_type = return_type_write };

	func_table_insert(func_table, write);


	// func Int2Double(_ term ∶ Int) -> Double
	String str_int_to_double;
	init_string(&str_int_to_double);
	string_append(&str_int_to_double, "Int2Double");

	SymbolRecord *sr_int_to_double;
	symboltable_insert(symtab, &str_int_to_double, &sr_int_to_double);

	DataType return_type_int_to_double = {.nil_allowed = false, .type = VARTYPE_DOUBLE };

	String str_param_int_to_double;
	init_string(&str_param_int_to_double);
	string_append(&str_param_int_to_double, "Int2Double");
	
	SymbolRecord *sr_param_int_to_double;
	symboltable_insert(symtab, &str_int_to_double, &sr_param_int_to_double);

	DataType dt_param_int_to_double = { .nil_allowed = false, .type = VARTYPE_INT };

	Parameter param_term = { .extern_id = NULL, .intern_id = sr_param_int_to_double, .type = dt_param_int_to_double };

	Parameter* param_list_int_to_double = (Parameter*)malloc(sizeof(Parameter)*1);
	if (param_list_int_to_double == NULL) {
		exit(99);
	}

	param_list_int_to_double[0] = param_term;

	Function int_to_double = {.id = sr_int_to_double, .is_defined = true, .is_write = false, .param_count = 0, .parameters =  param_list_int_to_double, .return_type = return_type_int_to_double };

	func_table_insert(func_table, int_to_double);


	// func Double2Int(_ term ∶ Double) -> Int
	String str_double_to_int;
	init_string(&str_double_to_int);
	string_append(&str_double_to_int, "Int2Double");

	SymbolRecord *sr_double_to_int;
	symboltable_insert(symtab, &str_double_to_int, &sr_double_to_int);
	
	DataType return_type_double_to_int = {.nil_allowed = false, .type = VARTYPE_INT };

	String str_param_double_to_int;
	init_string(&str_param_double_to_int);
	string_append(&str_param_double_to_int, "term");
	
	SymbolRecord *sr_param_double_to_int;
	symboltable_insert(symtab, &str_double_to_int, &sr_param_double_to_int);

	DataType dt_param_double_to_int = { .nil_allowed = false, .type = VARTYPE_DOUBLE };

	Parameter param_double_term = { .extern_id = NULL, .intern_id = sr_param_double_to_int, .type = dt_param_double_to_int };

	Parameter* param_list_double_to_int = (Parameter*)malloc(sizeof(Parameter)*1);
	if (param_list_double_to_int == NULL) {
		exit(99);
	}

	param_list_double_to_int[0] = param_double_term;

	Function double_to_int = {.id = sr_double_to_int, .is_defined = true, .is_write = false, .param_count = 0, .parameters = param_list_double_to_int, .return_type = return_type_double_to_int };

	func_table_insert(func_table, double_to_int);


	// func length(_ s : String) -> Int
	String str_length;
	init_string(&str_length);
	string_append(&str_length, "length");

	SymbolRecord *sr_length;
	symboltable_insert(symtab, &str_length, &sr_length);
	
	DataType return_type_length = {.nil_allowed = false, .type = VARTYPE_INT };

	String str_param_length;
	init_string(&str_param_length);
	string_append(&str_param_length, "s");
	
	SymbolRecord *sr_param_length;
	symboltable_insert(symtab, &str_length, &sr_param_length);

	DataType dt_param_length = { .nil_allowed = false, .type = VARTYPE_STRING };

	Parameter param_s = { .extern_id = NULL, .intern_id = sr_param_length, .type = dt_param_length };

	Parameter* param_list_length = (Parameter*)malloc(sizeof(Parameter)*1);
	if (param_list_length == NULL) {
		exit(99);
	}

	param_list_length[0] = param_s;

	Function length = {.id = sr_length, .is_defined = true, .is_write = false, .param_count = 1, .parameters = param_list_length, .return_type = return_type_length };

	func_table_insert(func_table, length);



	// func substring(of s : String, startingAt i : Int, endingBefore j : Int) -> String?
	String str_substring;
	init_string(&str_substring);
	string_append(&str_substring, "substring");

	SymbolRecord *sr_substring;
	symboltable_insert(symtab, &str_substring, &sr_substring);
	
	DataType return_type_substring = {.nil_allowed = true, .type = VARTYPE_STRING };

	// param of
	String str_param_of;
	init_string(&str_param_of);
	string_append(&str_param_of, "of");
	
	SymbolRecord *sr_param_of;
	symboltable_insert(symtab, &str_param_of, &sr_param_of);

	String str_param_s;
	init_string(&str_param_s);
	string_append(&str_param_s, "s");
	
	SymbolRecord *sr_param_s;
	symboltable_insert(symtab, &str_param_s, &sr_param_s);

	DataType dt_param_of = { .nil_allowed = false, .type = VARTYPE_STRING };
	Parameter param_1 = { .extern_id = sr_param_of, .intern_id = sr_param_s, .type = dt_param_of };

	// param startingAt
	String str_param_starting_at;
	init_string(&str_param_starting_at);
	string_append(&str_param_starting_at, "startingAt");
	
	SymbolRecord *sr_param_starting_at;
	symboltable_insert(symtab, &str_param_starting_at, &sr_param_starting_at);

	String str_param_i;
	init_string(&str_param_i);
	string_append(&str_param_i, "i");
	
	SymbolRecord *sr_param_i;
	symboltable_insert(symtab, &str_param_i, &sr_param_i);

	DataType dt_param_starting_at = { .nil_allowed = false, .type = VARTYPE_INT };
	Parameter param_2 = { .extern_id = sr_param_starting_at, .intern_id = sr_param_i, .type = dt_param_starting_at };

	// param endingBefore
	String str_param_ending_before;
	init_string(&str_param_ending_before);
	string_append(&str_param_ending_before, "endingBefore");
	
	SymbolRecord *sr_param_ending_before;
	symboltable_insert(symtab, &str_param_ending_before, &sr_param_ending_before);

	String str_param_j;
	init_string(&str_param_j);
	string_append(&str_param_j, "j");
	
	SymbolRecord *sr_param_j;
	symboltable_insert(symtab, &str_param_j, &sr_param_j);

	DataType dt_param_ending_before = { .nil_allowed = false, .type = VARTYPE_INT };

	Parameter param_3 = { .extern_id = sr_param_ending_before, .intern_id = sr_param_j, .type = dt_param_ending_before };

	Parameter* param_list_substring = (Parameter*)malloc(sizeof(Parameter)*3);
	if (param_list_substring == NULL) {
		exit(99);
	}

	param_list_substring[0] = param_1;
	param_list_substring[1] = param_2;
	param_list_substring[2] = param_3;

	Function substring = {.id = sr_substring, .is_defined = true, .is_write = false, .param_count = 3, .parameters = param_list_substring, .return_type = return_type_substring };
	func_table_insert(func_table, substring);



	// func ord(_ c : String) -> Int
	String str_ord;
	init_string(&str_ord);
	string_append(&str_ord, "ord");

	SymbolRecord *sr_ord;
	symboltable_insert(symtab, &str_ord, &sr_ord);
	
	DataType return_type_ord = {.nil_allowed = false, .type = VARTYPE_INT };

	String str_param_ord;
	init_string(&str_param_ord);
	string_append(&str_param_ord, "c");
	
	SymbolRecord *sr_param_ord;
	symboltable_insert(symtab, &str_ord, &sr_param_ord);

	DataType dt_param_ord = { .nil_allowed = false, .type = VARTYPE_STRING };

	Parameter param_c = { .extern_id = NULL, .intern_id = sr_param_ord, .type = dt_param_ord };
	
	Parameter* param_list_ord = (Parameter*)malloc(sizeof(Parameter)*1);
	if (param_list_ord == NULL) {
		exit(99);
	}
	
	param_list_ord[0] = param_c;

	Function ord = {.id = sr_ord, .is_defined = true, .is_write = false, .param_count = 1, .parameters = param_list_ord, .return_type = return_type_ord };

	func_table_insert(func_table, ord);


	// func chr(_ i : Int) -> String
	String str_chr;
	init_string(&str_chr);
	string_append(&str_chr, "chr");

	SymbolRecord *sr_chr;
	symboltable_insert(symtab, &str_chr, &sr_chr);
	
	DataType return_type_chr = {.nil_allowed = false, .type = VARTYPE_INT };

	String str_param_chr;
	init_string(&str_param_chr);
	string_append(&str_param_chr, "i");
	
	SymbolRecord *sr_param_chr;
	symboltable_insert(symtab, &str_chr, &sr_param_chr);

	DataType dt_param_chr = { .nil_allowed = false, .type = VARTYPE_STRING };

	Parameter param_i = { .extern_id = NULL, .intern_id = sr_param_chr, .type = dt_param_chr };

	Parameter* param_list_chr = (Parameter*)malloc(sizeof(Parameter)*1);
	if (param_list_chr == NULL) {
		exit(99);
	}
	
	param_list_chr[0] = param_i;

	Function chr = {.id = sr_chr, .is_defined = true, .is_write = false, .param_count = 1, .parameters = param_list_chr, .return_type = return_type_chr };

	func_table_insert(func_table, chr);
}

int parse_statement(
	Input* input, SymbolTable* symtab, Statement** statement,
	VarTableStack* var_table_stack, FuncTable* func_table,
	Statement* current_function, size_t block_counter
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

		// zacatek scope
		VarTable vartable;
		init_var_table(&vartable);
		vartable_stack_push(var_table_stack, &vartable);

		if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_LET) {

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_ID) {
				return 2;
			}

			(*statement)->if_.check_nil = true;

		}
		else {
			(*statement)->if_.check_nil = false;
		}

		ret = parse_expression(input, symtab, &exp, token, &out_token, &out_token_returned);

		if (ret != 0) {
			return ret;
		}

		(*statement)->if_.exp = exp;

		if (do_semantic_analysis) {
			ret = semantic_if(var_table_stack, func_table, *statement);// return value = jestli prosla
			if(ret != 0){
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
			return 2;
		}

		// parse if body
		ret = parse_statement_list(input,
			symtab, &(*statement)->if_.body,
			var_table_stack, func_table,
			*statement, current_function, ++block_counter
		);

		if (ret != -2) { // -2 == skoncilo s '}' to je spravne
			return 2;
		}

		// rozsireni nepovinny else
		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_ELSE) {

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_BRACE_L) {
				return 2;
			}

			// parse else body
			ret = parse_statement_list(input,
				symtab, &(*statement)->if_.else_,
				var_table_stack, func_table,
				*statement, current_function, ++block_counter
			);

			if (ret != -2) {
				return 2;
			}

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

		// zacatek scope
		VarTable vartable;
		init_var_table(&vartable);
		vartable_stack_push(var_table_stack, &vartable);

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		ret = parse_expression(input, symtab, &exp, token, &out_token, &out_token_returned);

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
			ret = parse_statement_list(input,
				symtab, &(*statement)->while_.body,
				var_table_stack, func_table,
				*statement, current_function, ++block_counter
			);

			if (ret != -2) {
				return 2;
			}

			return 0;
		}

		return 2;
	}

	// <statement> -> let [\n] <id> [\n] [: <type>] [\n] = [\n] <exp> \n
	// <statement> -> var [\n] <id> [\n] [: <type>] [\n]  = [\n] <exp> \n
	// <statement> -> var [\n] <id> [\n] : <type> [\n] [= [\n] <exp>] \n
	else if (token->type == TOKENTYPE_KEYWORD && (token->value.keyword == KEYWORD_LET || token->value.keyword == KEYWORD_VAR)) {
		(*statement)->type = ST_VAR;

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
						(*statement)->var.data_type = VARTYPE_DOUBLE;
						break;
					case KEYWORD_INT:
						(*statement)->var.data_type = VARTYPE_INT;
						break;
					case KEYWORD_STRING:
						(*statement)->var.data_type = VARTYPE_STRING;
						break;
					default:
						return 2;
					}

					get_token(input, symtab, token);

					if (token->type == TOKENTYPE_QUESTIONMARK) {
						//TODO: Double \n ? byt nemuze ne?
						(*statement)->var.allow_nil = true;
						get_token(input, symtab, token);
					}
					else {
						(*statement)->var.allow_nil = false;
					}

					// var a : Int \n => correct
					if ((*statement)->var.modifiable && token->type == TOKENTYPE_NEWLINE) {
						return 0;

					}
					else if (!(*statement)->var.modifiable && token->type == TOKENTYPE_NEWLINE) {
						// let a : Int \n => error
						return 2;
					}
				}
				else {
					return 2;
				}
			}
			else {
				(*statement)->var.data_type = VARTYPE_VOID;
			}

			if (token->type == TOKENTYPE_EQUALS) {
				get_token(input, symtab, token);
				PARSE_POTENTIAL_NEWLINE(input, symtab, token);

				ret = parse_expression(input, symtab, &exp, token, &out_token, &out_token_returned);

				if (do_semantic_analysis) {
					//TODO: ret = semantic_variable(VarTableStack, FunctionTable, Statement) return value = jestli prosla
				}
				(*statement)->var.exp = exp;

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
			}
		}

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

		ret = parse_expression(input, symtab, &exp, token, &out_token, &out_token_returned);

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

		return 2;
	}

	// <statement> -> <id> [\n] = [\n] <exp> \n
	else if (token->type == TOKENTYPE_ID) {
		(*statement)->type = ST_ASSIGN;

		(*statement)->assign.id = token->value.id;

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_EQUALS) {

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			ret = parse_expression(input, symtab, &exp, token, &out_token, &out_token_returned);

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

			return 2;
		}
	}

	// <func> -> func <id> ( [<id> <id> : <type>] ) [-> <type>] { <statementList> }
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_FUNC) {
		(*statement)->type = ST_FUNC;

		get_token(input, symtab, token);

		//TODO: <statement> -> <func> () volani funkce
		//TODO: muze byt <func> \n () ??

		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_ID) {
			// zacatek scope
			VarTable vartable;
			init_var_table(&vartable);
			vartable_stack_push(var_table_stack, &vartable);

			(*statement)->func.id = token->value.id;

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_PAR_L) {
				return 2;
			}

			ret = parse_parameters(input, symtab, *statement);

			if (ret != -3) { // -3 == skoncilo ')'
				return 2;
			}

			get_token(input, symtab, token);

			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_ARROW && token->type != TOKENTYPE_BRACE_L) {
				return 2;
			}

			if (token->type == TOKENTYPE_ARROW) {

				get_token(input, symtab, token);
				PARSE_POTENTIAL_NEWLINE(input, symtab, token);

				if (token->type != TOKENTYPE_KEYWORD) {
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
					PRINT_LINE;
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
					*statement, *statement, ++block_counter
				);


				if (ret != -2) {
					return 2;
				}

				return 0;
			}
		}
		return 2;
	}

	return 2;
}

// pokud jsem v bloku, nemuzu udelat return ani definici funkce
// pokud jsem ve funkci, nemuzu udelat definici funkce
// current_scope == NULL => jsme v globalni
// out_token => vraci prvni precteny token, ktery uz nepatri statement listu (napr. EOF, {, Int, Double, String,...)
int parse_statement_list(Input* input, SymbolTable* symtab, Statement** statement,
	VarTableStack* var_table_stack, FuncTable* func_table,
	Statement* current_scope, Statement* current_function, size_t block_counter
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
			current_function, block_counter
		);

		if (ret)
			break;

		// pridani hodnoty zanoreni a nazvu funkce do var.id_prefix
		if (st->type == ST_VAR) {
			st->var.id_prefix.block_counter = block_counter;
			if (current_function == NULL) {
				st->var.id_prefix.func_id = NULL;
			}
			else {
				st->var.id_prefix.func_id = current_function->func.id;
			}
		}

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
int parse_expression(Input* input, SymbolTable* symtab, Expression** exp, Token* in_token, Token* out_token, bool* out_token_returned) {
	Token _token;
	Token* token = &_token;
	if (in_token != NULL) {
		token = in_token;
	} else {
		get_token(input, symtab, token);
	}
	*exp = (Expression*)malloc(sizeof(Expression));
	if (*exp == NULL) {
		exit(99);
	}
	switch (token->type) {
		case TOKENTYPE_DOUBLE:
			(*exp)->type = ET_DOUBLE;
			(*exp)->double_ = token->value.double_;
			break;
		case TOKENTYPE_INT:
			(*exp)->type = ET_INT;
			(*exp)->int_ = token->value.int_;
			break;
		case TOKENTYPE_STRING:
			(*exp)->type = ET_STRING;
			(*exp)->str_ = token->value.str_;
			break;
		case TOKENTYPE_KEYWORD:
			if (token->value.keyword != KEYWORD_NIL) {
				return 2;
			}
			(*exp)->type = ET_NIL;
			break;
		case TOKENTYPE_ID:
			(*exp)->type = ET_ID;
			(*exp)->id = token->value.id;
			break;
		default:
			return 2;
	}
	//TODO: out_token
	*out_token_returned = false;
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
			printf("TOKEN: %d\n", token->type);
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
