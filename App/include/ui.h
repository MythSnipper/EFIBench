#ifndef UI_H
#define UI_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <bootmgmt.h>
#include <utils.h>
#include <keyboard.h>
#include <filesystem.h>
#include <efi_image.h>
#include <pcspkr.h>

//forward declare
typedef struct bench_file_entry bench_file_entry;

#define COLOR_NORMAL EFI_GREEN | EFI_BACKGROUND_BLACK
#define COLOR_HIGHLIGHT EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK

uint64_t run_selection_menu(wchar_t* title, wchar_t** entries, uint64_t entries_count, uint64_t selected);
uint64_t run_selection_menu_benchmark();
uint64_t run_selection_menu_boot(wchar_t* title, boot_entry* entries_boot, uint64_t entries_count, uint64_t selected);
uint64_t run_selection_menu_view_previous(wchar_t* title, bench_file_entry* bench_entries, uint64_t entries_count, uint64_t selected);
uint64_t run_selection_menu_view_previous_edit(wchar_t* title, bench_file_entry* bench_entries, uint64_t entries_count, uint64_t selected);
void menu_view_result_detail(bench_file_entry* entry);
wchar_t* run_prompt(wchar_t* prompt, uint64_t max_len, bool* quit);
void menu_main();
void menu_boot();
void menu_boot_edit_add();
void menu_boot_edit_remove();
void menu_benchmarks();
void menu_view_previous();
void menu_view_previous_edit();
void menu_settings();


#endif