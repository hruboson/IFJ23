#include "AST.h"
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
	int ret;

	ret = parse( &in, &ast );
	if ( ret ) {
		return ret;
	}

	print_ast( &ast );

	clear_ast( &ast );

	return ret;
}
