#ifndef WORKERS_H
#define WORKERS_H

#include "config.h"

#define LAST_BRUTE_CHARS (2)

void generate_tasks_worker(config_t* config, task_t* initial_task);

void bruteforce_task_worker(config_t* config);

void* bruteforce_task_thread_job(void* arg);

#endif /* WORKERS_H */