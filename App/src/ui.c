
#include <ui.h>


uint64_t run_selection_menu(wchar_t* title, wchar_t** entries, uint64_t entries_count, uint64_t selected){
    if (entries_count == 0) {
        return 0;
    }
    
    //selected entry(index of entries)
    int64_t selected_entry = selected;

    //Loop
    while(1){
        //clear screen
        clrscr();

        //Title
        Print(L"%s\r\n", title);

        //determine frame of entries to display
        uint64_t draw_start_index;
        uint64_t draw_end_index;

        //indicators
        bool more_entries_up_top_indicator = false; //the ^^^ indicator on the first entry displayed
        bool more_entries_down_below_indicator = false; //the vvv indicator on the last entry displayed

        //screen can display 23 entries
        //modify display frame based on cursor position, if frame of entries is greater than 23
        if(entries_count > 23){ //more entries than the screen can display
            //if cursor is in the first 12 entries, display min frame
            if(selected_entry < 12){
                //MIN FRAME 0-22
                draw_start_index = 0;
            }
            //if cursor is in the last few entries, display max frame
            else if(selected_entry > entries_count-9){
                //MAX FRAME (if entries_count=24 it is 1-23)
                draw_start_index = entries_count-23;
            }
            //otherwise try to make cursor fit in the middle I guess
            else{
                draw_start_index = (selected_entry-12 > entries_count-23) ? entries_count-23 : selected_entry-12;
            }
            //end calculated based on start
            draw_end_index = draw_start_index+22;
            
            //also set indicators
            more_entries_up_top_indicator = draw_start_index != 0; //the ^^^ indicator on the first entry displayed
            more_entries_down_below_indicator = draw_start_index != entries_count-23; //the vvv indicator on the last entry displayed
        }
        else{
            //return normal display frame
            draw_start_index = 0;
            draw_end_index = entries_count-1;
        }

        //display entries from start index to end index, inclusive
        for(int i=draw_start_index;i<=draw_end_index;i++){
            //use highlight color if selected
            if(i == selected_entry)set_color(COLOR_HIGHLIGHT);
            
            if(i == selected_entry){
                Print(L"   >%s", entries[i]);
            }
            else{
                Print(L"    %s", entries[i]);
            }

            if(i == draw_start_index && more_entries_up_top_indicator){
                //display the ^^^ indicator
                Print(L"       ↑");
            }
            if(i == draw_end_index && more_entries_down_below_indicator){
                //display the vvv indicator
                Print(L"       ↓");
            }

            Print(L"\r\n");

            //reset highlight color if selected
            if(i == selected_entry)set_color(COLOR_NORMAL);
        }

        //read keystroke
        EFI_INPUT_KEY key = get_key();

        //update index based on key
        if(key.UnicodeChar == L'\r'){ //enter key
            return selected_entry;
        }
        if(key.ScanCode == SCAN_UP || key.UnicodeChar == L'w'){
            selected_entry--;
        }
        if(key.ScanCode == SCAN_DOWN || key.UnicodeChar == L's'){
            selected_entry++;
        }
        //restrain and loop selected entry to be in range
        if(selected_entry == -1)selected_entry = entries_count-1;
        if(selected_entry == entries_count)selected_entry = 0;
    }

}

//entries count does not include Back and Edit
uint64_t run_selection_menu_boot(wchar_t* title, boot_entry* entries_boot, uint64_t entries_count, uint64_t selected){
    //selected entry(index of entries)
    int64_t selected_entry = selected;

    //construct strings of entries
    wchar_t** entries = NULL;
    entries = malloc(sizeof(wchar_t*) * (entries_count+3));
    entries[0] = L"Back";
    entries[1] = L"Add Entry";
    entries[2] = L"Remove Entry";
    for(int i=0;i<entries_count;i++){
        entries[i+3] = entries_boot[i].name;
    }
    entries_count += 3;

    uint64_t ret = run_selection_menu(title, entries, entries_count, selected);

    free(entries);

    return ret;
}

wchar_t* run_prompt(wchar_t* prompt, uint64_t max_len, bool* quit){
    wchar_t* ret = malloc(sizeof(wchar_t) * (max_len+1));
    ret[0] = L'\0';
    uint64_t ret_i = 0;

    *quit = false;

    while(1){
        clrscr();
        Print(L"%s", prompt);
        Print(L"%s", ret);

        set_cursor_pos(0, 24);
        Print(L"Enter to confirm, Esc to go back");
        EFI_INPUT_KEY key = get_key();

        if(key.UnicodeChar == '\r'){ //enter
            return ret;
        }
        if(key.ScanCode == SCAN_ESC){ //esc
            *quit = true;
            free(ret);
            return NULL;
        }
        if(key.UnicodeChar == L'\b'){ //backspace
            if(ret_i > 0){
                ret_i--;
                ret[ret_i] = L'\0';
            }
            continue;
        }
        if(ret_i < max_len && 
            key.UnicodeChar != 0 &&
            key.UnicodeChar >= 32 &&
            key.UnicodeChar != 127 &&
            key.UnicodeChar != L','){

            ret[ret_i] = key.UnicodeChar;
            ret_i++;
            ret[ret_i] = L'\0';
        }
    }
}

