#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <mem.h>
#include <str.h>
#include <utils.h>

extern EFI_HANDLE IH;

char* read_file(wchar_t* filepath, uint64_t* file_size_out);
void write_file(wchar_t* filepath, void* buf);
void append_file(wchar_t* filepath, void* buf);


#endif