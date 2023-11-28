#include "parser.h"

#include "AST.h"

#include "token_stack.h"
#include "stdlib.h"
#include "vartable_stack.h"

int parse(Input* input, AST* ast) {
	SymbolTable symtab;
	init_symboltable(&symtab);

	int ret = 0;

	Statement* st;
	Statement** next_st = &ast->statement;

	VarTableStack var_table_stack;

	FuncTable func_table;

	while (ret == 0) {
		ret = parse_statement(input, &symtab, &st, &var_table_stack, &func_table);
		if (ret)
			break;

		ast->statement = st;
		*next_st = st;
		next_st = &st->next;
	}


	if (ret == -1)
		ret = 0;
	else
		ret = 99;

	return ret;
}

int parse_statement(Input *input, SymbolTable* symtab, Statement** statement, VarTableStack* var_table_stack, FuncTable* func_table) {
	Token _token;
	Token* token = &_token;

	*statement = (Statement*)malloc(sizeof(Statement));

	if (*statement == NULL) {
		exit(99);
	}

	get_token(input, symtab, token);
	if (token->type == TOKENTYPE_EOF) return -1;

	// <statement> -> if let <id> { <statementList> } [else { <statementList> }]
	// <statement> -> if ( <exp> ) { <statementList> } [else { <statementList> }]
	// TODO check newlines
	// TODO: je potreba kontrolovat jestli je to TOKENTYPE_KEYWORD pred zkontrolovanim token->value.keyword?
	if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_IF) {
		bool is_let = false;
		Expression exp;

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_LET) {
			get_token(input, symtab, token);
			if (token->type != TOKENTYPE_ID) {  // TODO: staci to takto?
				return 2;                       // TODO: ma se vracet nejake cislo != 0 oznacujici chybu? jake?
			}
			is_let = true;

		} else if (token->type == TOKENTYPE_PAR_L) {
			// TODO neco udelat s <exp>
			// exp = ...
			get_token(input, symtab, token);
			if (token->type != TOKENTYPE_PAR_R) {  // TODO: staci to takto?
				return 2;
			}
		}

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_BRACE_L) {
			// TODO neco udelat s <stmntList>
			Statement if_statement_list[] = {};  // TODO udělal strukturu pro statementlist
												 // parseStatement( pro vnitřek if );

			get_token(input, symtab, token);
			if (token->type == TOKENTYPE_BRACE_R) {
				// bez rozsireni je vzdy else

				get_token(input, symtab, token);
				if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_ELSE) {
					// TODO: else block can be empty

					get_token(input, symtab, token);
					if (token->type == TOKENTYPE_BRACE_L) {
						// TODO neco udelat s <stmntList>
						Statement else_statement_list[] = {};  // TODO udělal strukturu pro statementlist
															   // parseStatement( pro vnitřek if );

						get_token(input, symtab, token);
						if (token->type == TOKENTYPE_BRACE_R) {
							Statement else_statement;
							else_statement.if_.exp = NULL;
							else_statement.if_.check_nil = false;
							else_statement.if_.body = else_statement_list;
							else_statement.if_.else_ = NULL;

							if (is_let) {
								(*statement)->if_.exp = &exp;
								(*statement)->if_.check_nil = false;
							} else {
								(*statement)->if_.exp = NULL;
								(*statement)->if_.check_nil = true;
							}

							(*statement)->if_.body = if_statement_list;
							(*statement)->if_.else_ = &else_statement;

							return 0;
						}
					}
				}
			}
		}

		return 2;
	}

	// <statement> -> while ( <exp> ) { <statementList> }
	// TODO check/handle newlines
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_WHILE) {
		bool is_let = false;
		Expression *exp;

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_PAR_L) {
			// TODO neco udelat s <exp>
			// exp = ...

			get_token(input, symtab, token);
			if (token->type == TOKENTYPE_PAR_R) {
				get_token(input, symtab, token);
				if (token->type == TOKENTYPE_BRACE_L) {
					// TODO neco udelat s <stmntList>
					Statement while_statement_list[] = {};  // TODO udělal strukturu pro statementlist
															// parseStatement( pro vnitřek while );

					get_token(input, symtab, token);
					if (token->type == TOKENTYPE_BRACE_R) {
						(*statement)->while_.exp = NULL;
						(*statement)->while_.body = while_statement_list;

						return 0;
					}
				}
			}
		}

		return 2;
	}

	// <statement> -> let <id> [: <type>] = <exp> \n
	// <statement> -> var <id> [: <type>] = <exp> \n
	// <statement> -> var <id> : <type> [= <exp>] \n
	else if (token->type == TOKENTYPE_KEYWORD && (token->value.keyword == KEYWORD_LET || token->value.keyword == KEYWORD_VAR)) {
		
		// Data type is null if not specified
		bool nil_allowed = false;
		int ret;

		(*statement)->var.modifiable = token->value.keyword == KEYWORD_VAR;

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_ID) {

			get_token(input, symtab, token);
			if (token->type == TOKENTYPE_COLON) {
				get_token(input, symtab, token);
				
				if (token->type == TOKENTYPE_KEYWORD) {

					switch (token->value.keyword) {
						case KEYWORD_DOUBLE:
							(*statement)->var.data_type = KEYWORD_DOUBLE;
						case KEYWORD_INT:
							(*statement)->var.data_type = KEYWORD_INT;
						case KEYWORD_STRING:
							(*statement)->var.data_type = KEYWORD_STRING;
						default:
							return 2;
					}

					get_token(input, symtab, token);
					if (token->type == TOKENTYPE_QUESTIONMARK) {
						(*statement)->var.allow_nil = true;
						get_token(input, symtab, token);
					}

					if (token->type == TOKENTYPE_NEWLINE) {
						return 0;
					} else {
						return 2;
					}
				} else {
					return 2;
				}
			} else {
				(*statement)->var.data_type = KEYWORD_NIL;
			}

			if (token->type == TOKENTYPE_EQUALS) {
				Expression *exp;
				
				// vraci bool jestli je ten exp validni
				// vraci expression pointer
				// vraci ukazatel na to kde skoncila
				ret = parse_expression(input, symtab, &exp);

				if (ret != 0) {
					return ret;
				}

				get_token(input, symtab, token);
				if (token->type == TOKENTYPE_NEWLINE) {
					return 0;
				}
			}
		}
		return 2;
	}

	// <statement> -> return <exp> \n
	else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_RETURN) {
		// TODO parse exp

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_NEWLINE) {
			return 0;
		}

		return 2;
	}

	// TODO: <statement> -> <func>

	// <statement> -> <id> = <exp> \n
	else if (token->type == TOKENTYPE_ID) {
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
		Function func;

		get_token(input, symtab, token);
		if (token->type == TOKENTYPE_ID) {
			func.id = token->value.id;  //? correct ?
			get_token(input, symtab, token);
			if (token->type == TOKENTYPE_PAR_L) {
				get_token(input, symtab, token);

				// parsing params
				Parameter par;
				if (token->type == TOKENTYPE_UNDERSCORE) {  // func id = (_ intern)

					get_token(input, symtab, token);
					if (token->type == TOKENTYPE_ID) {
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
								case KEYWORD_INT:
									dt.type = VARTYPE_INT;
								case KEYWORD_STRING:
									dt.type = VARTYPE_STRING;
								default:
									return 2; //! check this
							}
							par.type = dt;
						}
						if (token->type == TOKENTYPE_COMMA) {  // more params
							// todo recursive call for parameters
						} else if (token->type == TOKENTYPE_PAR_R) {
							get_token(input, symtab, token);
							if (token->type == TOKENTYPE_BRACE_L) {
								get_token(input, symtab, token);

								// todo parse body
							}
						}
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
								case KEYWORD_INT:
									dt.type = VARTYPE_INT;
								case KEYWORD_STRING:
									dt.type = VARTYPE_STRING;
								default:
									return 2;  //! check this
							}
						}

						if (token->type == TOKENTYPE_COMMA) {  // more params
							// todo recursive call for parameters
						} else if (token->type == TOKENTYPE_PAR_R) {
							get_token(input, symtab, token);
							if (token->type == TOKENTYPE_BRACE_L) {
								get_token(input, symtab, token);

								// todo parse body
							}
						}
					}
				}
			}
		}
	}

	return 2;
}

int parse_expression(Input* input, SymbolTable* symtab, Expression** exp) {
	Token _token;
	Token* token = &_token;

	get_token(input, symtab, token);

	if (token->type == TOKENTYPE_KEYWORD) {

		*exp = (Expression*)malloc(sizeof(Expression));
		if (*exp == NULL) {
			exit(99);
		}

		switch (token->value.keyword) {
			case KEYWORD_DOUBLE:
				(*exp)->type = ET_DOUBLE;
				(*exp)->double_ = token->value.double_;
				break;
			case KEYWORD_INT:
				(*exp)->type = ET_INT;
				(*exp)->int_ = token->value.int_;
				break;
			case KEYWORD_STRING:
				(*exp)->type = ET_STRING;
				(*exp)->str_ = token->value.str_;
				break;
			default:
				return 2;
		}
		return 0;
	}

	return 2;
}
