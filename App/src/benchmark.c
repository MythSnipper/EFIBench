#include <benchmark.h>

//returns tsc frequency in Hz after testing for [test_duration_ms] ms
double measure_tsc_freq(double test_duration_ms){
    uint64_t prev = rdtsc();
    sleep(test_duration_ms);
    uint64_t after = rdtsc();
    return ((after-prev) * 1000.0)/test_duration_ms;
}

//stresses the cpu for a certain amount of time
void stress_cpu(uint64_t ms, double tsc_freq_hz){
    uint64_t target = (uint64_t)((ms * tsc_freq_hz / 1000.0));
    uint64_t prev = rdtsc();

    volatile int64_t x = -892037925723;
    while((rdtsc() - prev) < target){
        x = x << 3;
        x = x - 28972354;
        x = x * 8239 + 395009253809;
    }
}

//ALU benchmarks, returns in op/sec
double intadd_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 417083295461378908\n"
        ".att_syntax\n"
        :
        :
        : "rax"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "add rax, 878235398\n"
            ".att_syntax\n"
            :
            :
            : "rax", "cc"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double intsub_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 417083295461378908\n"
        ".att_syntax\n"
        :
        :
        : "rax"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "sub rax, 878235398\n"
            ".att_syntax\n"
            :
            :
            : "rax", "cc"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double intmul_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 417083295461378908\n"
        ".att_syntax\n"
        :
        :
        : "rax"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "imul rax, 878235398\n"
            ".att_syntax\n"
            :
            :
            : "rax", "cc"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double intdiv_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mov rax, 687925563423235\n" //smh idiv requiring RDX:RAX :skul:
            "cqo\n" //quadword to octaword smh
            "mov rsi, 9\n"
            "idiv rsi\n"
            ".att_syntax\n"
            :
            :
            : "rax", "rdx", "rsi", "cc"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}

double bit_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 417083295461378908\n"
        ".att_syntax\n"
        :
        :
        : "rax"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "xor rax, 878235398\n"
            ".att_syntax\n"
            :
            :
            : "rax", "cc"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}



//FPU benchmarks
double f32add_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov rax, 0x3f800000\n"
        "movd xmm4, eax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "addss xmm0, xmm4\n"
            "addss xmm1, xmm4\n"
            "addss xmm2, xmm4\n"
            "addss xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}
double f32sub_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov eax, 0x3f800000\n"
        "movd xmm4, eax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "subss xmm0, xmm4\n"
            "subss xmm1, xmm4\n"
            "subss xmm2, xmm4\n"
            "subss xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}
double f32mul_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov eax, 0x3f800000\n"
        "movd xmm4, eax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mulss xmm0, xmm4\n"
            "mulss xmm1, xmm4\n"
            "mulss xmm2, xmm4\n"
            "mulss xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}
double f32div_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov eax, 0x3f800000\n"
        "movd xmm4, eax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "divss xmm0, xmm4\n"
            "divss xmm1, xmm4\n"
            "divss xmm2, xmm4\n"
            "divss xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}

double f64add_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "addsd xmm0, xmm4\n"
            "addsd xmm1, xmm4\n"
            "addsd xmm2, xmm4\n"
            "addsd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}
double f64sub_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "subsd xmm0, xmm4\n"
            "subsd xmm1, xmm4\n"
            "subsd xmm2, xmm4\n"
            "subsd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}
double f64mul_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mulsd xmm0, xmm4\n"
            "mulsd xmm1, xmm4\n"
            "mulsd xmm2, xmm4\n"
            "mulsd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}
double f64div_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "divsd xmm0, xmm4\n"
            "divsd xmm1, xmm4\n"
            "divsd xmm2, xmm4\n"
            "divsd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}



double sse_f32add_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov eax, 0x3f800000\n"
        "movd xmm4, eax\n"
        "shufps xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "addps xmm0, xmm4\n"
            "addps xmm1, xmm4\n"
            "addps xmm2, xmm4\n"
            "addps xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}
double sse_f32sub_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov eax, 0x3f800000\n"
        "movd xmm4, eax\n"
        "shufps xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "subps xmm0, xmm4\n"
            "subps xmm1, xmm4\n"
            "subps xmm2, xmm4\n"
            "subps xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}
double sse_f32mul_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov eax, 0x3f800000\n"
        "movd xmm4, eax\n"
        "shufps xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mulps xmm0, xmm4\n"
            "mulps xmm1, xmm4\n"
            "mulps xmm2, xmm4\n"
            "mulps xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}
