/**************************************************************************
 *   Common data structures, constants etc. related to app configuration
 *
 *   File    : config.h
 *   Author  : Mikhail Mordvinkin
 *   Date    : July 24, 2018
 *
 **************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"
#include "queue.h"
#include "struct.h"

// Defines constants for application run mode
typedef enum {
  // Displays help message
  APP_MODE_HELP,

  // Encrypts provided password using provided salt, thus generating hash
  APP_MODE_CRYPT,

  // Bruteforces provided hash
  APP_MODE_DECRYPT,

  // Calculate application performance
  APP_MODE_BENCHMARK,

  // Server, i.e. master mode
  APP_MODE_SERVER,

  // Client, i.e. worker mode
  APP_MODE_CLIENT
} app_mode_t;

// Defines bruteforce algorithm - either recursive or ietrative
typedef enum {
  // Iterative algorithm
  BF_ITER,

  // Recursive algorithm
  BF_REC,
} bf_mode_t;

typedef struct config_s {
  // Flexible function pointers
  void (*brute_function)(task_t*, struct config_s*, void (*check_handler)(struct config_s* config, task_t* task));
  void (*check_function)(struct config_s* config, task_t* task);

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

  // Input parameters (for bruteforce)
  char* hash;

  // Input parameters (for crypt)
  char* password;
  char* salt;

  // Network parameters
  char* host;
  unsigned short port;

  // Multi-threading locks
  pthread_mutex_t result_mutex;
  pthread_mutex_t num_tasks_mutex;
  pthread_cond_t num_tasks_cv;
} config_t;

typedef struct worker_args_s {
  config_t* config;
  int thread_number;  // todo: chagne to thread_name
} worker_args_t;

#endif /* CONFIG_H */