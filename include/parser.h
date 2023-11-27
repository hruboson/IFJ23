#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "AST.h"
#include "expression.h"
#include "scanner.h"
#include "vartable_stack.h"
#include "statement.h"
#include "token_stack.h"
#include "table.h"

int parse(Input*, AST*);
int parse_statement(Input*, SymbolTable*, Statement**, VarTableStack*, FuncTable*);
int parse_expression(Input*, SymbolTable*, Expression**);

// <statement> -> if ( <exp> ) { <statementList> } [else { <statementList> }]
bool if_();
// <statement> -> if let <id> { <statementList> } [else { <statementList> }]
bool if_let();
// <statement> -> while ( <exp> ) { <statementList> }
bool while_();
// <statement> -> <id> = <exp> \n
bool assign();
// <statement> -> let <id> [: <type>] = <exp> \n
bool let();
// <statement> -> var <id> [: <type>] = <exp> \n
bool var();
// <statement> -> let <id> : <type> [= <exp>] \n
bool let_type();
// <statement> -> var <id> : <type> [= <exp>] \n
bool var_type();
// <statement> -> return <exp> \n
bool return_exp();
// <statement> -> <func>
bool func();
// <statementList> -> <statement>
bool statement();
// <statementList> -> <statement> <statementList>
bool statement_list();
// <func> -> func <id> ( [<id> <id> : <type>] ) [-> <type>] { <statementList> }
bool func_definition();
// <exp> -> <id>( [ [<id>:] <exp> ] ) \n
bool func_call();
// <type> -> {Double, Int, String}[?]
bool type();
// <exp> -> <id> [!]
bool id();
// <exp> -> <id> ?? <exp>
bool is_nil();
// <exp> -> {Double, Int, String} //takto ty tokeny int, double, string ?
bool value();
// <exp> -> <exp> <operator> <exp>
bool exp_op_exp();
// <operator> -> +, -, *, /, %, >, >=, <, <=, ==, !=
bool op();