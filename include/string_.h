#pragma once

#include <stddef.h>
#include <stdbool.h>

#define STR_INITIAL_CAPACITY 16 //TODO kdyztak zmenit na jinou hodnotu

typedef struct String {
    char* data;
    size_t length, capacity;
} String;

void init_string( String* );
void clear_string( String* );

void string_append( String*, const char* );
void string_append_c( String*, char );

bool string_eq( const String*, const String* );

String string_copy( const String* );

