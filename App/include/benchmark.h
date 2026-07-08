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

void benchmark_run();


#endif