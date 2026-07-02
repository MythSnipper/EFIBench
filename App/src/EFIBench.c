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


uint64_t run_selection_menu(wchar_t* title, wchar_t** entries_normal, boot_entry* entries_boot, uint64_t entries_count, uint64_t selected){
    //selected entry(index of entries)
    uint64_t selected_entry = selected;

    //construct strings of entries
    wchar_t** entries = malloc(sizeof(wchar_t*) * entries_count);
    
    if(entries_normal != NULL){
        for(int i=0;i<entries_count;i++){
            entries[i] = entries_normal[i];
        }
    }
    if(entries_boot != NULL){
        for(int i=0;i<entries_count;i++){
            entries[i] = entries_boot[i].name;
        }
    }

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
        selected = run_selection_menu(L"EFIBench", entries, NULL, entries_count, selected);
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
    wchar_t* entries[] = {
        L"Back",
    };
    uint64_t entries_count = sizeof(entries)/sizeof(entries[0]);
    
    //make selected entry persistent so it doesn't start at the first one every time
    uint64_t selected = 0;
    
    while(1){
        selected = run_selection_menu(L"Boot Entries", entries, NULL, entries_count, selected);
        //go to another menu
        switch(selected){
            case 0:
                return;
            break;
        }
    }
}

void menu_benchmarks(){
    wchar_t* entries[] = {
        L"Back"
    };
    uint64_t entries_count = sizeof(entries)/sizeof(entries[0]);

    //make selected entry persistent so it doesn't start at the first one every time
    uint64_t selected = 0;

    while(1){
        selected = run_selection_menu(L"Benchmarks", entries, NULL, entries_count, selected);
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
        selected = run_selection_menu(L"View Previous Results", entries, NULL, entries_count, selected);
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
        selected = run_selection_menu(L"Settings", entries, NULL, entries_count, selected);
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
        Print(L"No File\r\n");
        hang();
    }

    //get file size
    EFI_FILE_INFO* file_info = LibFileInfo(file);
    if(file_info == NULL){
        Print(L"No file info\r\n");
        hang();
    }

    uint64_t file_size = file_info->FileSize;
    char* buf = malloc(file_size + 1); //include null terminator

    uint64_t read_size = file_size;
    status = uefi_call_wrapper(file->Read, 3, file, &read_size, buf);
    if(EFI_ERROR(status)){
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
        Print(L"No File\r\n");
        hang();
    }

    uint64_t filesize = strlen(buf);
    status = uefi_call_wrapper(file->Write, 3, file, &filesize, buf);
    if(EFI_ERROR(status)){
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
        Print(L"No Load Image\r\n");
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

//parse file data to entries, fill entries array, and return number of entries
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
    uint64_t size;
    char* data = read_file(L"\\EFIBench\\entries.txt", &size);

    boot_entry* entries;
    parse_boot_entries(data, &entries);

    if(data != NULL){
        Print(L"File size: %u bytes\r\n", size);
        Print(L"%a\r\n", data);

        free(data);
    }

    write_file(L"\\testwrite.txt", "1234\r\n");
    
    start_efi_image(L"\\EFI\\BOOT\\SHELLX64.EFI");
    hang();
}