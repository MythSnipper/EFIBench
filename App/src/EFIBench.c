#include <EFIBench.h>

EFI_HANDLE IH;

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){
    InitializeLib(ImageHandle, SystemTable);

    //set to global
    IH = ImageHandle;

    //disable watchdog timer
    uefi_call_wrapper(BS->SetWatchdogTimer, 4, 0, 0, 0, NULL);
    //set mode to 80x25 text
    uefi_call_wrapper(ST->ConOut->SetMode, 2, ST->ConOut, 0);
    //disable cursor
    uefi_call_wrapper(ST->ConOut->EnableCursor, 2, ST->ConOut, 0);
    //set color
    set_color(COLOR_NORMAL);
    //clear screen
    clrscr();

    menu_main();
    hang();

    return EFI_SUCCESS;
}

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
        L"Settings",
        L"Testing function"
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
            case 4:
                test();
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
        L"Back"
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
        L"Back"
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

//allocate bytes of contiguous memory
void* malloc(uint64_t bytes){
    void* buf = NULL;
    EFI_STATUS status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, bytes, &buf);
    if(EFI_ERROR(status)){
        return NULL;
    }
    if(buf == NULL){
        Print(L"malloc OOM\r\n");
        hang();
    }
    return buf;
}
//free allocated memory
void free(void* ptr){
    uefi_call_wrapper(BS->FreePool, 1, ptr);
}
//clears the screen
void clrscr(){
    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
}
//get a keystroke
EFI_INPUT_KEY get_key(){
    //wait for keystroke
    uint64_t index;
    uefi_call_wrapper(BS->WaitForEvent, 3, 1, &ST->ConIn->WaitForKey, &index);

    //read keystroke
    EFI_INPUT_KEY key;
    uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key);

    return key;
}
//set color attribute for drawing functions
void set_color(uint64_t attrib){
    //set color attribute
    uefi_call_wrapper(ST->ConOut->SetAttribute, 2, ST->ConOut, attrib);
}
//set cursor position to (x, y), where (0, 0) is top left
void set_cursor_pos(uint64_t x, uint64_t y){
    //uefi call
    uefi_call_wrapper(ST->ConOut->SetCursorPosition, 3, ST->ConOut, x, y);
}
//hangs the cpu on low power without ACPI shenanigans indefinitely
void hang(){
    while(1){
        __asm__ volatile("hlt");
    }
}

//gets the length of a string
uint64_t strlen(char* str){
    uint64_t len = 0;

    while(str[len] != '\0'){
        len++;
    }

    return len;
}
//gets the length of a wide character
uint64_t wstrlen(wchar_t* str){
    uint64_t len = 0;

    while(str[len] != L'\0'){
        len++;
    }

    return len;
}

//mallocs a new string which is the concatenation of the two strings
char* StrAppend(char* a, char* b){
    char* newstr = malloc(strlen(a)+strlen(b)+1);
    uint64_t vel = 0;

    while(*a != '\0'){
        newstr[vel] = *a;
        vel++;
        a++;
    }
    while(*b != '\0'){
        newstr[vel] = *b;
        vel++;
        b++;
    }
    newstr[vel] = '\0';

    return newstr;
}

//reads a file from its file path and returns a pointer to the start
char* read_file(wchar_t* filepath, uint64_t* file_size_out){

    EFI_STATUS status;

    //get SFSP from UEFI
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SFSP;
    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiSimpleFileSystemProtocolGuid, NULL, (void**)&SFSP);
    if(EFI_ERROR(status)){
        Print(L"No SFSP\r\n");
        hang();
    }

    EFI_FILE_PROTOCOL* root;
    status = uefi_call_wrapper(SFSP->OpenVolume, 2, SFSP, &root);
    if(EFI_ERROR(status)){
        Print(L"No Volume\r\n");
        hang();
    }

    EFI_FILE_PROTOCOL* file;
    status = uefi_call_wrapper(root->Open, 5, root, &file, filepath, EFI_FILE_MODE_READ, 0);
    if(EFI_ERROR(status)){
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No File\r\n");
        hang();
    }

    //get file size
    EFI_FILE_INFO* file_info = LibFileInfo(file);
    if(file_info == NULL){
        uefi_call_wrapper(file->Close, 1, file);
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No file info\r\n");
        hang();
    }

    uint64_t file_size = file_info->FileSize;
    char* buf = malloc(file_size + 1); //include null terminator

    uint64_t read_size = file_size;
    status = uefi_call_wrapper(file->Read, 3, file, &read_size, buf);
    if(EFI_ERROR(status)){
        free(buf);
        uefi_call_wrapper(file->Close, 1, file);
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No Read\r\n");
        hang();
    }

    buf[file_size] = '\0'; //set null terminator
    //return file size if the ptr is not null
    if(file_size_out != NULL){
        *file_size_out = file_size;
    }

    uefi_call_wrapper(file->Close, 1, file);
    uefi_call_wrapper(root->Close, 1, root);

    return buf;
}

