#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "bruteforce.h"
#include "parse_args.h"
#include "check.h"
#include "config.h"
#include "struct.h"
#include "crypt3.h"

#define LAST_BRUTE_CHARS (2)

void add_to_queue (config_t * config, task_t * task)
{
  pthread_mutex_lock(&config->num_tasks_mutex);
  ++config->num_tasks;
  pthread_mutex_unlock(&config->num_tasks_mutex);
  queue_push(&config -> queue, task);
}

void single_brute(config_t *config) {
  debug("Started in single-thread mode\n");

  task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  for (int i = 0; i < initial_task.to; i++) {
    initial_task.password[i] = config->alphabet[0];
  }

  config->brute_function(&initial_task, config, config->check_function);
}

void * client_worker (void * arg) {
  debug("Worker started\n");

  config_t * config = arg;

  task_t task;  
  for(;;) {   
    queue_pop (&config->queue, &task);    
    task.to = task.from;
    task.from = 0;

    config->brute_function(&task, config, config->check_function);

    pthread_mutex_lock(&config->num_tasks_mutex);
    --config->num_tasks;
    pthread_mutex_unlock(&config->num_tasks_mutex);

    if (config->num_tasks == 0)
      pthread_cond_signal(&config->num_tasks_cv);
  }

  pthread_exit(EXIT_SUCCESS);
}

void multi_brute(config_t *config) {
  debug("Started in multi-thread mode in %d threads\n", config->num_threads);

  queue_init(&config->queue);

  config->num_tasks = 0;      

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  int cpu;
  for (cpu = 0; cpu < config->num_threads; cpu++){
    pthread_t crypt_thread;
    pthread_create (&crypt_thread, &attr, client_worker, config);
  }

  task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  for (int i = 0; i < initial_task.to; i++) {
    initial_task.password[i] = config->alphabet[0];
  }

  task_t task;
  task.from = initial_task.from + LAST_BRUTE_CHARS;
  task.to = initial_task.to;
  strcpy(task.password, initial_task.password);

  config->brute_function (&task, config, add_to_queue);     

  pthread_mutex_lock(&config->num_tasks_mutex);
  while (config->num_tasks > 0)
    pthread_cond_wait(&config->num_tasks_cv, &config->num_tasks_mutex);
  pthread_mutex_unlock (&config->num_tasks_mutex);
}

void bruteforce(config_t *config) {
  if (config->num_threads > 1) {
    multi_brute(config);
  } else {
    single_brute(config);
  }
}

void help_routine() {
  printf("Available commands:\n\n");
  printf("uberforcer help\t\t\t\tshow help file\n");
  printf("uberforcer crypt <password> <salt>\tcall crypt function\n");
  printf("uberforcer decrypt <hash> <args>\tcall decrypt function\n");
  printf("uberforcer benchmark <args>\t\tperform benchmarking\n");
  printf("\n");
  printf("Available arguments:\n");
  printf("-r or --recursive\t\t\tuse recursive bruteforcing algorithm; mutually exclusive with --iterative\n");
  printf("-i or --iterative\t\t\t[default] use iterative bruteforcing algorithm; mutually exclusive with --recursive\n");
  printf("-a <value> or --alphabet <value>\tavailable alphabet\n");
  printf("-l <value> or --length <value>\t\tpresumed length of password\n");
}

void encrypt_routine(config_t *config) {
  debug("Started encryption with: password=%s, salt=%s\n", config->value, config->salt);

  char buf[CRYPT_HASH_SIZE];
  char *encrypted = crypt(config->value, config->salt, &buf[0]);
  printf("%s", encrypted);
}

void decrypt_routine(config_t *config) {
  debug("Started decryption in %s mode\n", config->bruteforce_mode == BF_ITER ? "iterative" : "recursive");

  bruteforce(config);

  if (config->result.found != false) {
    printf("Result found: %s\n", config->result.password);
  } else {
    printf("Result not found\n");
  }
}

void benchmark_routine(config_t *config) {
  debug("Started benchmark in %s mode\n", config->bruteforce_mode == BF_ITER ? "iterative" : "recursive");

  struct timeval start;
  struct timeval end;

  gettimeofday(&start, NULL);

  bruteforce(config);

  gettimeofday(&end, NULL);

  int alphabet_len = strlen(config->alphabet);
  long long cnt = 1;
  for (int i = 0; i < config->length; i++){
    cnt *= alphabet_len;
  }

  float elapsed = ((end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec) / 1000.0f) / 1000.0;
  float perf = cnt * 1.0 / elapsed;

  printf("Bruteforced: %ld items.\n", cnt);
  printf("Time elapsed: %.4f seconds\n", elapsed);
  printf("Performance: %.2f hashes / sec", perf);
}

int main(int argc, char *argv[]) {
  config_t config = {
    .brute_function = bruteforce_iter,
    .check_function = check_task,

    .app_mode = APP_MODE_HELP,
    .bruteforce_mode = BF_ITER,

    // Default parameters
    .alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890",
    .length = 4,
    .num_threads = 1
  };

  if (EXIT_SUCCESS == parse_args(argc, argv, &config)) {
    switch (config.app_mode) {
      case APP_MODE_HELP:
        help_routine();
        break;

      case APP_MODE_CRYPT:
        encrypt_routine(&config);
        break;

      case APP_MODE_DECRYPT:
        decrypt_routine(&config);
        break;

      case APP_MODE_BENCHMARK:
        benchmark_routine(&config);
        break;

      default:
        help_routine();
    }
  } else {
    printf("\n");
    help_routine();
  }

  return EXIT_SUCCESS;
}