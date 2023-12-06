/**
 * Implementace překladače imperativního jazyka IFJ23
 * xjestr04 Radek Jestřabík
*/

#include "semantic.h"
#include "AST.h"

#include <stdlib.h>

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

