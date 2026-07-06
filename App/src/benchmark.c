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
double fadd_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x4014000000000000\n"
        "movq xmm0, rax\n"
        "mov rax, 0x4008000000000000\n"
        "movq xmm1, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "addsd xmm0, xmm1\n"
            ".att_syntax\n"
            :
            :
            : "xmm0"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double fsub_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x4014000000000000\n"
        "movq xmm0, rax\n"
        "mov rax, 0x4008000000000000\n"
        "movq xmm1, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "subsd xmm0, xmm1\n"
            ".att_syntax\n"
            :
            :
            : "xmm0"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double fmul_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x4014000000000000\n"
        "movq xmm0, rax\n"
        "mov rax, 0x4008000000000000\n"
        "movq xmm1, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "mulsd xmm0, xmm1\n"
            ".att_syntax\n"
            :
            :
            : "xmm0"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double fdiv_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 0x4014000000000000\n"
        "movq xmm0, rax\n"
        "mov rax, 0x4008000000000000\n"
        "movq xmm1, rax\n"
        ".att_syntax\n"
        :
        :
        : "rax", "xmm0", "xmm1"
    );
    for(uint64_t i=0;i<ops;++i){
        __asm__ volatile(
            ".intel_syntax noprefix\n"
            "divsd xmm0, xmm1\n"
            ".att_syntax\n"
            :
            :
            : "xmm0"
        );
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
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
    Print(L"TSC Frequency: ");
    double tsc_f = measure_tsc_freq(1000);
    Print(L"%f MHz\r\n\r\n", tsc_f / 1000000.0);

    //SETTINGS for benchmarks
    //ms
    uint64_t stress_ms = 10000;

    //millions
    uint64_t addsub_ops = 1000;
    uint64_t muldiv_ops = 100;
    uint64_t logic_ops = 1000;
    uint64_t faddsub_ops = 100;
    uint64_t fmuldiv_ops = 100;

    //MB
    uint64_t mem_bufsize = 512;

    addsub_ops *= 1000000;
    muldiv_ops *= 1000000;
    logic_ops *= 1000000;
    faddsub_ops *= 1000000;
    fmuldiv_ops *= 1000000;
    mem_bufsize *= 1000000;

    double run1, run2, run3, avg;


    Print(L"Stressing CPU... ");
    stress_cpu(stress_ms, tsc_f);
    Print(L"Done!\r\n\r\n");

    //ALU
    Print(L"ALU ADD: ");
    intadd_benchmark(addsub_ops/10, tsc_f);
    run1 = intadd_benchmark(addsub_ops, tsc_f);
    run2 = intadd_benchmark(addsub_ops, tsc_f);
    run3 = intadd_benchmark(addsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    Print(L"ALU SUB: ");
    intsub_benchmark(addsub_ops/10, tsc_f);
    run1 = intsub_benchmark(addsub_ops, tsc_f);
    run2 = intsub_benchmark(addsub_ops, tsc_f);
    run3 = intsub_benchmark(addsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    Print(L"ALU MUL: ");
    intmul_benchmark(muldiv_ops/10, tsc_f);
    run1 = intmul_benchmark(muldiv_ops, tsc_f);
    run2 = intmul_benchmark(muldiv_ops, tsc_f);
    run3 = intmul_benchmark(muldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    Print(L"ALU DIV: ");
    intdiv_benchmark(muldiv_ops/10, tsc_f);
    run1 = intdiv_benchmark(muldiv_ops, tsc_f);
    run2 = intdiv_benchmark(muldiv_ops, tsc_f);
    run3 = intdiv_benchmark(muldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    Print(L"ALU LOGIC: ");
    bit_benchmark(logic_ops/10, tsc_f);
    run1 = bit_benchmark(logic_ops, tsc_f);
    run2 = bit_benchmark(logic_ops, tsc_f);
    run3 = bit_benchmark(logic_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    //FPU
    Print(L"FPU ADD: ");
    fadd_benchmark(faddsub_ops/10, tsc_f);
    run1 = fadd_benchmark(faddsub_ops, tsc_f);
    run2 = fadd_benchmark(faddsub_ops, tsc_f);
    run3 = fadd_benchmark(faddsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    Print(L"FPU SUB: ");
    fsub_benchmark(faddsub_ops/10, tsc_f);
    run1 = fsub_benchmark(faddsub_ops, tsc_f);
    run2 = fsub_benchmark(faddsub_ops, tsc_f);
    run3 = fsub_benchmark(faddsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    Print(L"FPU MUL: ");
    fmul_benchmark(fmuldiv_ops/10, tsc_f);
    run1 = fmul_benchmark(fmuldiv_ops, tsc_f);
    run2 = fmul_benchmark(fmuldiv_ops, tsc_f);
    run3 = fmul_benchmark(fmuldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    Print(L"FPU DIV: ");
    fdiv_benchmark(fmuldiv_ops/10, tsc_f);
    run1 = fdiv_benchmark(fmuldiv_ops, tsc_f);
    run2 = fdiv_benchmark(fmuldiv_ops, tsc_f);
    run3 = fdiv_benchmark(fmuldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f Mop/s\r\n", avg/1000000.0);

    //MEMORY
    Print(L"MEMORY READ: ");
    mread_benchmark(mem_bufsize/10, tsc_f);
    run1 = mread_benchmark(mem_bufsize, tsc_f);
    run2 = mread_benchmark(mem_bufsize, tsc_f);
    run3 = mread_benchmark(mem_bufsize, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f MB/s\r\n", avg/1000000.0);

    Print(L"MEMORY WRITE: ");
    mwrite_benchmark(mem_bufsize/10, tsc_f);
    run1 = mwrite_benchmark(mem_bufsize, tsc_f);
    run2 = mwrite_benchmark(mem_bufsize, tsc_f);
    run3 = mwrite_benchmark(mem_bufsize, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f MB/s\r\n", avg/1000000.0);

    Print(L"MEMORY COPY: ");
    mcopy_benchmark(mem_bufsize/10, tsc_f);
    run1 = mcopy_benchmark(mem_bufsize, tsc_f);
    run2 = mcopy_benchmark(mem_bufsize, tsc_f);
    run3 = mcopy_benchmark(mem_bufsize, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"%f MB/s\r\n", avg/1000000.0);



    get_key();
    get_key();
    get_key();
}




