#include <cpu.h>

bool cpu_has_aes(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);
    return (ecx & (1 << 25)) != 0;
}

bool cpu_has_avx(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);

    bool avx = (ecx & (1 << 28)) != 0;

    return avx;
}

bool cpu_has_avx2(){
    uint32_t eax, ebx, ecx, edx;

    if(!cpu_has_avx()){
        return false;
    }

    cpuid(0, 0, &eax, &ebx, &ecx, &edx);
    if(eax < 7){
        return false;
    }

    cpuid(7, 0, &eax, &ebx, &ecx, &edx);
    return (ebx & (1 << 5)) != 0;
}

bool cpu_avx_usable(){
    if(!cpu_has_avx()){
        return false;
    }

    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);

    //do checks for AVX and OSXSAVE 
    bool osxsave = (ecx & (1 << 27)) != 0;
    bool sxsave = (ecx & (1 << 26)) != 0;

    if(!osxsave || !sxsave){
        return false;
    }

    //then check xcr0 to see if xmm and ymm states are enabled
    uint64_t xcr0 = xgetbv(0);

    return (xcr0 & 0x6) == 0x6;
}


bool cpu_has_sse(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);
    return (edx & (1 << 25)) != 0;
}

bool cpu_has_sse2(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);
    return (edx & (1 << 26)) != 0;
}



void cpu_get_model(char* model){
    uint32_t eax, ebx, ecx, edx;
    cpuid(0x80000000, 0, &eax, &ebx, &ecx, &edx);

    //check if model string available
    if(eax < 0x80000004){
        model[0] = '\0';
    }

    uint32_t* dst = (uint32_t*)model;
    //copy from model leaf to string
    for(uint64_t leaf=0x80000002;leaf<=0x80000004;leaf++){
        cpuid(leaf, 0, &eax, &ebx, &ecx, &edx);
        *dst++ = eax;
        *dst++ = ebx;
        *dst++ = ecx;
        *dst++ = edx;
    }

    model[48] = '\0';
}










