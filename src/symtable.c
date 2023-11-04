#include "symtable.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ST_DEFAULT_SIZE 1043

// djb2
size_t
hash( String* s ) {
	size_t hash = 5381;
        int c;
	const char* str = s->data;

        while ( ( c = *str++ ) )
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
}

void
free_symtab_record( SymbolRecord* r ) {
	clear_string( &r->symbol );
	free( r );
}

void
init_symboltable( SymbolTable* st ) {
	st->size = ST_DEFAULT_SIZE;
	st->count = 0;
	st->symbols = malloc( sizeof( *st->symbols ) * st->size );
	if ( st->symbols == NULL )
		exit( 99 );

	memset( st->symbols, 0, st->size * sizeof( *st->symbols ) );
}

void
clear_symboltable( SymbolTable* st ) {

	for ( size_t i = 0; i < st->size; i++ ) {
		if ( st->symbols[ i ] == NULL )
			continue;

		free_symtab_record( st->symbols[ i ] );
	}

	st->size = 0;
	st->count = 0;
	free( st->symbols );
	st->symbols = NULL;
}

void
free_symboltable( SymbolTable* st ) {
	clear_symboltable( st );
	free( st );
}

int
symboltable_insert( SymbolTable* st, String* symbol, SymbolRecord** rec ) {
	if ( st == NULL || symbol == NULL || rec == NULL )
		return 99;

	if ( st->count * 60 / 100 > st->size ) {
		/* increase hash table size */

		size_t new_size = st->size * 2 + 1;

		// number overflow, malloc should fail before this
		if ( new_size < st->size )
			return 99;

		SymbolRecord** new_symbols = malloc( sizeof( SymbolRecord* ) * new_size );
		if ( new_symbols == NULL )
			return 99;

		memset( new_symbols, 0, new_size * sizeof( *st->symbols ) );

		/* fill new table */
		for ( size_t i = 0; i < st->size; i++ ) {
			if ( st->symbols[ i ] == NULL )
				continue;

			SymbolRecord* r = st->symbols[ i ];

			size_t s_i = r->hash % new_size;

			do {
				if ( st->symbols[ s_i ] == NULL ) {
					st->symbols[ s_i ] = r;
					break;
				}

				s_i = ( s_i + 1 ) % new_size;
			} while ( true );
		}

		/* replace old symbols with the new ones */
		free( st->symbols );
		st->symbols = new_symbols;
		st->size = new_size;
	}

	/* insert new symbol */
	const size_t h = hash( symbol );
	size_t i = h % st->size;

	do {
		/* insert into an empty cell */
		if ( st->symbols[ i ] == NULL ) {
			SymbolRecord* r = malloc( sizeof( SymbolRecord ) );
			if ( r == NULL )
				exit( 99 );

			st->symbols[ i ] = r;
			*rec = r;

			r->symbol = *symbol;
			r->hash = h;

			st->count++;

			return 0;
		}

		/* same symbol */
		if (
			st->symbols[ i ]->hash == h &&
			string_eq( symbol, &st->symbols[ i ]->symbol )
		) {
			*rec = st->symbols[ i ];

			// clear string
			clear_string( symbol );
			return 0;
		}

		i = ( i + 1 ) % st->size;

	} while ( true );
}
