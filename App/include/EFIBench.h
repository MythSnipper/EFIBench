#ifndef EFIBench_H
#define EFIBench_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#define COLOR_NORMAL EFI_GREEN | EFI_BACKGROUND_BLACK
#define COLOR_HIGHLIGHT EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK

#define MAX_BOOT_ENTRIES 1024
#define MAX_BOOT_ENTRY_NAME_LEN 64
#define MAX_BOOT_ENTRY_PATH_LEN 1024

typedef struct{
    wchar_t name[MAX_BOOT_ENTRY_NAME_LEN];
    wchar_t path[MAX_BOOT_ENTRY_PATH_LEN];
} boot_entry;

uint64_t run_selection_menu(wchar_t* title, wchar_t** entries_normal, boot_entry* entries_boot, uint64_t entries_count, uint64_t selected);
void menu_main();
void menu_boot();
void menu_benchmarks();
void menu_view_previous();
void menu_settings();

void* malloc(uint64_t bytes);
void free(void* ptr);
void clrscr();
EFI_INPUT_KEY get_key();
void set_color(uint64_t attrib);
void hang();

uint64_t strlen(char* str);
uint64_t wstrlen(wchar_t* str);

char* read_file(wchar_t* filepath, uint64_t* file_size_out);
void write_file(wchar_t* filepath, void* buf);
void append_file(wchar_t* filepath, void* buf);

void start_efi_image(wchar_t* filepath);

void charstr_to_wcharstr(char* src, wchar_t* dst);

uint64_t parse_boot_entries(char* filedata, boot_entry** entries_ret);

void test();


#endif