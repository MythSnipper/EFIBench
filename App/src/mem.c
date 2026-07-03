#include <mem.h>

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


