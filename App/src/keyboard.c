#include <keyboard.h>

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


