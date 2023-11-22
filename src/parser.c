#include "parser.h"

#include "AST.h"
#include "table.h"
#include "token_stack.h"

int parse(FILE* f, AST* ast) {
    TokenStack stack;
    init_token_stack(&stack);
    token_stack_push(&stack, TOKENTYPE_EOF);

    SymbolTable symtab;
    init_symboltable(&symtab);

    int ret = 0;

    Statement* st;
    Statement** next_st = &ast->statement;

    VarTable var_table;
    FuncTable func_table;
    AST ast;

    while (ret == 0) {
        ret = parse_statement(f, &symtab, &st, &var_table, &func_table);
        if (ret)
            break;

        ast->statement = st;
        *next_st = st;
        next_st = &st->next;
    }

    clear_token_stack(&stack);

    if (ret == -1)
        ret = 0;
    else
        ret = 99;

    return ret;
}

int parse_statement(FILE* f, SymbolTable* symtab, Statement** statement, VarTable* var_table, FuncTable* func_table) {
    Token* token;

    get_token(f, symtab, token);
    if (token->type == TOKENTYPE_EOF) return -1;

    // <statement> -> if let <id> { <statementList> } [else { <statementList> }]
    // <statement> -> if ( <exp> ) { <statementList> } [else { <statementList> }]
    // TODO check newlines
    // TODO: je potreba kontrolovat jestli je to TOKENTYPE_KEYWORD pred zkontrolovanim token->value.keyword?
    if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_IF) {
        bool is_let = false;
        Expression exp;

        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_LET) {
            get_token(f, symtab, token);
            if (token->type != TOKENTYPE_ID) {  // TODO: staci to takto?
                return 2;                       // TODO: ma se vracet nejake cislo != 0 oznacujici chybu? jake?
            }
            is_let = true;

        } else if (token->type == TOKENTYPE_PAR_L) {
            // TODO neco udelat s <exp>
            // exp = ...
            get_token(f, symtab, token);
            if (token->type != TOKENTYPE_PAR_R) {  // TODO: staci to takto?
                return 2;
            }
        }

        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_BRACE_L) {
            // TODO neco udelat s <stmntList>
            Statement if_statement_list[] = {};  // TODO udělal strukturu pro statementlist
                                                 // parseStatement( pro vnitřek if );

            get_token(f, symtab, token);
            if (token->type == TOKENTYPE_BRACE_R) {
                // bez rozsireni je vzdy else

                get_token(f, symtab, token);
                if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_ELSE) {
                    // TODO: else block can be empty

                    get_token(f, symtab, token);
                    if (token->type == TOKENTYPE_BRACE_L) {
                        // TODO neco udelat s <stmntList>
                        Statement else_statement_list[] = {};  // TODO udělal strukturu pro statementlist
                                                               // parseStatement( pro vnitřek if );

                        get_token(f, symtab, token);
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
        Expression exp;

        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_PAR_L) {
            // TODO neco udelat s <exp>
            // exp = ...

            get_token(f, symtab, token);
            if (token->type == TOKENTYPE_PAR_R) {
                get_token(f, symtab, token);
                if (token->type == TOKENTYPE_BRACE_L) {
                    // TODO neco udelat s <stmntList>
                    Statement while_statement_list[] = {};  // TODO udělal strukturu pro statementlist
                                                            // parseStatement( pro vnitřek while );

                    get_token(f, symtab, token);
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
    // <statement> -> let <id> : <type> [= <exp>] \n
    // <statement> -> var <id> : <type> [= <exp>] \n
    else if (token->type == TOKENTYPE_KEYWORD && (token->value.keyword == KEYWORD_LET || token->value.keyword == KEYWORD_VAR)) {
        // TODO: ma se misto tohoto primo pridavat do VarTable?
        //? should this be DataType ?
        Variable var;
        TokenType var_type;  // Null if not specified
        bool nil_allowed = false;

        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_ID) {
            get_token(f, symtab, token);
            if (token->type == TOKENTYPE_COLON) {
                get_token(f, symtab, token);
                // TODO: takto kontrolovat type?
                DataType dt;
                switch (token->type) {
                    case KEYWORD_DOUBLE:
                        dt.type = VARTYPE_DOUBLE;
                    case KEYWORD_INT:
                        dt.type = VARTYPE_INT;
                    case KEYWORD_STRING:
                        dt.type = VARTYPE_STRING;
                    default:
                        return 2; //! check this 
                }

                get_token(f, symtab, token);
                if (token->type == TOKENTYPE_QUESTIONMARK) {
                    dt.nil_allowed = true;
                    get_token(f, symtab, token);
                }
                var.type = dt;

                if (token->type == TOKENTYPE_NEWLINE) {
                    return 0;
                }

                return 2;
            }

            if (token->type == TOKENTYPE_EQUALS) {
                if (var_type != NULL) {  // both type and "=" specified => not allowed
                    return 2;
                }

                // TODO parse exp

                get_token(f, symtab, token);
                if (token->type == TOKENTYPE_NEWLINE) {
                    return 0;
                }

                return 2;
            }
        }
    }

    // <statement> -> return <exp> \n
    else if (token->type == TOKENTYPE_KEYWORD && token->value.keyword == KEYWORD_RETURN) {
        // TODO parse exp

        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_NEWLINE) {
            return 0;
        }

        return 2;
    }

    // TODO: <statement> -> <func>

    // <statement> -> <id> = <exp> \n
    else if (token->type == TOKENTYPE_ID) {
        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_EQUALS) {
            // TODO parse exp

            get_token(f, symtab, token);
            if (token->type == TOKENTYPE_NEWLINE) {
                return 0;
            }
        }
    }

    // <func> -> func <id> ( [<id> <id> : <type>] ) [-> <type>] { <statementList> }
    else if (token->type == KEYWORD_FUNC) {
        Function func;

        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_ID) {
            func.id = token->value.id;  //? correct ?
            get_token(f, symtab, token);
            if (token->type == TOKENTYPE_PAR_L) {
                get_token(f, symtab, token);

                // parsing params
                Parameter par;
                if (token->type == TOKENTYPE_UNDERSCORE) {  // func id = (_ intern)

                    get_token(f, symtab, token);
                    if (token->type == TOKENTYPE_ID) {
                        //? correct ?
                        par.intern_id = token->value.id;
                        par.extern_id = par.intern_id;

                        DataType dt;

                        get_token(f, symtab, token);
                        if (token->type == TOKENTYPE_COLON) {
                            get_token(f, symtab, token);
                            switch (token->type) {
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
                            get_token(f, symtab, token);
                            if (token->type == TOKENTYPE_BRACE_L) {
                                get_token(f, symtab, token);

                                // todo parse body
                            }
                        }
                    }
                } else if (token->type == TOKENTYPE_ID) {  // func id = (extern intern)
                    par.intern_id = token->value.id;
                    get_token(f, symtab, token);
                    if (token->type == TOKENTYPE_ID) {
                        //? correct ?

                        par.extern_id = token->value.id;

                        DataType dt;

                        get_token(f, symtab, token);
                        if (token->type == TOKENTYPE_COLON) {
                            get_token(f, symtab, token);
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
                            get_token(f, symtab, token);
                            if (token->type == TOKENTYPE_BRACE_L) {
                                get_token(f, symtab, token);

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

int parse_expression(FILE*, TokenStack*, Expression**);

// Radek ->
// <statement> -> if ( <exp> ) { <statementList> } [else { <statementList> }]
bool if_() {
}
// <statement> -> if let <id> { <statementList> } [else { <statementList> }]
bool if_let() {
}
// <statement> -> while ( <exp> ) { <statementList> }
bool while_() {
}
// <statement> -> <id> = <exp> \n
bool assign() {
}
// <statement> -> let <id> [: <type>] = <exp> \n
bool let() {
}
// <statement> -> var <id> [: <type>] = <exp> \n
bool var() {
}
// <statement> -> let <id> : <type> [= <exp>] \n
bool let_type() {
}
// <statement> -> var <id> : <type> [= <exp>] \n
bool var_type() {
}

// <statement> -> return <exp> \n
bool return_exp() {
}
// <statement> -> <func>
bool func() {
}

// Ondra ->
// <statementList> -> <statement>
bool statement() {
}
// <statementList> -> <statement> <statementList>
bool statement_list() {
}
// <func> -> func <id> ( [<id> <id> : <type>] ) [-> <type>] { <statementList> }
bool func_definition() {
}
// <exp> -> <id>( [ [<id>:] <exp> ] ) \n
bool func_call() {
}
// <type> -> {Double, Int, String}[?]
bool type() {
}

// <exp> -> <id> [!]
bool id() {
}
// <exp> -> <id> ?? <exp>
bool is_nil() {
}
// <exp> -> {Double, Int, String} //takto ty tokeny int, double, string ?
bool value() {
}
// <exp> -> <exp> <operator> <exp>
bool exp_op_exp() {
}

// <operator> -> +, -, *, /, %, >, >=, <, <=, ==, !=
bool op() {
}