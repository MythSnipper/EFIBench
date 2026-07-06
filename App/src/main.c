#include <main.h>

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

