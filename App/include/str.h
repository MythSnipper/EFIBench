#ifndef STR_H
#define STR_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <mem.h>

uint64_t strlen(char* str);
uint64_t wstrlen(wchar_t* str);
char* StrAppend(char* a, char* b);
uint64_t StrCopy(char* src, char* dst);


void charstr_to_wcharstr(char* src, wchar_t* dst);
void wcharstr_to_charstr(wchar_t* src, char* dst);

void sprint2ch(char* buf, uint64_t num);
void sprint4ch(char* buf, uint64_t num);

//defines for sprint fixed
#define SPRINT_FIXED_SCALE 1000000
#define SPRINT_FIXED_WIDTH 20
void sprint_fixed_double(char* buf, double num);
void sprint_fixed_int(char* buf, uint64_t num);
double parse_fixed_double(char* buf);


#endif