double sse_f32div_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorps xmm0, xmm0\n"
        "xorps xmm1, xmm1\n"
        "xorps xmm2, xmm2\n"
        "xorps xmm3, xmm3\n"
        "mov eax, 0x3f800000\n"
        "movd xmm4, eax\n"
        "shufps xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "divps xmm0, xmm4\n"
            "divps xmm1, xmm4\n"
            "divps xmm2, xmm4\n"
            "divps xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}

double sse2_f64add_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 3; //div by 8
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        "shufpd xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "addpd xmm0, xmm4\n"
            "addpd xmm1, xmm4\n"
            "addpd xmm2, xmm4\n"
            "addpd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 3) / ((after-prev)/tsc_freq_hz);
}
double sse2_f64sub_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 3; //div by 8
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        "shufpd xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "subpd xmm0, xmm4\n"
            "subpd xmm1, xmm4\n"
            "subpd xmm2, xmm4\n"
            "subpd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 3) / ((after-prev)/tsc_freq_hz);
}
double sse2_f64mul_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 3; //div by 8
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        "shufpd xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mulpd xmm0, xmm4\n"
            "mulpd xmm1, xmm4\n"
            "mulpd xmm2, xmm4\n"
            "mulpd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 3) / ((after-prev)/tsc_freq_hz);
}
double sse2_f64div_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 3; //div by 8
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "xorpd xmm0, xmm0\n"
        "xorpd xmm1, xmm1\n"
        "xorpd xmm2, xmm2\n"
        "xorpd xmm3, xmm3\n"
        "mov rax, 0x3ff0000000000000\n"
        "movq xmm4, rax\n"
        "shufpd xmm4, xmm4, 0\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "divpd xmm0, xmm4\n"
            "divpd xmm1, xmm4\n"
            "divpd xmm2, xmm4\n"
            "divpd xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();
    return (ops << 3) / ((after-prev)/tsc_freq_hz);
}



double avx_f32add_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, 0x3f800000\n"
        "vmovd xmm4, eax\n"
        "vshufps xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovaps ymm0, ymm4\n"
        "vmovaps ymm1, ymm4\n"
        "vmovaps ymm2, ymm4\n"
        "vmovaps ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vaddps ymm0, ymm0, ymm4\n"
            "vaddps ymm1, ymm1, ymm4\n"
            "vaddps ymm2, ymm2, ymm4\n"
            "vaddps ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}
double avx_f32sub_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, 0x3f800000\n"
        "vmovd xmm4, eax\n"
        "vshufps xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovaps ymm0, ymm4\n"
        "vmovaps ymm1, ymm4\n"
        "vmovaps ymm2, ymm4\n"
        "vmovaps ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vsubps ymm0, ymm0, ymm4\n"
            "vsubps ymm1, ymm1, ymm4\n"
            "vsubps ymm2, ymm2, ymm4\n"
            "vsubps ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}
double avx_f32mul_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, 0x3f800000\n"
        "vmovd xmm4, eax\n"
        "vshufps xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovaps ymm0, ymm4\n"
        "vmovaps ymm1, ymm4\n"
        "vmovaps ymm2, ymm4\n"
        "vmovaps ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vmulps ymm0, ymm0, ymm4\n"
            "vmulps ymm1, ymm1, ymm4\n"
            "vmulps ymm2, ymm2, ymm4\n"
            "vmulps ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}
double avx_f32div_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, 0x3f800000\n"
        "vmovd xmm4, eax\n"
        "vshufps xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovaps ymm0, ymm4\n"
        "vmovaps ymm1, ymm4\n"
        "vmovaps ymm2, ymm4\n"
        "vmovaps ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vdivps ymm0, ymm0, ymm4\n"
            "vdivps ymm1, ymm1, ymm4\n"
            "vdivps ymm2, ymm2, ymm4\n"
            "vdivps ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}

double avx_f64add_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x3ff0000000000000\n"
        "vmovq xmm4, rax\n"
        "vshufpd xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovapd ymm0, ymm4\n"
        "vmovapd ymm1, ymm4\n"
        "vmovapd ymm2, ymm4\n"
        "vmovapd ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vaddpd ymm0, ymm0, ymm4\n"
            "vaddpd ymm1, ymm1, ymm4\n"
            "vaddpd ymm2, ymm2, ymm4\n"
            "vaddpd ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}
