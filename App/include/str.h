#ifndef STR_H
#define STR_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <mem.h>

uint64_t strlen(char* str);
uint64_t wstrlen(wchar_t* str);
char* StrAppend(char* a, char* b);

void charstr_to_wcharstr(char* src, wchar_t* dst);
void wcharstr_to_charstr(wchar_t* src, char* dst);


#endif