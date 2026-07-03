#ifndef MEMORY_H
#define MEMORY_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <utils.h>

void* malloc(uint64_t bytes);
void free(void* ptr);





#endif