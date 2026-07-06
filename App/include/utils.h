#ifndef UTILS_H
#define UTILS_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

void clrscr();
void set_color(uint64_t attrib);
void set_cursor_pos(uint64_t x, uint64_t y);
void hang();
void sleep(double ms);

static inline void outb(uint16_t port, uint8_t value){
    __asm__ volatile(
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

static inline uint8_t inb(uint16_t port){
    uint8_t value;
    __asm__ volatile(
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );
    return value;
}
 
static inline uint64_t rdtsc(){
    uint32_t low;
    uint32_t high;
    __asm__ volatile(
        "rdtsc"
        :
        "=a"(low), "=d"(high)
    );
    return ((uint64_t)high << 32) | low;
}


#endif