double avx_f64sub_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x3ff0000000000000\n"
        "vmovq xmm4, rax\n"
        "vshufpd xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovapd ymm0, ymm4\n"
        "vmovapd ymm1, ymm4\n"
        "vmovapd ymm2, ymm4\n"
        "vmovapd ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vsubpd ymm0, ymm0, ymm4\n"
            "vsubpd ymm1, ymm1, ymm4\n"
            "vsubpd ymm2, ymm2, ymm4\n"
            "vsubpd ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}
double avx_f64mul_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x3ff0000000000000\n"
        "vmovq xmm4, rax\n"
        "vshufpd xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovapd ymm0, ymm4\n"
        "vmovapd ymm1, ymm4\n"
        "vmovapd ymm2, ymm4\n"
        "vmovapd ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vmulpd ymm0, ymm0, ymm4\n"
            "vmulpd ymm1, ymm1, ymm4\n"
            "vmulpd ymm2, ymm2, ymm4\n"
            "vmulpd ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}
double avx_f64div_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 4; //div by 16
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x3ff0000000000000\n"
        "vmovq xmm4, rax\n"
        "vshufpd xmm4, xmm4, xmm4, 0\n"
        "vinsertf128 ymm4, ymm4, xmm4, 1\n"
        "vmovapd ymm0, ymm4\n"
        "vmovapd ymm1, ymm4\n"
        "vmovapd ymm2, ymm4\n"
        "vmovapd ymm3, ymm4\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vdivpd ymm0, ymm0, ymm4\n"
            "vdivpd ymm1, ymm1, ymm4\n"
            "vdivpd ymm2, ymm2, ymm4\n"
            "vdivpd ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();

    __asm__ volatile("vzeroupper");

    return (ops << 4) / ((after-prev)/tsc_freq_hz);
}



double avx2_intadd_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, 3\n"
        "vmovd xmm4, eax\n"
        "vpbroadcastd ymm4, xmm4\n"
        "vpxor ymm0, ymm0, ymm0\n"
        "vpxor ymm1, ymm1, ymm1\n"
        "vpxor ymm2, ymm2, ymm2\n"
        "vpxor ymm3, ymm3, ymm3\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vpaddd ymm0, ymm0, ymm4\n"
            "vpaddd ymm1, ymm1, ymm4\n"
            "vpaddd ymm2, ymm2, ymm4\n"
            "vpaddd ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}
double avx2_intsub_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, 3\n"
        "vmovd xmm4, eax\n"
        "vpbroadcastd ymm4, xmm4\n"
        "vpxor ymm0, ymm0, ymm0\n"
        "vpxor ymm1, ymm1, ymm1\n"
        "vpxor ymm2, ymm2, ymm2\n"
        "vpxor ymm3, ymm3, ymm3\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vpsubd ymm0, ymm0, ymm4\n"
            "vpsubd ymm1, ymm1, ymm4\n"
            "vpsubd ymm2, ymm2, ymm4\n"
            "vpsubd ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}
double avx2_intmul_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, 3\n"
        "vmovd xmm4, eax\n"
        "vpbroadcastd ymm4, xmm4\n"
        "vpcmpeqd ymm0, ymm0, ymm0\n"
        "vpcmpeqd ymm1, ymm1, ymm1\n"
        "vpcmpeqd ymm2, ymm2, ymm2\n"
        "vpcmpeqd ymm3, ymm3, ymm3\n"
        ".att_syntax\n"
        :
        :
        : "rax", "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vpmulld ymm0, ymm0, ymm4\n"
            "vpmulld ymm1, ymm1, ymm4\n"
            "vpmulld ymm2, ymm2, ymm4\n"
            "vpmulld ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}
double avx2_intlogic_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 5; //div by 32
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "vpcmpeqd ymm4, ymm4, ymm4\n"

        "vpxor ymm0, ymm0, ymm0\n"
        "vpxor ymm1, ymm1, ymm1\n"
        "vpxor ymm2, ymm2, ymm2\n"
        "vpxor ymm3, ymm3, ymm3\n"
        ".att_syntax\n"
        :
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "ymm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "vpxor ymm0, ymm0, ymm4\n"
            "vpxor ymm1, ymm1, ymm4\n"
            "vpxor ymm2, ymm2, ymm4\n"
            "vpxor ymm3, ymm3, ymm4\n"
            ".att_syntax\n"
            :
            :
            : "ymm0", "ymm1", "ymm2", "ymm3"
        );
    }
    uint64_t after = rdtsc();
    __asm__ volatile("vzeroupper");

    return (ops << 5) / ((after-prev)/tsc_freq_hz);
}

