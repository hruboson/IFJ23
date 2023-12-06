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
#include <assert.h>

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
		NULL, NULL, 0
	);

	if (ret == -1) {
		ret = 0;
	} else {

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
	} else if (token->type == TOKENTYPE_BRACE_R) {
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

		} else {
			(*statement)->if_.check_nil = false;
		}

		ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned);
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
		} else {
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
			*statement, current_function, ++block_counter
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
				*statement, current_function, ++block_counter
			);

			if (ret != -2) {
				PRINT_TEXT("parse list neskoncilo s }");
				return ret;
			}

			vartable_stack_pop(var_table_stack, NULL);

		} else {
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

		ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned);

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
				*statement, current_function, ++block_counter
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

				ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned);

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

		ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned);

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

			ret = parse_expression(input, symtab, &exp, token_1, token, &out_token, &out_token_returned);

			(*statement)->assign.exp = exp;

			if (ret != 0) {
				return ret;
			}

			return 0;
		}

		else if (token->type == TOKENTYPE_EQUALS) {

			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			ret = parse_expression(input, symtab, &exp, token, NULL, &out_token, &out_token_returned);

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
					*statement, *statement, ++block_counter
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

		if (st->type == ST_FUNC && current_scope != NULL) {
			return 2;
		} else if (st->type == ST_RETURN && !current_function) {
			return 2;
		}

		*next_st = st;
		next_st = &st->next;
	}

	if (ret == -1 && current_scope != NULL) { // ret == -1 znamena EOF
		return 2;
	} else if (ret == -2 && current_scope == NULL) { // ret == -2 znamena '}'
		return 2;
	}

	return ret;
}

void
print_token( const Token* t ) {
	switch ( t->type ) {
	case TOKENTYPE_NEWLINE: printf( "NL" ); break;
	case TOKENTYPE_EOF: printf( "EOF" ); break;
	case TOKENTYPE_PLUS: printf( "+" ); break;
	case TOKENTYPE_MINUS: printf( "-" ); break;
	case TOKENTYPE_STAR: printf( "*" ); break;
	case TOKENTYPE_SLASH: printf( "/" ); break;
	case TOKENTYPE_QUESTIONMARK2: printf( "??" ); break;
	case TOKENTYPE_EQUALS2: printf( "==" ); break;
	case TOKENTYPE_NOT_EQUALS: printf( "!=" ); break;
	case TOKENTYPE_LESSER: printf( "<" ); break;
	case TOKENTYPE_GREATER: printf( ">" ); break;
	case TOKENTYPE_LESSER_OR_EQUAL: printf( "<=" ); break;
	case TOKENTYPE_GREATER_OR_EQUAL: printf( ">=" ); break;
	case TOKENTYPE_EXCLAMATION: printf( "!" ); break;
	case TOKENTYPE_QUESTIONMARK: printf( "?" ); break;
	case TOKENTYPE_EQUALS: printf( "=" ); break;
	case TOKENTYPE_ARROW: printf( "->" ); break;
	case TOKENTYPE_COLON: printf( ":" ); break;
	case TOKENTYPE_PAR_L: printf( "(" ); break;
	case TOKENTYPE_PAR_R: printf( ")" ); break;
	case TOKENTYPE_BRACE_L: printf( "{" ); break;
	case TOKENTYPE_BRACE_R: printf( "}" ); break;
	case TOKENTYPE_COMMA: printf( "," ); break;
	case TOKENTYPE_UNDERSCORE: printf( "_" ); break;
	case TOKENTYPE_ID:
		printf( "id( %s )", t->value.id->symbol.data );
		break;
	case TOKENTYPE_STRING:
		printf( "string( \"" );
		for ( size_t i = 0; i < t->value.str_.length; i++ ) {
			char c = t->value.str_.data[ i ];
			if ( c <= 31 )
				printf( "\\u{%x}", c );
			else
				printf( "%c", c );
		}
		printf( "\" )" );
		break;
	case TOKENTYPE_INT:
		printf( "int( %i )", t->value.int_ );
		break;
	case TOKENTYPE_DOUBLE:
		printf( "double( %f )", t->value.double_ );
		break;
	case TOKENTYPE_KEYWORD:
		printf( "KEYWORD" );
		break;
	default:
		printf( "DEFAULT(%i)", t->type );
	}
}

