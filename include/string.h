#pragma once

#include <stddef.h>
#include <stdbool.h>

#define STR_INITIAL_CAPACITY 16 //TODO kdyztak zmenit na jinou hodnotu

typedef struct String {
    char* data;
    size_t length, capacity;
} String;

void init_string( String* );
void free_string( String* );

void append_string( String*, const char* );
void append_string_c( String*, char );

bool string_eq( const String*, const String* );
