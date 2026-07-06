#ifndef TIMER_H
#define TIMER_H


#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

//uefi timer shenanigans measures in weird 100 ns units
#define UEFI_1_MS 10000 


void sleep(double ms);



#endif