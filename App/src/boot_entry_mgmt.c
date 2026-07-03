#include <boot_entry_mgmt.h>

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


