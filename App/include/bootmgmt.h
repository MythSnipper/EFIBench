#ifndef BOOTMGMT_H
#define BOOTMGMT_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <mem.h>
#include <str.h>

#define MAX_BOOT_ENTRIES 1024
#define MAX_BOOT_ENTRY_NAME_LEN 64
#define MAX_BOOT_ENTRY_PATH_LEN 1024

typedef struct boot_entry{
    wchar_t name[MAX_BOOT_ENTRY_NAME_LEN];
    wchar_t path[MAX_BOOT_ENTRY_PATH_LEN];
} boot_entry;

#define ENTRIES_FILE_PATH L"\\EFIBench\\entries.txt"

uint64_t parse_boot_entries(char* filedata, boot_entry** entries_ret);

#endif