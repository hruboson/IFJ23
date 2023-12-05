#include "AST.h"

#include <stdlib.h>
#include "string_.h"

void init_ast(AST* ast) {
    ast->statement = NULL;
    init_symboltable(&ast->symtab);
}

void clear_ast(AST* ast) {
    if (ast->statement) {
        /*Statement* curr = ast->statement;
        while (curr) {
            Statement* tmp = curr->next;
            free(curr);
            curr = tmp;
        }*/
        ast->statement = NULL;
    }
    clear_symboltable(&ast->symtab);
}

void
print_exp( const Expression* e ) {
	if ( e == NULL )
		return;

	switch ( e->type ) {
	case ET_ID:
		printf( "%s", e->id->symbol.data );
		break;
	case ET_INT:
		printf( "%i", e->int_ );
		break;
	case ET_DOUBLE:
		printf( "%f", e->double_ );
		break;
	case ET_STRING:
		printf( "\"" );
		for ( size_t i = 0; i < e->str_.length; i++ ) {
			char c = e->str_.data[ i ];

			if ( c <= 31 )
				printf( "\\u{%x}", c );
			else
				printf( "%c", c );
		}
		printf( "\"" );
		break;
	case ET_ADD:
		print_exp( e->ops[ 0 ] );
		printf( " + " );
		print_exp( e->ops[ 1 ] );
		break;
	case ET_SUB:
		print_exp( e->ops[ 0 ] );
		printf( " - " );
		print_exp( e->ops[ 1 ] );
		break;
	case ET_MULT:
		print_exp( e->ops[ 0 ] );
		printf( " * " );
		print_exp( e->ops[ 1 ] );
		break;
	case ET_DIV:
		print_exp( e->ops[ 0 ] );
		printf( " / " );
		print_exp( e->ops[ 1 ] );
		break;
	case ET_FUNC:
                printf("%s(", e->fn_call.id->symbol.data );
		for ( size_t i = 0; i < e->fn_call.arg_count; i++ ) {
			printf( " " );
			const Argument* a = e->fn_call.args + i;

			if ( a->id != NULL )
				printf( "%s: ", a->id->symbol.data );

			print_exp( a->exp );

			if ( i != e->fn_call.arg_count - 1 )
				printf( "," );
			else
				printf( " " );
		}
		printf( ")" );
		break;
	}


}

void ast_append(AST* ast, Statement* st) {
    if (!(ast->statement)) {  // adding first statement
        ast->statement = st;
        return;
    }

    Statement* curr = ast->statement;
    while (curr->next) {
        curr = curr->next;
    }

    curr->next = st;
}

void ast_get(AST* ast, Statement* st) {
    // todo
}

