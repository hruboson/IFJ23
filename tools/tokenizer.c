#include "scanner.h"

#include <assert.h>
#include "utils.h"

int
main() {
	Input in = {
		.type = INT_FILE,
		.file.f = stdin,
	};

	SymbolTable symtab;
	init_symboltable( &symtab );

	bool run = true;
	int ret;
	while ( run ) {
		Token token;
		ret = get_token( &in, &symtab, &token );
		if ( ret != 0 ) {
			fprintf( stderr, "error: get_token %i\n", ret );
			break;
		}

		print_token( &token );
		if ( token.type == TOKENTYPE_EOF )
			break;

		if ( token.type == TOKENTYPE_STRING )
			clear_string( &token.value.str_ );
	}

	clear_symboltable( &symtab );

}