void
print_rule_tree( const Node* n, size_t i ) {
	if ( n == NULL )
		return;

	for ( size_t j = 0; j < i; j++ )
		printf( "  " );

	printf( "%s ", n->isTerminal ? "T" : "NT" );
	if ( n->isTerminal ) {
		print_token( n->val );

		printf( "\n" );

		return;
	} else {
		printf( "%i", n->nt );

		printf( "\n" );
		for ( size_t ii = 0; ii < 3; ii++ )
			print_rule_tree( n->children_nodes[ ii ], i + 1 );

		return;
	}

}

const char*
terminal_to_string( Terminal t ) {
	switch ( t ) {
	case T_NIL_TEST: return "??";
	case T_EQUAL: return "==";
	case T_N_EQUAL: return "!=";
	case T_LT: return "<";
	case T_GT: return ">";
	case T_LTE: return "<=";
	case T_GTE: return ">=";
	case T_ADD: return "+";
	case T_SUB: return "-";
	case T_MULT: return "*";
	case T_DIV: return "/";
	case T_NT_EXCLAMATION: return "!";
	case T_PAR_L: return "(";
	case T_INT: return "int";
	case T_STRING: return "string";
	case T_DOUBLE: return "double";
	case T_ID: return "id";
	case T_COMMA: return ",";
	case T_PAR_R: return ")";
	case T_END: return "$";
	default: return "DEFAULT";
	}
}

const char*
nonterminal_to_string( NonTerminal nt ) {
	switch ( nt ) {
	case NT_EXP: return "<exp>";
	case NT_EXP_: return "<exp'>";
	case NT_EXP1: return "<exp1>";
	case NT_EXP1_: return "<exp1'>";
	case NT_EXP2: return "<exp2>";
	case NT_EXP2_: return "<exp2'>";
	case NT_EXP3: return "<exp3>";
	case NT_EXP3_: return "<exp3'>";
	case NT_EXP4: return "<exp4>";
	case NT_EXP4_: return "<exp4'>";
	case NT_EXP5: return "<exp5>";
	case NT_ARGS: return "<args>";
	case NT_ARGS_LIST: return "<args_list>";
	case NT_E_ID: return "<e_id>";
	case NT_ARG_LIST_N: return "<arg_list_n>";
	case NT_EXP_ID: return "<exp_id>";
	default: return "DEFAULT";
	}
}

const char*
tnt_to_string( const TNT* boom ) {
	if ( boom->is_terminal )
		return terminal_to_string( boom->terminal );
	else
		return nonterminal_to_string( boom->non_terminal );
}

