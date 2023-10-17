#pragma once

#include <stddef.h>

typedef struct SymbolRecord {
    char* symbol;
} SymbolRecord;

typedef struct SymbolTable {
    SymbolRecord* symbols;
    size_t size;
} SymbolTable;

SymbolTable* init_symboltable( void );
void free_symboltable( SymbolTable* );

void symboltable_insert( SymbolTable*, const char* symbol );

