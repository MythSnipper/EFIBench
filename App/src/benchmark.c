#include <benchmark.h>

//returns tsc frequency in Hz after testing for [test_duration_ms] ms
double measure_tsc_freq(double test_duration_ms){
    uint64_t prev = rdtsc();
    sleep(test_duration_ms);
    uint64_t after = rdtsc();
    return ((after-prev) * 1000.0)/test_duration_ms;
}

//assembly functions to prevent optimizations
static inline void asm_add(){
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "add rax, 8782398\n"
        ".att_syntax\n"
        :
        :
        : "rax", "cc"
    );
}
static inline void asm_sub(){
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "sub rax, 8782398\n"
        ".att_syntax\n"
        :
        :
        : "rax", "cc"
    );
}
static inline void asm_imul(){
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "imul rax, 8782398\n"
        ".att_syntax\n"
        :
        :
        : "rax", "cc"
    );
}
static inline void asm_idiv(){
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov rax, 6879253423235\n" //smh idiv requiring RDX:RAX :skul:
        "cqo\n" //quadword to octaword smh
        "mov rsi, 6\n"
        "idiv rsi\n"
        ".att_syntax\n"
        :
        :
        : "rax", "rdx", "rsi", "cc"
    );
}

//integer operation benchmarks, returns in op/sec
double intadd_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        asm_add();
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double intsub_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        asm_sub();
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double intmul_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        asm_imul();
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}
double intdiv_benchmark(uint64_t ops, double tsc_freq_hz){
    uint64_t prev = rdtsc();
    for(uint64_t i=0;i<ops;++i){
        asm_idiv();
    }
    uint64_t after = rdtsc();
    return ops / ((after-prev)/tsc_freq_hz);
}

void benchmark_run(){
    double tsc_f = measure_tsc_freq(2000);
    Print(L"TSC Frequency: %f MHz\r\n", tsc_f / 1000000.0);

    uint64_t addsub_ops = 1000; //millions
    uint64_t muldiv_ops = 1000; //millions
    addsub_ops *= 1000000;
    muldiv_ops *= 1000000;

    double run1, run2, run3, avg;

    intadd_benchmark(addsub_ops/10, tsc_f);
    run1 = intadd_benchmark(addsub_ops, tsc_f);
    run2 = intadd_benchmark(addsub_ops, tsc_f);
    run3 = intadd_benchmark(addsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"Int add avg: %f Kop/s\r\n", avg/1000.0);

    intsub_benchmark(addsub_ops/10, tsc_f);
    run1 = intsub_benchmark(addsub_ops, tsc_f);
    run2 = intsub_benchmark(addsub_ops, tsc_f);
    run3 = intsub_benchmark(addsub_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"Int sub avg: %f Kop/s\r\n", avg/1000.0);

    intmul_benchmark(muldiv_ops/10, tsc_f);
    run1 = intmul_benchmark(muldiv_ops, tsc_f);
    run2 = intmul_benchmark(muldiv_ops, tsc_f);
    run3 = intmul_benchmark(muldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"Int mul avg: %f Kop/s\r\n", avg/1000.0);

    intdiv_benchmark(muldiv_ops/10, tsc_f);
    run1 = intdiv_benchmark(muldiv_ops, tsc_f);
    run2 = intdiv_benchmark(muldiv_ops, tsc_f);
    run3 = intdiv_benchmark(muldiv_ops, tsc_f);
    avg = (run1+run2+run3)/3.0;
    Print(L"Int div avg: %f Kop/s\r\n", avg/1000.0);


    get_key();
}




