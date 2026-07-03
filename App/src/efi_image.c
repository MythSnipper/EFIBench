#include <efi_image.h>


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