void menu_main(){
    wchar_t* entries[] = {
        L"Boot Entries",
        L"Run Benchmarks",
        L"View Previous Results",
        L"Settings"
    };
    uint64_t entries_count = sizeof(entries)/sizeof(entries[0]);
    
    //make selected entry persistent so it doesn't start at the first one every time
    uint64_t selected = 0;

    while(1){
        selected = run_selection_menu(L"EFIBench", entries, entries_count, selected);
        //go to another menu
        switch(selected){
            case 0:
                menu_boot();
            break;
            case 1:
                menu_benchmarks();
            break;
            case 2:
                menu_view_previous();
            break;
            case 3:
                menu_settings();
            break;
        }
    }
}

void menu_boot(){
    //read entries file
    uint64_t size;
    char* data = read_file(L"\\EFIBench\\entries.txt", &size);

    boot_entry* entries;
    uint64_t entries_count = parse_boot_entries(data, &entries);

    //make selected entry persistent so it doesn't start at the first one every time
    uint64_t selected = 0;
    
    while(1){
        selected = run_selection_menu_boot(L"Boot Entries", entries, entries_count, selected);
        //go to another menu
        switch(selected){
            case 0:
                free(data);
                free(entries);
                return;
            break;
            case 1:
                menu_boot_edit_add();
                free(data);
                free(entries);
                data = read_file(L"\\EFIBench\\entries.txt", &size);
                entries_count = parse_boot_entries(data, &entries);
            break;
            case 2:
                menu_boot_edit_remove();
                free(data);
                free(entries);
                data = read_file(L"\\EFIBench\\entries.txt", &size);
                entries_count = parse_boot_entries(data, &entries);
            break;
            default:
                start_efi_image(entries[selected-3].path);
        }
    }
}

void menu_boot_edit_add(){
    //read entries file
    uint64_t size;
    char* data = read_file(L"\\EFIBench\\entries.txt", &size);

    boot_entry* entries;
    uint64_t entries_count = parse_boot_entries(data, &entries);

    //prompt enter for name and path
    bool quit;
    
    wchar_t* name;
    name = run_prompt(L"Name for new boot entry\r\n> ", MAX_BOOT_ENTRY_NAME_LEN, &quit);
    if(quit){
        free(data);
        free(entries);
        return;
    }

    wchar_t* path;
    path = run_prompt(L"Path for new boot entry\r\n> ", MAX_BOOT_ENTRY_PATH_LEN, &quit);
    if(quit){
        free(name);
        free(data);
        free(entries);
        return;
    }


    //add new entry & inc entries_count
    boot_entry* new_entries = malloc(sizeof(boot_entry) * (entries_count+1));
    for(int i=0;i<entries_count;i++){
        new_entries[i] = entries[i];
    }
    free(entries);
    entries = new_entries;
    
    StrCpy(entries[entries_count].name, name);
    StrCpy(entries[entries_count].path, path);

    entries_count++;

    //construct string
    // :3
    char* file_content = malloc(1);
    *file_content = '\0';
    char* tmp;
    {
        //first get the name and the path and convert them to char*
        char* name = malloc(MAX_BOOT_ENTRY_NAME_LEN+1);
        wcharstr_to_charstr(entries[entries_count-1].name, name);

        char* path = malloc(MAX_BOOT_ENTRY_PATH_LEN+1);
        wcharstr_to_charstr(entries[entries_count-1].path, path);

        tmp = StrAppend(file_content, name);
        free(file_content);
        free(name);
        file_content = tmp;

        tmp = StrAppend(file_content, ",");
        free(file_content);
        file_content = tmp;

        tmp = StrAppend(file_content, path);
        free(file_content);
        free(path);
        file_content = tmp;

        tmp = StrAppend(file_content, "\r\n");
        free(file_content);
        file_content = tmp;
    }
    //append entry to file
    append_file(ENTRIES_FILE_PATH, file_content);

    free(file_content);
    free(name);
    free(path);
    free(data);
    free(entries);
}

void menu_boot_edit_remove(){
    //read entries file
    uint64_t size;
    char* data = read_file(ENTRIES_FILE_PATH, &size);

    boot_entry* entries;
    uint64_t entries_count = parse_boot_entries(data, &entries);

    //construct wchar_t** necessary for normal selection menu


    //select entry



    //delete selected entry


    free(data);
    free(entries);
}

void menu_benchmarks(){
    wchar_t* entries[] = {
        L"Back",
        L"Benchmark"
    };
    uint64_t entries_count = sizeof(entries)/sizeof(entries[0]);

    //make selected entry persistent so it doesn't start at the first one every time
    uint64_t selected = 0;

    while(1){
        selected = run_selection_menu(L"Benchmarks", entries, entries_count, selected);
        //go to another menu
        switch(selected){
            case 0:
                return;
            break;
            case 1:
                benchmark_run();
            break;
        }
    }
}

void menu_view_previous(){
    wchar_t* entries[] = {
        L"Back"
    };
    uint64_t entries_count = sizeof(entries)/sizeof(entries[0]);

    //make selected entry persistent so it doesn't start at the first one every time
    uint64_t selected = 0;

    while(1){
        selected = run_selection_menu(L"View Previous Results", entries, entries_count, selected);
        //go to another menu
        switch(selected){
            case 0:
                return;
            break;
        }
    }
}

void menu_settings(){
    wchar_t* entries[] = {
        L"Back",
    };
    uint64_t entries_count = sizeof(entries)/sizeof(entries[0]);

    //make selected entry persistent so it doesn't start at the first one every time
    uint64_t selected = 0;

    while(1){
        selected = run_selection_menu(L"Settings", entries, entries_count, selected);
        //go to another menu
        switch(selected){
            case 0:
                return;
            break;
        }
    }
}



