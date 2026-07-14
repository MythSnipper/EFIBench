#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <utils.h>
#include <cpu.h>
#include <timer.h>
#include <keyboard.h>
#include <mem.h>
#include <filesystem.h>
#include <ui.h>

#define RUN_BENCH(label, ops, bench_func, result_member, unit, newline)\
    do {\
        double _run1, _run2, _run3, _avg;\
        Print(L"%s: ", (label));\
        (void)(bench_func)((ops) / 10, tsc_f);\
        _run1 = (bench_func)((ops), tsc_f);\
        _run2 = (bench_func)((ops), tsc_f);\
        _run3 = (bench_func)((ops), tsc_f);\
        _avg = (_run1+_run2+_run3)/3;\
        result.result_member = _avg;\
        Print(\
            (newline) ? L"%f %s/s\r\n" : L"%f %s/s",\
            _avg / 1000000.0,\
            (unit)\
        );\
    } while (0)

#define BENCH_RESULTS_PATH L"\\EFIBench\\results.txt"


typedef struct benchmark_result{
    char model[49];
    bool has_sse, has_sse2, has_avx, has_avx2, avx_usable, has_aes;

    double tsc_frequency;
    double int_add, int_sub, int_mul, int_div, int_logic;

    double f32_add, f32_sub, f32_mul, f32_div; //float
    double p32_add, p32_sub, p32_mul, p32_div; //packed
    double a32_add, a32_sub, a32_mul, a32_div; //avx

    double f64_add, f64_sub, f64_mul, f64_div; //double
    double p64_add, p64_sub, p64_mul, p64_div; //packed
    double a64_add, a64_sub, a64_mul, a64_div; //avx
    double aint_add, aint_sub, aint_mul, aint_logic; //avx2

    double aes; //aes
    double mem_read, mem_write, mem_copy;

    uint64_t time_taken; //s
} benchmark_result;

#define BENCH_LABEL_LEN 32


typedef struct bench_file_entry{
    char label[BENCH_LABEL_LEN + 1];
    char time[20];
    benchmark_result result;
} bench_file_entry;

double measure_tsc_freq(double test_duration_ms);

void stress_cpu(uint64_t ms, double tsc_freq_hz);

void benchmark_run();

char* generate_results(char* label, EFI_TIME timestamp, benchmark_result* result);
char* parse_result_entry(char* buf, bench_file_entry* entry);
uint64_t parse_result_file(bench_file_entry** entries_out);


#endif