void print_ast(AST* ast) {
    String s;
    init_string(&s);

    Statement* stm = ast->statement;

    printf("=============\n");
    while (stm) {
        switch (stm->type) {
            case ST_ASSIGN:
                printf("assign {\n");
                printf("\tid: %s\n", stm->assign.id->symbol.data);
                print_exp(stm->assign.exp);
                printf("}\n");
                break;
            case ST_EXP:
                printf("expression {\n\t");
                print_exp(stm->exp.exp);
                printf("\n}\n");
                break;
            case ST_FUNC:
                printf("function {\n");
                printf("\tid: %s\n", stm->func.id->symbol.data);
                printf("\tused: %s\n", stm->func.used ? "true" : "false");
                printf("\tparam_count: %zu\n", stm->func.param_count);
                printf("\tparameters {\n");
                for (size_t i = 0; i < stm->func.param_count; i++) {
                    printf("\t\textern: %s, intern: %s, data_type: %s, ", stm->func.parameters[i].extern_id->symbol.data, stm->func.parameters[i].intern_id->symbol.data);
                    switch (stm->func.parameters[i].type.type) {
                        case VARTYPE_DOUBLE:
                            printf("double");
                            break;
                        case VARTYPE_INT:
                            printf("int");
                            break;
                        case VARTYPE_STRING:
                            printf("string");
                            break;
                        case VARTYPE_VOID:
                            printf("void");
                            break;
                        case VARTYPE_NIL:
                            printf("nil");
                            break;

                        default:
                            break;
                    }
                    printf("nil_allowed: %s\n", stm->func.parameters[i].type.nil_allowed ? "true" : "false");
                }
                printf("\t} \n");
                // todo print_exp(stm->exp.exp);
                printf("\treturn_type: ");
                switch (stm->func.return_type.type) {
                    case VARTYPE_DOUBLE:
                        printf("double");
                        break;
                    case VARTYPE_INT:
                        printf("int");
                        break;
                    case VARTYPE_STRING:
                        printf("string");
                        break;
                    case VARTYPE_VOID:
                        printf("void");
                        break;
                    case VARTYPE_NIL:
                        printf("nil");
                        break;

                    default:
                        break;
                }
                printf(", nil_allowed: %s\n", stm->func.return_type.nil_allowed ? "true" : "false");
                Statement* stm_func = stm->func.body;
                printf("\tbody {\n");
                while (stm_func) {
                    print_statement(stm_func, 2);
                    stm_func = stm_func->next;
                }
                printf("\t}\n");
                printf("}\n");
                break;
            case ST_IF:
                printf("if {\n");
                // todo print_exp(stm->if_.exp)
                printf("\tcheck_nil: %s\n", stm->if_.check_nil ? "true" : "false");
                printf("\tbody {\n");
                Statement* stm_if = stm->if_.body;
                while (stm_if) {
                    print_statement(stm_if, 2);
                    stm_if = stm_if->next;
                }
                printf("\t}\n");
                stm_if = stm->if_.else_;
                printf("\telse {\n");
                while (stm_if) {
                    print_statement(stm_if, 2);
                    stm_if = stm_if->next;
                }
                printf("\t}\n");
                printf("}\n");
                break;
            case ST_RETURN:
                printf("return {\n");
                // todo print_exp(stm->return_.exp);
                printf("\n}\n");
                break;
            case ST_VAR:
                printf("variable {\n");
                printf("\tid: %s\n", stm->var.id->symbol.data);
                if (stm->var.id_prefix.func_id) {
                    printf("\tprefix_id. $%s%%%zu%%%s\n", stm->var.id_prefix.func_id->symbol.data, stm->var.id_prefix.block_counter, stm->var.id->symbol.data);
                } else {
                    printf("\tprefix_id. $_%%%zu%%%s\n", stm->var.id_prefix.block_counter, stm->var.id->symbol.data);
                }
                // todo print_exp(stm->var.exp);
                printf("\tmodifiable: %s\n", stm->var.modifiable ? "true" : "false");
                printf("\tdata_type: ");
                switch (stm->var.data_type) {
                    case VARTYPE_DOUBLE:
                        printf("double");
                        break;
                    case VARTYPE_INT:
                        printf("int");
                        break;
                    case VARTYPE_STRING:
                        printf("string");
                        break;
                    case VARTYPE_VOID:
                        printf("void");
                        break;
                    //? can return type be nil
                    case VARTYPE_NIL:
                        printf("nil");
                        break;

                    default:
                        break;
                }
                printf(", nil_allowed: %s\n", stm->var.allow_nil ? "true" : "false");
                printf("\tused: %s\n", stm->var.used ? "true" : "false");
                printf("}\n");
                break;
            case ST_WHILE:
                printf("while {\n");
                // todo print_exp(stm->while_.exp);
                Statement* stm_while = stm->while_.body;
                while (stm_while) {
                    print_statement(stm_while, 1);
                    stm_while = stm_while->next;
                }
                printf("}\n");
                break;
            default:
                break;
        }
        stm = stm->next;
    }
    printf("=============\n");
}

