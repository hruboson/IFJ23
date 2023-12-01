#include "parser.h"

#include "AST.h"

#include "token_stack.h"
#include "stdlib.h"
#include "vartable_stack.h"

// newline check
// pars: Input* input, SymbolTable* symtab, Token* token
#define PARSE_POTENTIAL_NEWLINE(input, symtab, token) do { \
	if (token->type == TOKENTYPE_NEWLINE) { \
		get_token(input, symtab, token); \
	} else { \
		break; \
	} \
} while (1)

int parse(Input* input, AST* ast) {
	int ret = 0;

	VarTableStack var_table_stack;
	init_vartable_stack(&var_table_stack);

	FuncTable func_table;
	init_func_table(&func_table);

	//TODO: bude se tady vyuzivat out_token?
	Token out_token;
	bool out_token_returned;

	ret = parse_statement_list(input,
		&ast->symtab, &ast->statement,
		&var_table_stack, &func_table, 
		NULL, false
	);

	if (ret == -1) {
		ret = 0;
	}
	else {
		// TODO: clear structs
		return 2;
	}

	// TODO: ret = semantic_check_if_called_functions_were_declared()
	// TODO: if (ret != 0) return ret;

	return ret;
}

int parse_statement(Input *input, SymbolTable* symtab, Statement** statement, VarTableStack* var_table_stack, FuncTable* func_table, bool in_function) {
	Token _token;
	Token* token = &_token;

	Token out_token;
	bool out_token_returned;

	int ret;

	get_token(input, symtab, token);
	PARSE_POTENTIAL_NEWLINE(input, symtab, token);

	if (token->type == TOKENTYPE_EOF) {
		return -1;
	}

	else if (token->type == TOKENTYPE_BRACE_R) {
		printf("END WITH -2\n");
		return -2; // -2 oznacuje "}"
	}

	*statement = (Statement*)malloc(sizeof(Statement));

	if (*statement == NULL) {
		exit(99);
	}

	(*statement)->next = NULL;

	// <statement> -> if let <id> { <statementList> } [else { <statementList> }]
	// <statement> -> if <exp> { <statementList> } [else { <statementList> }]
	// TODO check newlines
	if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_IF) {
		(*statement)->type = ST_IF;

		bool is_let = false;
		Expression exp;

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		// zacatek scope
		VarTable *vartable;
		init_var_table(vartable);
		vartable_stack_push(var_table_stack, vartable);

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_LET) {
			
			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_ID) {  // TODO: staci to takto?
				return 2;                       // TODO: ma se vracet nejake cislo != 0 oznacujici chybu? jake?
			}
			is_let = true;

		}
		
		// TODO parse exp

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type != TOKENTYPE_BRACE_L) {
			return 2;
		}

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		// parse if body
		ret = parse_statement_list(input,
			symtab, &(*statement)->if_.body,
			var_table_stack, func_table, 
			*statement, in_function
		);

		if (ret != -2) {
			return 2;
		}

		if (out_token_returned) {
			token = &out_token;
		} else {
			get_token(input, symtab, token);
		}

		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type != TOKENTYPE_BRACE_R) {
			return 2;
		}

		// bez rozsireni je vzdy else
		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_ELSE) {
			get_token(input, symtab, token);
			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			get_token(input, symtab, token);
			if (token->type == TOKENTYPE_BRACE_L) {
				return 2;
			}

			// parse else body
			ret = parse_statement_list(input,
				symtab, &(*statement)->if_.else_,
				var_table_stack, func_table, 
				*statement, in_function
			);

			if (ret != -2) {
				return 2;
			}

			if (out_token_returned) {
				token = &out_token;
			} else {
				get_token(input, symtab, token);
			}

			PARSE_POTENTIAL_NEWLINE(input, symtab, token);

			if (token->type != TOKENTYPE_BRACE_R) {
				return 2;
			}
		} else {
			(*statement)->if_.else_ = NULL;
		}

		if (is_let) {
			(*statement)->if_.exp = &exp;
			(*statement)->if_.check_nil = false;
		} else {
			(*statement)->if_.exp = NULL;
			(*statement)->if_.check_nil = true;
		}

		return 0;
	}

	// <statement> -> while [\n] <exp> [\n] { [\n] <statementList> [\n] }
	// TODO check/handle newlines
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_WHILE) {
		(*statement)->type = ST_WHILE;

		Expression *exp;

		// zacatek scope
		VarTable vartable;
		init_var_table(&vartable);
		vartable_stack_push(var_table_stack, &vartable);

		get_token(input, symtab, token);
		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		ret = parse_expression(input, symtab, &exp, token, &out_token, &out_token_returned);

		//TODO: is_valid = semantic_condition(VarTableStack, FunctionTable, Statement) return value = jestli prosla

		(*statement)->while_.exp = exp;

		if (ret != 0) {
			return ret;
		}

		if (out_token_returned) {
			token = &out_token;
		} else {
			get_token(input, symtab, token);
		}

		PARSE_POTENTIAL_NEWLINE(input, symtab, token);

		if (token->type == TOKENTYPE_BRACE_L) {
			ret = parse_statement_list( input,
					symtab, &(*statement)->while_.body,
					var_table_stack, func_table,
					*statement, in_function
				);

			if (ret != -2) {
				return 2;
			}
			
			return 0;
		}
		printf("RETURN 2: %d\n", __LINE__);
		return 2;
	}

	// <statement> -> let <id> [: <type>] = <exp> \n
	// <statement> -> var <id> [: <type>] = <exp> \n
	// <statement> -> var <id> : <type> [= <exp>] \n
	else if (token->type == TOKENTYPE_KEYWORD && (token->value.keyword == KEYWORD_LET || token->value.keyword == KEYWORD_VAR)) {
		(*statement)->type = ST_VAR;

		// Data type is null if not specified
		bool nil_allowed = false;

		(*statement)->var.modifiable = token->value.keyword == KEYWORD_VAR;

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_ID) {

			(*statement)->var.id = token->value.id;

			get_token(input, symtab, token);
			if (token->type == TOKENTYPE_COLON) {
				get_token(input, symtab, token);
				
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
						(*statement)->var.allow_nil = true;
						get_token(input, symtab, token);
					}

					// var a : Int \n => correct
					if ((*statement)->var.modifiable && token->type == TOKENTYPE_NEWLINE) {					
						return 0;
						
					} else if (!(*statement)->var.modifiable && token->type == TOKENTYPE_NEWLINE) {
						// let a : Int \n => error
						return 2;
					}
				} else {
					return 2;
				}
			} else {
				(*statement)->var.data_type = VARTYPE_VOID;
			}

			if (token->type == TOKENTYPE_EQUALS) {
				Expression *exp;

				ret = parse_expression(input, symtab, &exp, NULL, &out_token, &out_token_returned);

				//TODO: is_valid = semantic_variable(VarTableStack, FunctionTable, Statement) return value = jestli prosla

				(*statement)->var.exp = exp;

				if (ret != 0) {
					return ret;
				}

				if (out_token_returned) {
					token = &out_token;
				} else {
					get_token(input, symtab, token);
				}

				if (token->type == TOKENTYPE_NEWLINE) {
					return 0;
				}
			}
		}
		//TODO: nejake testy konci tady i kdyz by nemely
		printf("RET 2 %d\n", __LINE__);
		return 2;
	}

	// <statement> -> return <exp> \n
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_RETURN) {
		// TODO parse exp
		(*statement)->type = ST_RETURN;

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_NEWLINE) {
			return 0;
		}

		return 2;
	}

	// TODO: <statement> -> <func>

	// <statement> -> <id> = <exp> \n
	else if (token->type == TOKENTYPE_ID) {
		(*statement)->type = ST_ASSIGN;

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_EQUALS) {
			// TODO parse exp

			get_token(input, symtab, token);
			if (token->type == TOKENTYPE_NEWLINE) {
				return 0;
			}
		}
	}

	// <func> -> func <id> ( [<id> <id> : <type>] ) [-> <type>] { <statementList> }
	else if (token->type == KEYWORD_FUNC) {
		(*statement)->type = ST_FUNC;

		Function func;

		// zacatek scope
		VarTable *vartable;
		init_var_table(vartable);
		vartable_stack_push(var_table_stack, vartable);

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_ID) {
			func.id = token->value.id;  //? correct ?
			
			get_token(input, symtab, token);

			// parsing params
			Parameter par;
			if (token->type == TOKENTYPE_UNDERSCORE) {  // func id = (_ intern)

				get_token(input, symtab, token);
				if (token->type != TOKENTYPE_ID) {
					return 2;
				}
				//? correct ?
				par.intern_id = token->value.id;
				par.extern_id = par.intern_id;

				DataType dt;

				get_token(input, symtab, token);
				if (token->type == TOKENTYPE_COLON) {
					get_token(input, symtab, token);
					//TODO: if(type == keyword)
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
							return 2; //! check this
					}
					par.type = dt;
				}
			} else if (token->type == TOKENTYPE_ID) {  // func id = (extern intern)
				par.intern_id = token->value.id;
				get_token(input, symtab, token);
				if (token->type == TOKENTYPE_ID) {
					//? correct ?

					par.extern_id = token->value.id;

					DataType dt;

					get_token(input, symtab, token);
					if (token->type == TOKENTYPE_COLON) {
						get_token(input, symtab, token);
						switch (token->type) {
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
								return 2;  //! check this
						}
					}
				}
			}

			if (token->type == TOKENTYPE_COMMA) {  // more params
				// todo recursive call for parameters
			} else if (token->type == TOKENTYPE_PAR_R) {
				get_token(input, symtab, token);

				if (token->type == TOKENTYPE_BRACE_L) {
					ret = parse_statement_list( input,
							symtab, &(*statement)->func.body,
							var_table_stack, func_table,
							*statement, true
						);

					if (ret != -2) {
						return 2;
					}

					if (out_token_returned) {
						token = &out_token;
					} else {
						get_token(input, symtab, token);
					}

					if (token->type == TOKENTYPE_BRACE_R) {
						return 0;
					}
					return 2;
				}
			}
		}
	}

	return 2;
}

