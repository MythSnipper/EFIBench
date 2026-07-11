#ifndef CPU_H
#define CPU_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>




bool cpu_has_aes();
bool cpu_has_avx();
bool cpu_has_avx2();
bool cpu_avx_usable();
bool cpu_has_sse();
bool cpu_has_sse2();

void cpu_get_model(char* model);


//use CPUID to get cpu features
static inline void cpuid(uint32_t leaf, uint32_t subleaf, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx){
    __asm__ volatile(
        "cpuid"
        : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
        : "a"(leaf), "c"(subleaf)
    );
}

//get value of an ECR register
static inline uint64_t xgetbv(uint32_t index){
    uint32_t eax;
    uint32_t ebx;
    __asm__ volatile(
        "xgetbv"
        : "=a"(eax), "=b"(ebx)
        : "c"(index)
    );
    return ((uint64_t)ebx << 32) | eax;
}


//port output
static inline void outb(uint16_t port, uint8_t value){
    __asm__ volatile(
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}
//port input
static inline uint8_t inb(uint16_t port){
    uint8_t value;
    __asm__ volatile(
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );
    return value;
}

//read the TSC timer
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