//AES
double aes_enc_benchmark(uint64_t ops, double tsc_freq_hz){
    ops >>= 2; //div by 4
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "pxor xmm0, xmm0\n"
        "pxor xmm1, xmm1\n"
        "pxor xmm2, xmm2\n"
        "pxor xmm3, xmm3\n"
        "pxor xmm4, xmm4\n"
        ".att_syntax\n"
        :
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "xmm4"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "aesenc xmm0, xmm4\n"
            "aesenc xmm1, xmm4\n"
            "aesenc xmm2, xmm4\n"
            "aesenc xmm3, xmm4\n"
            ".att_syntax\n"
            :
            :
            : "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
    uint64_t after = rdtsc();

    return (ops << 2) / ((after-prev)/tsc_freq_hz);
}



//memory benchmarks, returns in B/s
double mread_benchmark(uint64_t bufsize, double tsc_freq_hz){
    uint64_t* buf = malloc(bufsize);

    uint64_t target = bufsize/sizeof(uint64_t);

    for(uint64_t i=0;i<target;++i){
        buf[i] = i;
    }
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<target;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mov rax, qword ptr [%0]\n"
            ".att_syntax\n"
            :
            : "r"(&buf[i])
            : "rax"
        );
    }
    uint64_t after = rdtsc();

    free(buf);
    return (target * sizeof(uint64_t)) / ((after-prev)/tsc_freq_hz);
}
double mwrite_benchmark(uint64_t bufsize, double tsc_freq_hz){
    uint64_t* buf = malloc(bufsize);

    uint64_t target = bufsize/sizeof(uint64_t);

    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0xdede6767aebec999\n"
        ".att_syntax\n"
        :
        :
        : "rax"
    );
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<target;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mov qword ptr [%0], rax\n"
            ".att_syntax\n"
            :
            : "r"(&buf[i])
            : "rax", "memory"
        );
    }
    uint64_t after = rdtsc();

    free(buf);
    return (target * sizeof(uint64_t)) / ((after-prev)/tsc_freq_hz);
}
double mcopy_benchmark(uint64_t bufsize, double tsc_freq_hz){
    uint64_t* buf = malloc(bufsize);
    uint64_t* buf2 = malloc(bufsize);

    uint64_t target = bufsize/sizeof(uint64_t);

    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<target;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mov rax, qword ptr [%0]\n"
            "mov qword ptr [%1], rax\n"
            ".att_syntax\n"
            :
            : "r"(&buf[i]), "r"(&buf2[i])
            : "rax", "memory"
        );
    }
    uint64_t after = rdtsc();

    free(buf);
    free(buf2);
    return (target * sizeof(uint64_t)) / ((after-prev)/tsc_freq_hz);
}

