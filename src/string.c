#include "../include/string.h"

#include <stdlib.h>

//TODO: je potreba checkovat jestli je string NULL nebo si to ma zkontrolovat uzivatel?

void init_string(String *string) {
    if (string == NULL)
        return;
    
    string->capacity = STR_INITIAL_CAPACITY;
    string->length = 0;

    string->data = (char *) malloc(sizeof(char) * STR_INITIAL_CAPACITY);

    if (string->data == NULL) {
        exit(1); //TODO: tady by se mozna melo neco vracet z te funkce
    }
}

void clear_string (String *string) {
    if (string == NULL)
        return;
    
    string->length = 0;
    string->capacity = STR_INITIAL_CAPACITY;

    free(string->data);
    string->data = NULL;
}

void string_append(String * string, const char * str_to_append) {
    if (string == NULL)
        return;
    
    for (int i = 0; str_to_append[i] != '\0'; i++) {
        if (string->length + 1 > string->capacity) { // pokud by se to po pridani znaku uz nevlezlo, zvetsi se 2x
            string->data = (char *) malloc(sizeof(char) * string->capacity * 2);

            if (string->data == NULL) {
                exit(1); //TODO: asi zase neco vracet
            }

            string->capacity *= 2;
        } else {
            string->data[(string->length)++] = str_to_append[i];
        }
    }
}
