#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"
#include "struct.h"
#include "queue.h"

typedef enum { 
  APP_MODE_HELP, 
  APP_MODE_CRYPT,
  APP_MODE_DECRYPT,
  APP_MODE_BENCHMARK
} app_mode_t; 

typedef enum {
  BF_ITER,
  BF_REC,
} bf_mode_t;

typedef struct config_s {
  // Flexible function pointers
  void (*brute_function) (task_t *, struct config_s *, void (*check_handler) (struct config_s * config, task_t *task));
  void (*check_function) (struct config_s *config, task_t *task);

  // Application modes
  app_mode_t app_mode;
  bf_mode_t bruteforce_mode;

  // Working data
  result_t result;
  queue_t queue;
  int num_tasks;

  // Bruteforce parameters
  char* alphabet;
  int length;
  int num_threads;

  // Input parameters
  char* value;
  char* salt;

  // Multi-threading locks
  pthread_mutex_t result_mutex;
  pthread_mutex_t num_tasks_mutex;
  pthread_cond_t num_tasks_cv;
} config_t;
  
typedef struct worker_args_s {
  config_t* config;
  int thread_number;
} worker_args_t;

#endif /* CONFIG_H */