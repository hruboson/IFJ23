#pragma once

#include <stddef.h>

typedef struct String {
    char* data;
    size_t length, capacity;
} String;

void init_string( String* );
void clear_string( String* );

void string_append( String*, const char* );
void string_append_c( String*, char );
