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

double measure_tsc_freq(double test_duration_ms);

void stress_cpu(uint64_t ms, double tsc_freq_hz);

double intadd_benchmark(uint64_t ops, double tsc_freq_hz);
double intsub_benchmark(uint64_t ops, double tsc_freq_hz);
double intmul_benchmark(uint64_t ops, double tsc_freq_hz);
double intdiv_benchmark(uint64_t ops, double tsc_freq_hz);

double bit_benchmark(uint64_t ops, double tsc_freq_hz);

double fadd_benchmark(uint64_t ops, double tsc_freq_hz);
double fsub_benchmark(uint64_t ops, double tsc_freq_hz);
double fmul_benchmark(uint64_t ops, double tsc_freq_hz);
double fdiv_benchmark(uint64_t ops, double tsc_freq_hz);

void benchmark_run();


#endif