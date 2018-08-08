#ifndef WORKERS_H
#define WORKERS_H

#include "config.h"

#define LAST_BRUTE_CHARS (2)

void single_brute(config_t *config, task_t * initial_task);

void multi_brute(config_t *config, task_t * initial_task);

void generate_tasks_worker(config_t *config, task_t * initial_task);

#endif /* WORKERS_H */