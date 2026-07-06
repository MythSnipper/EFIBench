#include <benchmark.h>

//returns tsc frequency in Hz after testing for [test_duration_ms] ms
double measure_tsc_freq(double test_duration_ms){
    uint64_t prev = rdtsc();
    sleep(test_duration_ms);
    uint64_t after = rdtsc();
    return ((after-prev) * 1000.0)/test_duration_ms;
}

//integer operation benchmark, returns in op/sec
double int_benchmark(uint64_t ops, double tsc_freq_hz){
    volatile uint64_t result = 1; //prevent optimizations

    uint64_t prev = rdtsc();

    for(uint64_t i=0;i<ops;i++){
        result = result * 28375291113 + 1895918302602;
    }

    uint64_t after = rdtsc();

    return ops / ((after-prev)/tsc_freq_hz);
}


void benchmark_run(){
    uint64_t prev = rdtsc();

    double tsc_f = measure_tsc_freq(1000);
    Print(L"TSC Frequency: %lf MHz\r\n", tsc_f / 1000000.0);
    int_benchmark(10000000, tsc_f);

    double run1 = int_benchmark(1000000000, tsc_f);
    double run2 = int_benchmark(1000000000, tsc_f);
    double run3 = int_benchmark(1000000000, tsc_f);

    uint64_t after = rdtsc();

    Print(L"Int: %lf op/s\r\n", run1);
    Print(L"Int: %lf op/s\r\n", run2);
    Print(L"Int: %lf op/s\r\n", run3);
    Print(L"Int avg: %lf op/s\r\n", (run1+run2+run3)/3.0);

    Print(L"Done!\r\n");
    Print(L"Took %d seconds");

    get_key();
}




