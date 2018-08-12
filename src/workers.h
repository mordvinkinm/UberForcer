#ifndef WORKERS_H
#define WORKERS_H

#include "config.h"

#define LAST_BRUTE_CHARS (2)

/**************************************************************************
 * Function:    single_brute
 *
 * Description: highlevel worker that bruteforces provided task in a
 *              single-threaded mode
 *
 * Inputs:      config_t* config
 *              Pointer to application config
 *
 *              task_t* initial_task
 *              Pointer to task needs to be bruteforced
 *
 * Returns:     none
 *
 *************************************************************************/
void single_brute(config_t *config, task_t *initial_task);

/**************************************************************************
 * Function:    multi_brute
 *
 * Description: highlevel worker that bruteforces provided task in a
 *              multi-threaded mode
 *
 * Inputs:      config_t* config
 *              Pointer to application config
 *
 *              task_t* initial_task
 *              Pointer to task needs to be bruteforced
 *
 * Returns:     none
 *
 *************************************************************************/
void multi_brute(config_t *config, task_t *initial_task);

/**************************************************************************
 * Function:    generate_tasks
 *
 * Description: function that generates thread tasks based on provided
 *              base task
 *
 * Inputs:      config_t* config
 *              Pointer to application config
 *
 *              task_t* initial_task
 *              Pointer a "base" task. Other tasks will be generated based
 *              on this "base" task.
 *
 * Returns:     none
 *
 *************************************************************************/
void generate_tasks(config_t *config, task_t *initial_task);

/**************************************************************************
 * Function:    bruteforce_task_job
 *
 * Description: One-thread job, that extracts tasks from the queue and
 *              bruteforces this task.
 *
 * Inputs:      void* arg [actual type is worker_args_t*]
 *              parameters passed to a thread worker
 *
 * Returns:     void*
 *
 *************************************************************************/
void* bruteforce_task_job(void *arg);

#endif /* WORKERS_H */