#include "ir.h"
#include "parser.h"

int
main() {

	Input in = {
		.type = INT_FILE,
		.file.f = stdin,
	};

	/* init resources */
	AST ast;
	init_ast( &ast );
	IR ir;
	init_ir( &ir );

	int ret;

	ret = parse( &in, &ast );
	if ( ret ) {
		return ret;
	}

	/* convert AST to internal representation */
	ret = convert_to_ir( &ast, &ir );
	clear_ast( &ast );
	if ( ret ) {
		return ret;
	}

	print_ir( stdout, &ir );

	clear_ir( &ir );

	return ret;
}
