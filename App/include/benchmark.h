#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

#include <utils.h>
#include <keyboard.h>

double measure_tsc_freq(double test_duration_ms);
double int_benchmark(uint64_t ops, double tsc_freq_hz);

void benchmark_run();


#endif