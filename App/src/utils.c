#include <utils.h>

//clears the screen
void clrscr(){
    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
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





