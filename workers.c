#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "struct.h"
#include "workers.h"

void add_to_queue(config_t* config, task_t* task) {
  pthread_mutex_lock(&config->num_tasks_mutex);
  ++config->num_tasks;
  pthread_mutex_unlock(&config->num_tasks_mutex);
  queue_push(&config->queue, task);

  trace("Task added to the queue: [from: %d, to: %d, password: %s]\n", task->from, task->to, task->password);
}

void generate_tasks_worker(config_t* config, task_t* initial_task) {
  debug("Task generator started\n");

  queue_init(&config->queue);

  config->num_tasks = 0;

  for (int i = 0; i < initial_task->to; i++) {
    initial_task->password[i] = config->alphabet[0];
  }

  task_t task = {.from = initial_task->from + LAST_BRUTE_CHARS, .to = initial_task->to};
  strcpy(task.password, initial_task->password);

  config->brute_function(&task, config, add_to_queue);

  debug("Task generator finished execution\n");
}

void bruteforce_task_worker(config_t* config) {
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
}

void* bruteforce_task_thread_job(void* arg) {
  worker_args_t* args = arg;
  config_t* config = args->config;

  debug("Worker #%d started\n", args->thread_number);

  bruteforce_task_worker(config);

  debug("Client worker #%d finished\n", args->thread_number);

  pthread_exit(EXIT_SUCCESS);
}