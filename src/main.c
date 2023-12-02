#include "parser.h"
#include "ir.h"
#include "code_generator.h"

#include <stdio.h>

int process_file( Input* in, AST* ast, IR* ir, String* target_code ) {

	int ret;

	/* convert source code into an AST */
	ret = parse(in, ast);
	if ( ret ) {
		return ret;
	}

	/* convert AST to internal representation */
	ret = convert_to_ir( ast, ir );
	clear_ast( ast );
	if ( ret ) {
		return ret;
	}

	// optimize
	// TODO

	/* convert internal code to target code */
	ret = generate_code( ir, target_code );
	if ( ret ) {
		return ret;
	}

	return 0;
}

int main(int argc, char const *argv[]) {
	(void)argc; (void)argv;

	Input in = {
		.type = INT_FILE,
		.file.f = stdin,
	};

	/* init resources */
	AST ast;
	init_ast( &ast );
	IR ir;
	init_ir( &ir );
	String target_code;
	init_string( &target_code );

	/* process source code into target code */
	int ret = process_file( &in, &ast, &ir, &target_code );
	if ( ret == 0 ) { /* on success */
		/* output target code to stdout */
		fputs( target_code.data, stdout );
	}

	/* free resources */
	clear_ast( &ast );
	clear_ir( &ir );
	clear_string( &target_code );

	return ret;
}