//TODO: oznacovat ze jsem v bloku i ze jsem ve funkci v parametrech
//TODO: staci to oznacit Statementem ne?
//TODO: tady by se mel posilat i in_token ne? kdyz se to vola ve scopu, tak je predtim muze byt nacteny token
// pokud jsem v bloku, nemuzu udelat return ani definici funkce
// pokud jsem ve funkci, nemuzu udelat definici funkce
// current_scope == NULL => jsme v globalni
// out_token => vraci prvni precteny token, ktery uz nepatri statement listu (napr. EOF, {, Int, Double, String,...)
int parse_statement_list(Input* input, SymbolTable* symtab, Statement** statement, VarTableStack* var_table_stack, FuncTable* func_table, Statement *current_scope, bool in_function) {
	int ret = 0;

	Statement* st;
	Statement** next_st = statement;

	while (ret == 0) {
		st = NULL;
		ret = parse_statement(input,
			symtab, &st, 
			var_table_stack, 
			func_table, in_function
		);

		if (ret)
			break;

		if (st->type == ST_FUNC && current_scope != NULL) {
			return 2;
		} else if (st->type == ST_RETURN && !in_function) {
			return 2;
		}

		*next_st = st;
		next_st = &st->next;
	}

	if (ret == -1 && current_scope != NULL) { // ret == -1 znamena EOF
		return 2;

	} else if (ret == -2 && current_scope == NULL) { // ret == -2 znamena '}'
		printf("RET 2 %d\n", __LINE__);
		return 2;
	}

	return ret;
}

// vraci bool jestli je ten exp validni
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
			if (token->type != KEYWORD_NIL) {
				return 2;
			}
			(*exp)->type = ET_NIL;
			break;

		case TOKENTYPE_ID:
			(*exp)->type = ET_ID;
			(*exp)->id = token->value.id;
		default:
			return 2;
	}

	//TODO: out_token
	*out_token_returned = false;

	return 0;
}