//writes a null terminated string to a file
void write_file(wchar_t* filepath, void* buf){
    EFI_STATUS status;

    //get SFSP from UEFI
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SFSP;
    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiSimpleFileSystemProtocolGuid, NULL, (void**)&SFSP);
    if(EFI_ERROR(status)){
        Print(L"No SFSP\r\n");
        hang();
    }

    EFI_FILE_PROTOCOL* root;
    status = uefi_call_wrapper(SFSP->OpenVolume, 2, SFSP, &root);
    if(EFI_ERROR(status)){
        Print(L"No Volume\r\n");
        hang();
    }

    EFI_FILE_PROTOCOL* file;
    status = uefi_call_wrapper(root->Open, 5, root, &file, filepath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    if(EFI_ERROR(status)){
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No File\r\n");
        hang();
    }

    uint64_t filesize = strlen(buf);
    status = uefi_call_wrapper(file->Write, 3, file, &filesize, buf);
    if(EFI_ERROR(status)){
        uefi_call_wrapper(file->Close, 1, file);
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No Write\r\n");
        hang();
    }

    uefi_call_wrapper(file->Close, 1, file);
    uefi_call_wrapper(root->Close, 1, root);
}

//append a null terminated string to a file
void append_file(wchar_t* filepath, void* buf){
    EFI_STATUS status;

    //get SFSP from UEFI
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SFSP;
    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiSimpleFileSystemProtocolGuid, NULL, (void**)&SFSP);
    if(EFI_ERROR(status)){
        Print(L"No SFSP\r\n");
        hang();
    }

    EFI_FILE_PROTOCOL* root;
    status = uefi_call_wrapper(SFSP->OpenVolume, 2, SFSP, &root);
    if(EFI_ERROR(status)){
        Print(L"No Volume\r\n");
        hang();
    }

    EFI_FILE_PROTOCOL* file;
    status = uefi_call_wrapper(root->Open, 5, root, &file, filepath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    if(EFI_ERROR(status)){
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No File\r\n");
        hang();
    }

    //set position to end of file
    EFI_FILE_INFO* file_info = LibFileInfo(file);
    if(file_info == NULL){
        uefi_call_wrapper(file->Close, 1, file);
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No file info\r\n");
        hang();
    }

    uint64_t end_pos = file_info->FileSize;
    status = uefi_call_wrapper(file->SetPosition, 2, file, end_pos);
    if(EFI_ERROR(status)){
        uefi_call_wrapper(file->Close, 1, file);
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No set position\r\n");
        hang();
    }


    uint64_t filesize = strlen(buf);
    status = uefi_call_wrapper(file->Write, 3, file, &filesize, buf);
    if(EFI_ERROR(status)){
        uefi_call_wrapper(file->Close, 1, file);
        uefi_call_wrapper(root->Close, 1, root);
        Print(L"No Write\r\n");
        hang();
    }

    uefi_call_wrapper(file->Close, 1, file);
    uefi_call_wrapper(root->Close, 1, root);
}

