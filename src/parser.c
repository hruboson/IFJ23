#include "parser.h"

#include "AST.h"
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

    while (ret == 0) {
        ret = parse_statement(f, &symtab, &st);
        if (ret)
            break;

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

int parse_statement(FILE* f, SymbolTable* symtab, Statement** statement) {
    Token* token;

    get_token(f, symtab, token);
    if (token->type == TOKENTYPE_EOF) return -1;

    // <statement> -> if ( <exp> ) { <statementList> } [else { <statementList> }]
    // todo check newlines
    if (token->type == TOKENTYPE_KEYWORD) {
        if (token->value.keyword == KEYWORD_IF) {  // nevim jak se to pise
            get_token(f, symtab, token);
            if (token->type == TOKENTYPE_PAR_L) {
                // TODO neco udelat s <exp>
                Expression exp;
                get_token(f, symtab, token);
                if (token->type == TOKENTYPE_PAR_R) {
                    get_token(f, symtab, token);
                    if (token->type == TOKENTYPE_BRACE_L) {
                        // TODO neco udelat s <stmntList>
                        Statement ifStatementList[] = {};  // todo udělal strukturu pro statementlist
                                                           // parseStatement( pro vnitřek if );
                        get_token(f, symtab, token);
                        if (token->type == TOKENTYPE_BRACE_L) {
                            get_token(f, symtab, token);
                            if (token->type == TOKENTYPE_BRACE_R) {
                                // bez rozsireni je vzdy else
                                if (token->type == TOKENTYPE_KEYWORD) {
                                    get_token(f, symtab, token);
                                    if (token->value.keyword == KEYWORD_ELSE) {  // nevim jak se to pise
                                                                                 // else block can be empty
                                        get_token(f, symtab, token);
                                        if (token->type == TOKENTYPE_PAR_L) {
                                            // TODO neco udelat s <stmntList>
                                            Statement elseStatementList[] = {};  // todo udělal strukturu pro statementlist
                                            get_token(f, symtab, token);
                                            if (token->type == TOKENTYPE_PAR_R) {
                                                get_token(f, symtab, token);
                                                if (token->type == TOKENTYPE_BRACE_L) {
                                                    get_token(f, symtab, token);
                                                    if (token->type == TOKENTYPE_BRACE_R) {
                                                        Statement elseStatement;
                                                        elseStatement.if_.exp = NULL;
                                                        elseStatement.if_.check_nil = false;
                                                        elseStatement.if_.body = elseStatementList;
                                                        elseStatement.if_.else_ = NULL;

                                                        (*statement)->if_.exp = &exp;
                                                        (*statement)->if_.check_nil = false;
                                                        (*statement)->if_.body = ifStatementList;
                                                        (*statement)->if_.else_ = &elseStatement;

                                                        return 0;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // <statement>-><id> = <exp> \n
    if (token->type == TOKENTYPE_ID) {
        get_token(f, symtab, token);
        if (token->type == TOKENTYPE_EQUALS) {
            // parse exp
			get_token(f, symtab, token);
			if(token->type == TOKENTYPE_NEWLINE){
				return 0;
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