void benchmark_run(){
    clrscr();
    //result struct for saving(or not saving) results
    benchmark_result result = {0};

    EFI_TIME timestamp = getTime();

    //SETTINGS for benchmarks
    //ms
    uint64_t tsc_test_ms = 2000;
    uint64_t stress_ms = 10000;

    //millions
    uint64_t addsub_ops = 1000;
    uint64_t muldiv_ops = 1000;
    uint64_t logic_ops = 1000;
    uint64_t faddsub_ops = 1000;
    uint64_t fmuldiv_ops = 1000;
    uint64_t aes_ops = 100;

    //MB
    uint64_t mem_bufsize = 512;

    addsub_ops *= 1000000;
    muldiv_ops *= 1000000;
    logic_ops *= 1000000;
    faddsub_ops *= 1000000;
    fmuldiv_ops *= 1000000;
    aes_ops *= 1000000;
    mem_bufsize *= 1000000;

    bool has_sse, has_sse2, has_avx, has_avx2, has_usable_avx, has_aes;
    char model[49];

    wchar_t* yes = L"Supported";
    wchar_t* no = L"Unsupported";
    
    has_sse = cpu_has_sse();
    result.has_sse = has_sse;
    has_sse2 = cpu_has_sse2();
    result.has_sse2 = has_sse2;
    has_avx = cpu_has_avx();
    result.has_avx = has_avx;
    has_avx2 = cpu_has_avx2();
    result.has_avx2 = has_avx2;
    has_usable_avx = cpu_avx_usable();
    result.avx_usable = has_usable_avx;
    has_aes = cpu_has_aes();
    result.has_aes = has_aes;

    cpu_get_model(model);
    for(int i=0;i<48;i++){
        result.model[i] = model[i];
    }

    Print(L"CPU Model: %a\r\n", model);
    Print(L"CPU Features:\r\n");
    Print(L"SSE: %s\r\n", has_sse ? yes : no);
    Print(L"SSE2: %s\r\n", has_sse2 ? yes : no);
    Print(L"AVX: %s, %s\r\n", has_avx ? yes : no, has_usable_avx ? L"Enabled" : L"Disabled");
    Print(L"AVX2: %s, %s\r\n", has_avx2 ? yes : no, has_usable_avx ? L"Enabled" : L"Disabled");
    Print(L"AES: %s\r\n", has_aes ? yes : no);

    Print(L"\r\nPress any key to continue...\r\n");
    get_key();
    clrscr();

    uint64_t t_before = rdtsc();

    Print(L"TSC Frequency: ");
    double tsc_f = measure_tsc_freq(tsc_test_ms);
    result.tsc_frequency = tsc_f;
    Print(L"%f MHz\r\n", tsc_f/1000000.0);

    double run1, run2, run3, avg;
    uint8_t cursor_x = 39;
    uint8_t cursor_y = 1;

    Print(L"Stressing CPU... ");
    stress_cpu(stress_ms, tsc_f);

    set_cursor_pos(0, 1);
    Print(L"                   ");
    set_cursor_pos(0, 1);

    //ALU 
    Print(L"ALU ADD: ");
    intadd_benchmark(addsub_ops/10, tsc_f);
    run1 = intadd_benchmark(addsub_ops, tsc_f);
    run2 = intadd_benchmark(addsub_ops, tsc_f);
    run3 = intadd_benchmark(addsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    result.int_add = avg;
    Print(L"%f MOP/s", avg/1000000.0);

    set_cursor_pos(cursor_x, cursor_y);
    Print(L"ALU SUB: ");
    intsub_benchmark(addsub_ops/10, tsc_f);
    run1 = intsub_benchmark(addsub_ops, tsc_f);
    run2 = intsub_benchmark(addsub_ops, tsc_f);
    run3 = intsub_benchmark(addsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    result.int_sub = avg;
    Print(L"%f MOP/s\r\n", avg/1000000.0);
    cursor_y++;

    Print(L"ALU MUL: ");
    intmul_benchmark(muldiv_ops/10, tsc_f);
    run1 = intmul_benchmark(muldiv_ops, tsc_f);
    run2 = intmul_benchmark(muldiv_ops, tsc_f);
    run3 = intmul_benchmark(muldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    result.int_mul = avg;
    Print(L"%f MOP/s", avg/1000000.0);

    set_cursor_pos(cursor_x, cursor_y);
    Print(L"ALU DIV: ");
    intdiv_benchmark(muldiv_ops/10, tsc_f);
    run1 = intdiv_benchmark(muldiv_ops, tsc_f);
    run2 = intdiv_benchmark(muldiv_ops, tsc_f);
    run3 = intdiv_benchmark(muldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    result.int_div = avg;
    Print(L"%f MOP/s\r\n", avg/1000000.0);
    cursor_y++;

    Print(L"ALU LOGIC: ");
    bit_benchmark(logic_ops/10, tsc_f);
    run1 = bit_benchmark(logic_ops, tsc_f);
    run2 = bit_benchmark(logic_ops, tsc_f);
    run3 = bit_benchmark(logic_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    result.int_logic = avg;
    Print(L"%f MOP/s\r\n", avg/1000000.0);
    cursor_y++;

    //ALL FLOAT TESTS
    if(has_sse){
        //FPU scalar floats, requires sse
        Print(L"FPU FLOAT ADD: ");
        f32add_benchmark(faddsub_ops/10, tsc_f);
        run1 = f32add_benchmark(faddsub_ops, tsc_f);
        run2 = f32add_benchmark(faddsub_ops, tsc_f);
        run3 = f32add_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f32_add = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"FPU FLOAT SUB: ");
        f32sub_benchmark(faddsub_ops/10, tsc_f);
        run1 = f32sub_benchmark(faddsub_ops, tsc_f);
        run2 = f32sub_benchmark(faddsub_ops, tsc_f);
        run3 = f32sub_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f32_sub = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"FPU FLOAT MUL: ");
        f32mul_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = f32mul_benchmark(fmuldiv_ops, tsc_f);
        run2 = f32mul_benchmark(fmuldiv_ops, tsc_f);
        run3 = f32mul_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f32_mul = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"FPU FLOAT DIV: ");
        f32div_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = f32div_benchmark(fmuldiv_ops, tsc_f);
        run2 = f32div_benchmark(fmuldiv_ops, tsc_f);
        run3 = f32div_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f32_div = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;


        //FPU packed floats SIMD, requires sse
        Print(L"SIMD FLOAT ADD: ");
        sse_f32add_benchmark(faddsub_ops/10, tsc_f);
        run1 = sse_f32add_benchmark(faddsub_ops, tsc_f);
        run2 = sse_f32add_benchmark(faddsub_ops, tsc_f);
        run3 = sse_f32add_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p32_add = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"SIMD FLOAT SUB: ");
        sse_f32sub_benchmark(faddsub_ops/10, tsc_f);
        run1 = sse_f32sub_benchmark(faddsub_ops, tsc_f);
        run2 = sse_f32sub_benchmark(faddsub_ops, tsc_f);
        run3 = sse_f32sub_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p32_sub = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"SIMD FLOAT MUL: ");
        sse_f32mul_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = sse_f32mul_benchmark(fmuldiv_ops, tsc_f);
        run2 = sse_f32mul_benchmark(fmuldiv_ops, tsc_f);
        run3 = sse_f32mul_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p32_mul = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"SIMD FLOAT DIV: ");
        sse_f32div_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = sse_f32div_benchmark(fmuldiv_ops, tsc_f);
        run2 = sse_f32div_benchmark(fmuldiv_ops, tsc_f);
        run3 = sse_f32div_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p32_div = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;
    }
    else{
        Print(L"SSE Unsupported, skipping FPU FLOAT tests\r\n");
        cursor_y++;
    }
    if(has_usable_avx){
        //AVX packed floats SIMD, requires avx
        Print(L"AVX FLOAT ADD: ");
        avx_f32add_benchmark(faddsub_ops/10, tsc_f);
        run1 = avx_f32add_benchmark(faddsub_ops, tsc_f);
        run2 = avx_f32add_benchmark(faddsub_ops, tsc_f);
        run3 = avx_f32add_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a32_add = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"AVX FLOAT SUB: ");
        avx_f32sub_benchmark(faddsub_ops/10, tsc_f);
        run1 = avx_f32sub_benchmark(faddsub_ops, tsc_f);
        run2 = avx_f32sub_benchmark(faddsub_ops, tsc_f);
        run3 = avx_f32sub_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a32_sub = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"AVX FLOAT MUL: ");
        avx_f32mul_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = avx_f32mul_benchmark(fmuldiv_ops, tsc_f);
        run2 = avx_f32mul_benchmark(fmuldiv_ops, tsc_f);
        run3 = avx_f32mul_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a32_mul = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"AVX FLOAT DIV: ");
        avx_f32div_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = avx_f32div_benchmark(fmuldiv_ops, tsc_f);
        run2 = avx_f32div_benchmark(fmuldiv_ops, tsc_f);
        run3 = avx_f32div_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a32_div = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;
    }
    else{
        Print(L"AVX disabled, skipping AVX FLOAT tests\r\n");
        cursor_y++;
    }


    //ALL DOUBLE TESTS
    if(has_sse2){
        //FPU scalar doubles, requires sse2
        Print(L"FPU DOUBLE ADD: ");
        f64add_benchmark(faddsub_ops/10, tsc_f);
        run1 = f64add_benchmark(faddsub_ops, tsc_f);
        run2 = f64add_benchmark(faddsub_ops, tsc_f);
        run3 = f64add_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f64_add = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"FPU DOUBLE SUB: ");
        f64sub_benchmark(faddsub_ops/10, tsc_f);
        run1 = f64sub_benchmark(faddsub_ops, tsc_f);
        run2 = f64sub_benchmark(faddsub_ops, tsc_f);
        run3 = f64sub_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f64_sub = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"FPU DOUBLE MUL: ");
        f64mul_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = f64mul_benchmark(fmuldiv_ops, tsc_f);
        run2 = f64mul_benchmark(fmuldiv_ops, tsc_f);
        run3 = f64mul_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f64_mul = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"FPU DOUBLE DIV: ");
        f64div_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = f64div_benchmark(fmuldiv_ops, tsc_f);
        run2 = f64div_benchmark(fmuldiv_ops, tsc_f);
        run3 = f64div_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.f64_div = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;


        //FPU packed doubles SIMD, requires sse2
        Print(L"SIMD DOUBLE ADD: ");
        sse2_f64add_benchmark(faddsub_ops/10, tsc_f);
        run1 = sse2_f64add_benchmark(faddsub_ops, tsc_f);
        run2 = sse2_f64add_benchmark(faddsub_ops, tsc_f);
        run3 = sse2_f64add_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p64_add = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"SIMD DOUBLE SUB: ");
        sse2_f64sub_benchmark(faddsub_ops/10, tsc_f);
        run1 = sse2_f64sub_benchmark(faddsub_ops, tsc_f);
        run2 = sse2_f64sub_benchmark(faddsub_ops, tsc_f);
        run3 = sse2_f64sub_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p64_sub = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"SIMD DOUBLE MUL: ");
        sse2_f64mul_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = sse2_f64mul_benchmark(fmuldiv_ops, tsc_f);
        run2 = sse2_f64mul_benchmark(fmuldiv_ops, tsc_f);
        run3 = sse2_f64mul_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p64_mul = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"SIMD DOUBLE DIV: ");
        sse2_f64div_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = sse2_f64div_benchmark(fmuldiv_ops, tsc_f);
        run2 = sse2_f64div_benchmark(fmuldiv_ops, tsc_f);
        run3 = sse2_f64div_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.p64_div = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;
    }
    else{
        Print(L"SSE2 Unsupported, skipping FPU DOUBLE tests\r\n");
        cursor_y++;
    }
    if(has_usable_avx){
        //AVX packed doubles SIMD, requires avx
        Print(L"AVX DOUBLE ADD: ");
        avx_f64add_benchmark(faddsub_ops/10, tsc_f);
        run1 = avx_f64add_benchmark(faddsub_ops, tsc_f);
        run2 = avx_f64add_benchmark(faddsub_ops, tsc_f);
        run3 = avx_f64add_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a64_add = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"AVX DOUBLE SUB: ");
        avx_f64sub_benchmark(faddsub_ops/10, tsc_f);
        run1 = avx_f64sub_benchmark(faddsub_ops, tsc_f);
        run2 = avx_f64sub_benchmark(faddsub_ops, tsc_f);
        run3 = avx_f64sub_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a64_sub = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"AVX DOUBLE MUL: ");
        avx_f64mul_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = avx_f64mul_benchmark(fmuldiv_ops, tsc_f);
        run2 = avx_f64mul_benchmark(fmuldiv_ops, tsc_f);
        run3 = avx_f64mul_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a64_mul = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"AVX DOUBLE DIV: ");
        avx_f64div_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = avx_f64div_benchmark(fmuldiv_ops, tsc_f);
        run2 = avx_f64div_benchmark(fmuldiv_ops, tsc_f);
        run3 = avx_f64div_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.a64_div = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;
    }
    else{
        Print(L"AVX disabled, skipping AVX DOUBLE tests\r\n");
        cursor_y++;
    }

    //AVX int
    if(has_usable_avx){
        //AVX int SIMD, requires avx
        Print(L"AVX INT ADD: ");
        avx2_intadd_benchmark(faddsub_ops/10, tsc_f);
        run1 = avx2_intadd_benchmark(faddsub_ops, tsc_f);
        run2 = avx2_intadd_benchmark(faddsub_ops, tsc_f);
        run3 = avx2_intadd_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.aint_add = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"AVX INT SUB: ");
        avx2_intsub_benchmark(faddsub_ops/10, tsc_f);
        run1 = avx2_intsub_benchmark(faddsub_ops, tsc_f);
        run2 = avx2_intsub_benchmark(faddsub_ops, tsc_f);
        run3 = avx2_intsub_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.aint_sub = avg;
        Print(L"%f MOP/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"AVX INT MUL: ");
        avx2_intmul_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = avx2_intmul_benchmark(fmuldiv_ops, tsc_f);
        run2 = avx2_intmul_benchmark(fmuldiv_ops, tsc_f);
        run3 = avx2_intmul_benchmark(fmuldiv_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.aint_mul = avg;
        Print(L"%f MOP/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"AVX INT LOGIC: ");
        avx2_intlogic_benchmark(fmuldiv_ops/10, tsc_f);
        run1 = avx2_intlogic_benchmark(faddsub_ops, tsc_f);
        run2 = avx2_intlogic_benchmark(faddsub_ops, tsc_f);
        run3 = avx2_intlogic_benchmark(faddsub_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.aint_logic = avg;
        Print(L"%f MOP/s", avg/1000000.0);
        cursor_y++;
    }
    else{
        Print(L"AVX disabled, skipping AVX INT tests\r\n");
        cursor_y++;
    }

    if(has_aes){
        //AES-NI benchmark, requires aes-ni
        Print(L"AES-NI AESENC: ");
        aes_enc_benchmark(aes_ops/10, tsc_f);
        run1 = aes_enc_benchmark(aes_ops, tsc_f);
        run2 = aes_enc_benchmark(aes_ops, tsc_f);
        run3 = aes_enc_benchmark(aes_ops, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.aes = avg;
        Print(L"%f M round/s\r\n", avg/1000000.0);
        cursor_y++;
    }
    else{
        Print(L"AES-NI Unsupported, skipping AES-NI tests\r\n");
        cursor_y++;
    }

    {
        //MEMORY
        Print(L"MEMORY READ: ");
        mread_benchmark(mem_bufsize/10, tsc_f);
        run1 = mread_benchmark(mem_bufsize, tsc_f);
        run2 = mread_benchmark(mem_bufsize, tsc_f);
        run3 = mread_benchmark(mem_bufsize, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.mem_read = avg;
        Print(L"%f MB/s", avg/1000000.0);

        set_cursor_pos(cursor_x, cursor_y);
        Print(L"MEMORY WRITE: ");
        mwrite_benchmark(mem_bufsize/10, tsc_f);
        run1 = mwrite_benchmark(mem_bufsize, tsc_f);
        run2 = mwrite_benchmark(mem_bufsize, tsc_f);
        run3 = mwrite_benchmark(mem_bufsize, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.mem_write = avg;
        Print(L"%f MB/s\r\n", avg/1000000.0);
        cursor_y++;

        Print(L"MEMORY COPY: ");
        mcopy_benchmark(mem_bufsize/10, tsc_f);
        run1 = mcopy_benchmark(mem_bufsize, tsc_f);
        run2 = mcopy_benchmark(mem_bufsize, tsc_f);
        run3 = mcopy_benchmark(mem_bufsize, tsc_f);
        avg = (run1+run2+run3)/3.0;
        result.mem_copy = avg;
        Print(L"%f MB/s\r\n", avg/1000000.0);
        cursor_y++;
    }
    uint64_t t_after = rdtsc();
    Print(L"Took %f seconds\r\n", (float)((t_after-t_before) / tsc_f));
    result.time_taken = (uint64_t)((t_after-t_before) / tsc_f);

    char* re = generate_results(timestamp, &result);

    Print(L"\r\nPress any key to continue...\r\nGenerated results: %a", re);
    get_key();
}

//returns a malloc'd string
char* generate_results(EFI_TIME timestamp, benchmark_result* result){
    //time
    char* timestr = genTimeStr(timestamp);

    uint64_t buf_size = 2048;
    char* buf = malloc(2048);
    uint64_t buf_i = 0; //points to next write char

    //construct results
    uint64_t len = StrCopy(timestr, buf);
    buf_i += len;

    buf[buf_i] = ',';
    buf_i++;

    len = StrCopy(result->model, &buf[buf_i]); buf_i += len;

    buf[buf_i++] = ',';

    //6 bools packed, lowest bit is has_aes, highest is has_sse
    //  construct
    uint8_t flags = 
    (result->has_sse << 5) |
    (result->has_sse2 << 4) |
    (result->has_avx << 3) |
    (result->has_avx2 << 2) |
    (result->avx_usable << 1) |
    (result->has_aes);
    //  copy
    sprint_fixed_int(&buf[buf_i], (uint64_t)flags); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    //do rest of the params
    sprint_fixed_double(&buf[buf_i], result->tsc_frequency); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->int_add); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->int_sub); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->int_mul); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->int_div); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->int_logic); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->f32_add); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->f32_sub); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->f32_mul); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->f32_div); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->p32_add); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->p32_sub); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->p32_mul); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->p32_div); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->a32_add); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->a32_sub); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->a32_mul); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->a32_div); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->f64_add); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->f64_sub); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->f64_mul); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->f64_div); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->p64_add); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->p64_sub); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->p64_mul); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->p64_div); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->a64_add); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->a64_sub); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->a64_mul); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->a64_div); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->aint_add);   buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->aint_sub);   buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->aint_mul);   buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->aint_logic); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->aes); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_double(&buf[buf_i], result->mem_read);  buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->mem_write); buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';
    sprint_fixed_double(&buf[buf_i], result->mem_copy);  buf_i += SPRINT_FIXED_WIDTH; buf[buf_i++] = ',';

    sprint_fixed_int(&buf[buf_i], result->time_taken); buf_i += SPRINT_FIXED_WIDTH;
    
    buf[buf_i] = '\r'; buf_i++;
    buf[buf_i] = '\n'; buf_i++;
    buf[buf_i] = '\0';

    return buf;
}


