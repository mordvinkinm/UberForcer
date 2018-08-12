#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "struct.h"
#include "workers.h"

/**************************************************************************
 * Function:    add_to_queue
 *
 * Description: adds a task to the queue
 *
 * Inputs:      config_t *config
 *              Pointer to application config
 *
 *              task_t *task
 *              Pointer to task that will be added to the queue
 *
 * Returns:     none
 *
 *************************************************************************/
void add_to_queue(config_t* config, task_t* task) {
  pthread_mutex_lock(&config->num_tasks_mutex);
  ++config->num_tasks;
  pthread_mutex_unlock(&config->num_tasks_mutex);
  queue_push(&config->queue, task);

  trace("Task added to the queue: [from: %d, to: %d, password: %s]\n", task->from, task->to, task->password);
}

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
 *              Pointer to "base" task:
 *              It might be different in case of one-machine mode
 *              (i.e. when we need to bruteforce the whole passwords set) and
 *              in master-worker mode (i.e. when partiaular client needs
 *              to bruteforce subset of passwords set)
 *
 * Returns:     none
 *
 *************************************************************************/
void generate_tasks(config_t* config, task_t* initial_task) {
  debug("Task generator started with parameters\n");

  for (int i = 0; i < initial_task->to; i++) {
    initial_task->password[i] = config->alphabet[0];
  }

  task_t task = {.from = initial_task->from + LAST_BRUTE_CHARS, .to = initial_task->to};
  strcpy(task.password, initial_task->password);

  config->brute_function(&task, config, add_to_queue);

  debug("Task generator finished execution\n");
}

/**************************************************************************
 * Function:    bruteforce_task_job
 *
 * Description: Job for one thread, that extracts tasks from the queue and
 *              bruteforces this task.
 *
 *              Thread-safe.
 *
 * Inputs:      void* arg [actual type is worker_args_t*]
 *              parameters passed to a thread worker - pointer to config
 *              and number / name of thread
 *
 * Returns:     void*
 *
 *************************************************************************/
void* bruteforce_task_job(void* arg) {
  // unwrap thread job arguments
  worker_args_t* args = arg;
  config_t* config = args->config;

  debug("Worker #%d started\n", args->thread_number);

  task_t task;
  for (;;) {
    queue_pop(&config->queue, &task);
    task.to = task.from;
    task.from = 0;

    config->brute_function(&task, config, config->check_function);

    pthread_mutex_lock(&config->num_tasks_mutex);
    --config->num_tasks;
    pthread_mutex_unlock(&config->num_tasks_mutex);

    if (config->num_tasks == 0)
      pthread_cond_signal(&config->num_tasks_cv);
  }

  debug("Client worker #%d finished\n", args->thread_number);

  pthread_exit(EXIT_SUCCESS);
}

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
void single_brute(config_t* config, task_t* initial_task) {
  debug("Started in single-thread mode: password %s, from: %d, to: %d\n", initial_task->password, initial_task->from, initial_task->to);

  config->brute_function(initial_task, config, config->check_function);
}

/**************************************************************************
 * Function:    multi_brute
 *
 * Description: highlevel worker that bruteforces provided task in a
 *              multi-threaded mode.
 *
 *              - Starts tasks generator, which produces tasks and puts them
 *              into the queue;
 *              - Starts N bruteforcing jobs, which retrieve tasks from the
 *              queue and bruteforce each task.
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
void multi_brute(config_t* config, task_t* initial_task) {
  debug("Started in multi-thread mode in %d threads: password %s, from: %d, to: %d\n", config->num_threads, initial_task->password, initial_task->from, initial_task->to);

  // init thread structures
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_cond_init(&config->num_tasks_cv, NULL);

  pthread_t threads[config->num_threads];
  worker_args_t args[config->num_threads];

  // init queue structures
  queue_init(&config->queue);
  config->num_tasks = 0;

  // create bruteforce task jobs
  for (int cpu = 0; cpu < config->num_threads; cpu++) {
    args[cpu].config = config;
    args[cpu].thread_number = cpu + 1;

    pthread_create(&threads[cpu], &attr, bruteforce_task_job, &args[cpu]);
  }

  // run task generator
  generate_tasks(config, initial_task);

  // Wait until completion of all tasks
  pthread_mutex_lock(&config->num_tasks_mutex);
  while (config->num_tasks > 0) {
    pthread_cond_wait(&config->num_tasks_cv, &config->num_tasks_mutex);
  }
  pthread_mutex_unlock(&config->num_tasks_mutex);
}