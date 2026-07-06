#ifndef CPU_H
#define CPU_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

void cpuid(uint32_t leaf, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx);



#endif