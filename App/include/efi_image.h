#ifndef EFI_IMAGE_H
#define EFI_IMAGE_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <utils.h>

extern EFI_HANDLE IH;

void start_efi_image(wchar_t* filepath);



#endif