#ifndef PCSPKR_H
#define PCSPKR_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <utils.h>


void pcspkr_tone(uint64_t hz);
void pcspkr_stop();


#endif