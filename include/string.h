#pragma once

#include <stddef.h>

#define STR_INITIAL_CAPACITY 16 //TODO kdyztak zmenit na jinou hodnotu

typedef struct String {
    char* data;
    size_t length, capacity;
} String;

void init_string( String* );
void clear_string( String* );

void string_append( String*, const char* );
