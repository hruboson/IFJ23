#include "../include/string.h"

#include <stdlib.h>

void init_string(String *string) {
    if (string == NULL)
        return;

    string->capacity = STR_INITIAL_CAPACITY;
    string->length = 0;

    string->data = (char *)malloc(sizeof(char) * STR_INITIAL_CAPACITY);

    if (string->data == NULL) {
        exit(99);
    }
}

void free_string(String *string) {
    if (string == NULL)
        return;

    string->length = 0;
    string->capacity = 0;

    free(string->data);
    string->data = NULL;
}

void append_string(String *string, const char *str_to_append) {
    if (string == NULL)
        return;

    for (int i = 0; str_to_append[i] != '\0'; i++) {
        if (string->length + 1 > string->capacity) {  // pokud by se to po pridani znaku uz nevlezlo, zvetsi se 2x
            char *new_data = (char *)realloc(string->data, sizeof(char) * string->capacity * 2);

            if (new_data == NULL) {
                free_string(string);
                exit(99);
            }
            string->data = new_data;

            string->capacity *= 2;
        }
        string->data[(string->length)++] = str_to_append[i];
    }
    
}

void append_string_c(String *string, char c) {
    if (string == NULL)
        return;

    if (string->length + 2 > string->capacity) {
        char *new_data = (char *)realloc(string->data, sizeof(char) * string->capacity * 2);
        if (new_data == NULL) {
            free_string(string);
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