void print_statement(Statement* stm, size_t tabs) {
    String tab;
    init_string(&tab);
    for (size_t i = 0; i < tabs; i++) {
        string_append(&tab, "\t");
    }

    switch (stm->type) {
        case ST_ASSIGN:
            printf("%sassign {\n", tab.data);
            // todo print_exp(stm->assign.exp, tabs);
            printf("%s\tid: %s\n", tab.data, stm->assign.id->symbol.data);
            printf("%s}\n", tab.data);
            break;
        case ST_EXP:
            printf("%sexpression {\n", tab.data);
            // todo print_exp(stm->exp.exp);
            printf("%s}\n", tab.data);
            break;
        case ST_FUNC:
            printf("%sfunction {\n", tab.data);
            printf("%s\tid: %s\n", tab.data, stm->func.id->symbol.data);
            printf("%s\tused: %s\n", tab.data, stm->func.used ? "true" : "false");
            printf("%s\tparam_count: %zu\n", tab.data, stm->func.param_count);
            printf("%s\tparameters {\n", tab.data);
            for (size_t i = 0; i < stm->func.param_count; i++) {
                printf("%s\t\textern: %s, intern: %s, data_type: %s", tab.data, stm->func.parameters[i].extern_id->symbol.data, stm->func.parameters[i].intern_id->symbol.data);
                switch (stm->func.parameters[i].type.type) {
                    case VARTYPE_DOUBLE:
                        printf("double");
                        break;
                    case VARTYPE_INT:
                        printf("int");
                        break;
                    case VARTYPE_STRING:
                        printf("string");
                        break;
                    case VARTYPE_VOID:
                        printf("void");
                        break;
                    //? can parameter type be nil
                    case VARTYPE_NIL:
                        printf("nil");
                        break;

                    default:
                        break;
                }
                printf(", nil_allowed: %s\n", stm->func.parameters[i].type.nil_allowed ? "true" : "false");
            }
            printf("%s\t} \n");
            // todo print_exp(stm->exp.exp);
            printf("%s\treturn_type: ", tab.data);
            switch (stm->func.return_type.type) {
                case VARTYPE_DOUBLE:
                    printf("double");
                    break;
                case VARTYPE_INT:
                    printf("int");
                    break;
                case VARTYPE_STRING:
                    printf("string");
                    break;
                case VARTYPE_VOID:
                    printf("void");
                    break;
                //? can return type be nil
                case VARTYPE_NIL:
                    printf("nil");
                    break;

                default:
                    break;
            }
            printf(", nil_allowed: %s\n", stm->func.return_type.nil_allowed ? "true" : "false");
            printf("%s\t body {\n", tab.data);
            Statement* stm_func = stm->func.body;
            while (stm_func) {
                print_statement(stm_func, ++tabs);
                stm_func = stm_func->next;
            }
            printf("%s\t }\n", tab.data);
            printf("%s}\n", tab.data);
            break;
        case ST_IF:
            printf("%sif {\n", tab.data);
            // todo print_exp(stm->if_.exp)
            printf("%s\tcheck_nil: %s\n", tab.data, stm->if_.check_nil ? "true" : "false");
            printf("%s\tbody {\n", tab.data);
            Statement* stm_if = stm->if_.body;
            while (stm_if) {
                print_statement(stm_if, ++tabs);
                stm_if = stm_if->next;
            }
            printf("%s\t}\n", tab.data);
            stm_if = stm->if_.else_;
            printf("%s\telse {\n", tab.data);
            while (stm_if) {
                print_statement(stm_if, ++tabs);
                stm_if = stm_if->next;
            }
            printf("%s\t}\n", tab.data);
            printf("%s}\n", tab.data);
            break;
        case ST_RETURN:
            printf("%sreturn {\n", tab.data);
            // todo print_exp(stm->return_.exp);
            printf("%s}\n", tab.data);
            break;
        case ST_VAR:
            printf("%svariable {\n", tab.data);
            printf("%s\tid: %s\n", tab.data, stm->var.id->symbol.data);
            if (stm->var.id_prefix.func_id) {
                printf("%s\tprefix_id. $%s%%%zu%%%s\n", tab.data, stm->var.id_prefix.func_id->symbol.data, stm->var.id_prefix.block_counter, stm->var.id->symbol.data);
            } else {
                printf("%s\tprefix_id. $_%%%zu%%%s\n", tab.data, stm->var.id_prefix.block_counter, stm->var.id->symbol.data);
            }

            // todo print_exp(stm->var.exp);
            printf("%s\tmodifiable: %s\n", tab.data, stm->var.modifiable ? "true" : "false");
            printf("%s\tdata_type: ", tab.data);
            switch (stm->var.data_type) {
                case VARTYPE_DOUBLE:
                    printf("double");
                    break;
                case VARTYPE_INT:
                    printf("int");
                    break;
                case VARTYPE_STRING:
                    printf("string");
                    break;
                case VARTYPE_VOID:
                    printf("void");
                    break;
                //? can return type be nil
                case VARTYPE_NIL:
                    printf("nil");
                    break;
                default:
                    break;
            }
            printf(", nil_allowed: %s\n", stm->var.allow_nil ? "true" : "false");
            printf("%s\tused: %s\n", tab.data, stm->var.used ? "true" : "false");
            printf("%s}\n", tab.data);
            break;
        case ST_WHILE:
            printf("%swhile {", tab.data);
            // todo print_exp(stm->while_.exp);
            Statement* stm_while = stm->while_.body;
            while (stm_while) {
                print_statement(stm_while, ++tabs);
                stm_while = stm_while->next;
            }
            printf("%s}", tab.data);
            break;
        default:
            break;
    }

    clear_string(&tab);
}