// vraci int 0 pokud je ten exp validni
// vraci expression pointer
// vraci ukazatel na to kde skoncila
// in_token Token, ktery muze dostat od parse_statement 
// out_token Token, ktery muze vratit parse_statementu
int parse_expression(
	Input* input, SymbolTable* symtab,
	Expression** exp, Token* in_token,
	Token* out_token, Token* in_token_2,
	bool* out_token_returned
) {
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
	size_t token_count = 0;

	Token token;

	if (in_token) {
		token_list[token_count++] = *in_token;
		if (in_token_2) {
			token_list[token_count++] = *in_token_2;
		}
	}

	while (token_accepted) {
	 	get_token(input, symtab, &token);
	 	if (
			token.type != TOKENTYPE_QUESTIONMARK2 &&
	 		token.type != TOKENTYPE_EQUALS2 &&
	 		token.type != TOKENTYPE_NOT_EQUALS &&
	 		token.type != TOKENTYPE_LESSER &&
	 		token.type != TOKENTYPE_GREATER &&
	 		token.type != TOKENTYPE_LESSER_OR_EQUAL &&
	 		token.type != TOKENTYPE_GREATER_OR_EQUAL &&
	 		token.type != TOKENTYPE_PLUS &&
	 		token.type != TOKENTYPE_MINUS &&
	 		token.type != TOKENTYPE_SLASH &&
	 		token.type != TOKENTYPE_EXCLAMATION &&
	 		token.type != TOKENTYPE_INT &&
	 		token.type != TOKENTYPE_STRING &&
	 		token.type != TOKENTYPE_DOUBLE &&
	 		token.type != TOKENTYPE_ID &&
	 		token.type != TOKENTYPE_COMMA &&
			token.type != TOKENTYPE_PAR_L &&
			token.type != TOKENTYPE_PAR_R
		) {
	 		*out_token = token; //! correct ?
	 		*out_token_returned = true;
	 		token_accepted = false;
			break;
	 	}
	 	token_list[token_count++] = token;
	}

	printf( "token count = %lu\n", token_count );
	for ( size_t i = 0; i < token_count; i++ ) {
		printf( "\t" );
		print_token( &token_list[ i ] );
		printf( "\n" );
	}

	// Máš nějaký stack terminálů a neterminálů, na spodu je terminál "$" nad tím je neterminál <exp>
	TNTStack tnt_stack_;
	TNTStack* tnt_stack = &tnt_stack_;
	init_tnt_stack(tnt_stack);

	Node* tree_root;
	init_rule_tree(&tree_root);

	tnt_stack_push( tnt_stack, ( TS_Item ){ &NT_exp, tree_root } );

	// Tvoříš strom, pomocí pomocí pravidel zjištěných z tabulky (2D pole) - Enumy na řádky a na sloupce
	ExpStack exp_stack_;
	ExpStack* exp_stack = &exp_stack_;

	token_index = 0;
	while (!(tnt_stack_is_empty(tnt_stack))) { // stack není prázdný
	 	TS_Item top;
	 	tnt_stack_pop(tnt_stack, &top);

		Token* t = token_list + token_index;
		//print_token( t );
		//printf( "\n" );

		Terminal term = T_END;
		switch ( t->type ) {
		case TOKENTYPE_QUESTIONMARK2: term = T_NIL_TEST; break;
		case TOKENTYPE_EQUALS2: term = T_EQUAL; break;
		case TOKENTYPE_NOT_EQUALS: term = T_N_EQUAL; break;
		case TOKENTYPE_LESSER: term = T_LT; break;
		case TOKENTYPE_GREATER: term = T_GT; break;
		case TOKENTYPE_LESSER_OR_EQUAL: term = T_LTE; break;
		case TOKENTYPE_GREATER_OR_EQUAL: term = T_GTE; break;
		case TOKENTYPE_PLUS: term = T_ADD; break;
		case TOKENTYPE_MINUS: term = T_SUB; break;
		case TOKENTYPE_STAR: term = T_MULT; break;
		case TOKENTYPE_SLASH: term = T_DIV; break;
		case TOKENTYPE_EXCLAMATION: term = T_NT_EXCLAMATION; break;
		case TOKENTYPE_INT: term = T_INT; break;
		case TOKENTYPE_STRING: term = T_STRING; break;
		case TOKENTYPE_DOUBLE: term = T_DOUBLE; break;
		case TOKENTYPE_ID: term = T_ID; break;
		case TOKENTYPE_PAR_L: term = T_PAR_L; break;
		case TOKENTYPE_PAR_R: term = T_PAR_R; break;
		}

	 	if ( top.tnt->is_terminal == false ) { // není terminál
	 		// podivej se do tabulky, rozepiš na stack, přidej do stromu
	 		Rule* r = get_rule( top.tnt->non_terminal, term );
			if ( r == NULL ) {
				//*(int*)NULL = 0;
				printf(
					"expression error: no rule for NT '%s' and T '%s'\n",
					nonterminal_to_string( top.tnt->non_terminal ),
					terminal_to_string( term )
				);
				return 2;
			}

			top.node->isTerminal = false;
			top.node->nt = top.tnt->non_terminal;

			printf( "%s -> ",
				nonterminal_to_string( top.tnt->non_terminal )
			);
			for ( int i = 0; i < 3; i++ ) {
				if ( r->expand_to[ i ] == NULL )
					break;
				printf( "%s ", tnt_to_string( r->expand_to[i] ) );
			}
			printf( "\n" );

	 		for (int i = 2; i >= 0; i--) {
	 			if (r->expand_to[i] != NULL) {
	 				// pushni všechna nová pravidla na stack
	 				rule_tree_insert(top.node, i, NULL);
	 				tnt_stack_push(
						tnt_stack,
						(TS_Item){ r->expand_to[i], top.node->children_nodes[i] }
					);
	 			}
	 		}

			continue;
	 	} else {

			if ( term != top.tnt->terminal ) {

				printf(
					"expression error: expected '%s' but got '%s'\n",
					terminal_to_string( top.tnt->terminal ),
					terminal_to_string( term )
				);

				//*(int*)NULL = 0;
				return 2;
			}

			if ( term == T_END )
				break;

			top.node->isTerminal = true;
			top.node->val = t;

			token_index++;

			continue;
	 	}


	}

	print_rule_tree( tree_root, 0 );

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
		} else if (token->type == TOKENTYPE_ID) {
			param.extern_id = token->value.id;
		} else {
			return 2;
		}

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		//intern
		if (token->type == TOKENTYPE_ID) {
			param.intern_id = token->value.id;
		} else {
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

		if (token->type == TOKENTYPE_PAR_R)
			return -3;
		if (token->type == TOKENTYPE_COMMA)
			continue;
		return 2;
	} while (1);
}
