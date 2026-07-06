#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <utils.h>
#include <cpu.h>
#include <keyboard.h>

double measure_tsc_freq(double test_duration_ms);
double intadd_benchmark(uint64_t ops, double tsc_freq_hz);
double intsub_benchmark(uint64_t ops, double tsc_freq_hz);
double intmul_benchmark(uint64_t ops, double tsc_freq_hz);
double intdiv_benchmark(uint64_t ops, double tsc_freq_hz);

void benchmark_run();


#endif