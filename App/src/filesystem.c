#include <filesystem.h>


//reads a file from its file path and returns a pointer to the start
char* read_file(wchar_t* filepath, uint64_t* file_size_out){

    EFI_STATUS status;

    //get LoadedImageProtocol
    EFI_LOADED_IMAGE_PROTOCOL* LIP;
    status = uefi_call_wrapper(BS->HandleProtocol, 3, IH, &gEfiLoadedImageProtocolGuid, (void**)&LIP);
    if(EFI_ERROR(status)){
        Print(L"No LoadedImageProtocol\r\n");
        hang();
    }

    //Now get the correct SFSP corresponding to where the current image is located
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SFSP;
    status = uefi_call_wrapper(BS->HandleProtocol, 3, LIP->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&SFSP);
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



