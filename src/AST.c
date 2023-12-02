#include "AST.h"

#include "stdlib.h"
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
                // todo print_exp(stm->assign.exp);
                printf("id: %s\n", stm->assign.id->symbol.data);
                printf("}\n");
                break;
            case ST_EXP:
                printf("expression {\n");
                // todo print_exp(stm->exp.exp);
                printf("}\n");
                break;
            case ST_FUNC:
                printf("function {\n");
                printf("id: %s\n", stm->func.id->symbol.data);
                printf("used: %s\n", stm->func.used ? "true" : "false");
                printf("param_count: %zu\n", stm->func.param_count);
                // todo print_exp(stm->exp.exp);
                printf("return_type: ");
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
                Statement* stm_func = stm->func.body;
                while (stm_func) {
                    print_statement(stm_func, 1);
                    stm_func = stm_func->next;
                }
                printf("}\n");
                break;
            case ST_IF:
                printf("if {\n");
                // todo print_exp(stm->if_.exp)
                printf("check_nil: %s\n", stm->if_.check_nil ? "true" : "false");
                printf("body {\n");
                Statement* stm_if = stm->if_.body;
                while (stm_if) {
                    print_statement(stm_if, 1);
                    stm_if = stm_if->next;
                }
                printf("}\n");
                stm_if = stm->if_.else_;
                printf("else {\n");
                while (stm_if) {
                    print_statement(stm_if, 1);
                    stm_if = stm_if->next;
                }
                printf("\n}");
                break;
            case ST_RETURN:
                printf("return {\n");
                // todo print_exp(stm->return_.exp);
                printf("}\n");
                break;
            case ST_VAR:
                printf("variable {\n");
                printf("id: %s\n", stm->var.id->symbol.data);
                // todo print_exp(stm->var.exp);
                printf("modifiable: %s\n", stm->var.modifiable ? "true" : "false");
                printf("data_type: ");
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
                printf("used: %s\n", stm->var.used ? "true" : "false");
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
            // todo print_exp(stm->assign.exp);
            printf("%sid: %s\n", tab.data, stm->assign.id->symbol.data);
            printf("%s}\n", tab.data);
            break;
        case ST_EXP:
            printf("%sexpression {\n", tab.data);
            // todo print_exp(stm->exp.exp);
            printf("%s}\n", tab.data);
            break;
        case ST_FUNC:
            printf("%sfunction {\n", tab.data);
            printf("%sid: %s\n", tab.data, stm->func.id->symbol.data);
            printf("%sused: %s\n", tab.data, stm->func.used ? "true" : "false");
            printf("%sparam_count: %zu\n", tab.data, stm->func.param_count);
            // todo print_exp(stm->exp.exp);
            printf("%sreturn_type: ", tab.data);
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
            Statement* stm_func = stm->func.body;
            while (stm_func) {
                print_statement(stm_func, ++tabs);
                stm_func = stm_func->next;
            }
            printf("}\n");
            break;
        case ST_IF:
            printf("%sif {\n", tab.data);
            // todo print_exp(stm->if_.exp)
            printf("%scheck_nil: %s\n", tab.data, stm->if_.check_nil ? "true" : "false");
            printf("%sbody {\n", tab.data);
            Statement* stm_if = stm->if_.body;
            while (stm_if) {
                print_statement(stm_if, ++tabs);
                stm_if = stm_if->next;
            }
            printf("%s}\n", tab.data);
            stm_if = stm->if_.else_;
            printf("%selse {\n", tab.data);
            while (stm_if) {
                print_statement(stm_if, ++tabs);
                stm_if = stm_if->next;
            }
            printf("%s}\n", tab.data);
            break;
        case ST_RETURN:
            printf("%sreturn {\n", tab.data);
            // todo print_exp(stm->return_.exp);
            printf("%s}\n", tab.data);
            break;
        case ST_VAR:
            printf("%svariable {\n", tab.data);
            printf("%sid: %s\n", tab.data, stm->var.id->symbol.data);
            // todo print_exp(stm->var.exp);
            printf("%smodifiable: %s\n", tab.data, stm->var.modifiable ? "true" : "false");
            printf("%sdata_type: ", tab.data);
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
            printf("%sused: %s\n", tab.data, stm->var.used ? "true" : "false");
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

void print_exp(Expression* exp) {
}