//load an image into memory and start it from the file path
void start_efi_image(wchar_t* filepath){
    EFI_STATUS status;

    //get LoadedImageProtocol
    EFI_LOADED_IMAGE_PROTOCOL* LIP;
    status = uefi_call_wrapper(BS->HandleProtocol, 3, IH, &gEfiLoadedImageProtocolGuid, (void**)&LIP);
    if(EFI_ERROR(status)){
        Print(L"No LoadedImageProtocol\r\n");
        hang();
    }

    //get image path
    EFI_DEVICE_PATH_PROTOCOL* image_path = NULL;
    image_path = FileDevicePath(LIP->DeviceHandle, filepath);
    if(image_path == NULL){
        Print(L"No device path\r\n");
        hang();
    }

    //load image to memory
    EFI_HANDLE image_handle;
    status = uefi_call_wrapper(BS->LoadImage, 6, false, IH, image_path, NULL, 0, &image_handle);
    if(EFI_ERROR(status)){
        Print(L"No Load Image\r\n %r\r\n %s\r\n", status, filepath);
        hang();
    }

    //start image
    status = uefi_call_wrapper(BS->StartImage, 3, image_handle, NULL, NULL);
    if(EFI_ERROR(status)){
        Print(L"No StartImage\r\n");
        hang();
    }
}

//converts a char string to wchar_t string
void charstr_to_wcharstr(char* src, wchar_t* dst){
    uint64_t i = 0;

    while(src[i] != '\0'){
        dst[i] = (wchar_t)src[i];
        i++;
    }
    //add null terminator
    dst[i] = L'\0';
}

//converts a wchar_t string to char string
void wcharstr_to_charstr(wchar_t* src, char* dst){
    uint64_t i = 0;

    while(src[i] != L'\0'){
        dst[i] = (char)src[i];
        i++;
    }
    //add null terminator
    dst[i] = '\0';
}

//parse file data to entries, fill entries array, and return number of entries, entries is malloced
uint64_t parse_boot_entries(char* filedata, boot_entry** entries_ret){
    uint64_t filedata_i = 0; //index of next character

    boot_entry* entries = malloc(MAX_BOOT_ENTRIES * sizeof(boot_entry));
    uint64_t entry_i = 0; //next entry to fill

    
    //do scan until file EOF or max entries filled
    do{

        char namebuf[MAX_BOOT_ENTRY_NAME_LEN + 1] = {0};
        uint64_t namebuf_i = 0; //next index in namebuf

        //scan until comma for name, not overflowing namebuf
        while(filedata[filedata_i] != ',' && namebuf_i < MAX_BOOT_ENTRY_NAME_LEN){
            namebuf[namebuf_i] = filedata[filedata_i];
            namebuf_i++;
            filedata_i++;
        }

        //skip until comma
        while(filedata[filedata_i] != ',')filedata_i++;
        //skip until no comma
        while(filedata[filedata_i] == ',')filedata_i++;

        char pathbuf[MAX_BOOT_ENTRY_PATH_LEN + 1] = {0};
        uint64_t pathbuf_i = 0; //next index in pathbuf
        //scan until comma for path, not overflowing pathbuf
        while(filedata[filedata_i] != '\r' && filedata[filedata_i] != '\n' && pathbuf_i < MAX_BOOT_ENTRY_PATH_LEN){
            pathbuf[pathbuf_i] = filedata[filedata_i];
            pathbuf_i++;
            filedata_i++;
        }

        //skip until new line
        while(filedata[filedata_i] != '\r' && filedata[filedata_i] != '\n')filedata_i++;
        //skip until no new line
        while(filedata[filedata_i] == '\r' || filedata[filedata_i] == '\n')filedata_i++;

        //now add the name and path to a new entry
        
        //convert to wide char strings
        wchar_t namebuf_w[MAX_BOOT_ENTRY_NAME_LEN + 1] = {0};
        wchar_t pathbuf_w[MAX_BOOT_ENTRY_PATH_LEN + 1] = {0};

        charstr_to_wcharstr(namebuf, namebuf_w);
        charstr_to_wcharstr(pathbuf, pathbuf_w);

        //Print(L"Entry %d:\r\n%s,%s\r\n", entry_i+1, namebuf_w, pathbuf_w);

        //now copy them to a new entry
        StrCpy(entries[entry_i].name, namebuf_w);
        StrCpy(entries[entry_i].path, pathbuf_w);

        entry_i++;
    } while(
        filedata[filedata_i] != '\0' && //file did not reach EOF
        entry_i < MAX_BOOT_ENTRIES //next entry is not invalid index
    );

    *entries_ret = entries;
    return entry_i;
}

void test(){
    Print(L"TEST");
}