#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include "config.h"

void bruteforce_rec(task_t* task, config_t* config, void (*check_handler)(config_t* config, task_t* task));

void bruteforce_iter(task_t* task, config_t* config, void (*check_handler)(config_t* config, task_t* task));

#endif /* BRUTEFORCE_H */