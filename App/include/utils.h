#ifndef UTILS_H
#define UTILS_H


#include <efi.h>
#include <efilib.h>
#include <stdbool.h>


void clrscr();
void set_color(uint64_t attrib);
void set_cursor_pos(uint64_t x, uint64_t y);
void hang();



#endif