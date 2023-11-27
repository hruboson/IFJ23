#include "../include/string_.h"

#include <stdlib.h>
#include <string.h>

void init_string(String *string) {
	if (string == NULL)
		return;

	string->capacity = STR_INITIAL_CAPACITY;
	string->length = 0;

	string->data = (char *)malloc(sizeof(char) * STR_INITIAL_CAPACITY);

	if (string->data == NULL) {
		exit(99);
	}

	string->data[ 0 ] = 0;
}

void clear_string(String *string) {
	if (string == NULL)
		return;

	string->length = 0;
	string->capacity = 0;

	free(string->data);
	string->data = NULL;
}

void string_append(String *string, const char *str_to_append) {
	if (string == NULL || str_to_append == NULL || str_to_append[ 0 ] == '\0')
		return;

	size_t append_length = strlen( str_to_append );
	size_t old_length = string->length;
	string->length += append_length;
	bool cap_changed = false;

	while ( string->length + 2 > string->capacity ) {
		cap_changed = true;
		string->capacity *= 2;
	}

	if ( cap_changed ) {
		char* new_data = realloc( string->data, string->capacity );
		if ( new_data == NULL ) {
			exit( 99 );
		}
		string->data = new_data;
	}

	memcpy( string->data + old_length, str_to_append, append_length );
	string->data[string->length] = '\0';
}

void string_append_c(String *string, char c) {
	if (string == NULL)
		return;

	if (string->length + 2 > string->capacity) {
		char *new_data = (char *)realloc(string->data, sizeof(char) * string->capacity * 2);
		if (new_data == NULL) {
			clear_string(string);
			exit(99);
		}
		string->data = new_data;

		string->capacity *= 2;
	}

	string->data[(string->length)++] = c;
	string->data[(string->length)] = '\0';
}

bool string_eq( const String* a, const String* b ) {
	if ( a == NULL || b == NULL )
		return false;

	if ( a == b )
		return true;

	if ( a->length != b->length )
		return false;

	return strcmp( a->data, b->data ) == 0;
}
