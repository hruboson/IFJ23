#include "symtable.h"

#include <stdlib.h>

void init_symboltable( SymbolTable* st ) { /* TODO */ }
void clear_symboltable( SymbolTable* st ) { /* TODO */ }

void free_symboltable( SymbolTable* st ) {
	clear_symboltable( st );
	free( st );
}

int symboltable_insert( SymbolTable* st, String* symbol, size_t* index ) {
	/* TODO */
	return 0;
}
