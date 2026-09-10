#include <efi_main.h>
#include <ui.h> //definitions for ui colors

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){
    InitializeLib(ImageHandle, SystemTable); //initialize GNU-EFI during runtime

    //set to be used globally
    IH = ImageHandle;

    //disable watchdog timer
    uefi_call_wrapper(BS->SetWatchdogTimer, 4, 0, 0, 0, NULL);
    //set mode to 80x25 text
    uefi_call_wrapper(ST->ConOut->SetMode, 2, ST->ConOut, 0);
    //disable cursor
    uefi_call_wrapper(ST->ConOut->EnableCursor, 2, ST->ConOut, 0);
    //set text color
    set_color(COLOR_NORMAL);
    //clear screen
    clrscr();

    //transfer control to C++ main
    cpp_main();
    //hang if cpp_main exits, unintended behavior
    hang();

    return EFI_SUCCESS;
}

