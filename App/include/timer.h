#ifndef TIMER_H
#define TIMER_H


#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <mem.h>
#include <str.h>

//uefi timer shenanigans measures in weird 100 ns units
#define UEFI_1_MS 10000 


void sleep(double ms);
EFI_TIME getTime();
char* getTimeStr();
char* genTimeStr(EFI_TIME time);



#endif