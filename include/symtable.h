#pragma once

#include <stddef.h>

#include "string.h"

typedef struct SymbolRecord {
    String symbol;
    size_t hash;
} SymbolRecord;

typedef struct SymbolTable {
    SymbolRecord* symbols;
    size_t size;
} SymbolTable;

void init_symboltable( SymbolTable* );
void clear_symboltable( SymbolTable* );
void free_symboltable( SymbolTable* );

int symboltable_insert( SymbolTable*, String* symbol, size_t* index );

