#include <cpu.h>

bool cpu_has_aes(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);
    return (ecx & (1 << 25)) != 0;
}

bool cpu_has_avx(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);

    //do checks for AVX and OSXSAVE 
    bool avx = (ecx & (1U << 28)) != 0;

    bool osxsave = (ecx & (1U << 27)) != 0;

    if(!avx || !osxsave){
        return false;
    }

    //then check xcr0 to see if xmm and ymm states are enabled
    uint64_t xcr0 = xgetbv(0);

    return (xcr0 & 0x6) == 0x6;
}

bool cpu_has_sse2(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);
    return (edx & (1 << 26)) != 0;
}
