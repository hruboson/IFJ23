#include "semantic.h"
#include "ir_generator.h"
#include "code_generator.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE* source_code = stdin;

    int ret;

    AST ast;
    ret = semantic( source_code, &ast );
    if ( ret ) {
        free_ast( &ast );
        //
        return ret;
    }

    IR ir;
    ret = convert_to_ir( &ast, &ir );
    free_ast( &ast );
    if ( ret ) {
        // 
        return ret;
    }

    // optimize
    // TODO

    String dest_code;
    ret = generate_code( &ir, &dest_code );
    if ( ret ) {
        //
        return ret;
    }

    fputs( dest_code.data );
    return 0;
}
