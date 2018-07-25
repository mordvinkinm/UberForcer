#ifndef CHECK_H
#define CHECK_H

#include "config.h"

// Thread-unsafe versions
void check_task(config_t* config, task_t* result);

void check_task_benchmark(config_t* config, task_t* result);

// Thread-safe versions
void check_task_r(config_t* config, task_t* result);

void check_task_benchmark_r(config_t* config, task_t* result);

#endif /* CHECK_H */