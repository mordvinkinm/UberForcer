/**************************************************************************
*            Entry point for uberforcer application
*
*   File    : main.c
*   Author  : Mikhail Mordvinkin
*   Date    : July 24, 2018
*
***************************************************************************
*   Entry point for uberforcer app:
*   call method to parse app argument;
*   constructs initial tasks and data structures;
*   run appropriate routines depending on selected application mode and parameters.
*   
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "lib/crypt3.h"
#include "bruteforce.h"
#include "parse_args.h"
#include "network.h"
#include "check.h"
#include "config.h"
#include "struct.h"
#include "workers.h"
#include "workers_network.h"

#define LAST_BRUTE_CHARS (2)

void bruteforce(config_t *config) {
  task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  for (int i = 0; i < initial_task.to; i++) {
    initial_task.password[i] = config->alphabet[0];
  }

  if (config->num_threads > 1) {
    multi_brute(config, &initial_task);
  } else {
    single_brute(config, &initial_task);
  }
}

void* generate_server_tasks_thread_job(void* raw_args) {
    worker_args_t * args = raw_args;
    config_t * config = args->config;

    task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  for (int i = 0; i < initial_task.to; i++) {
    initial_task.password[i] = config->alphabet[0];
  }

  generate_tasks_worker(config, &initial_task);
}

void generate_server_tasks(config_t * config){
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_cond_init(&config->num_tasks_cv, NULL);

  pthread_t thread;
  worker_args_t args = {
    .config = config,
    .thread_number = 1000
  };

  pthread_create (&thread, &attr, generate_server_tasks_thread_job, &args);
}

void help_routine() {
  printf("Available commands:\n\n");
  printf("uberforcer help\t\t\t\t\tshow help file\n");
  printf("uberforcer crypt <password> <salt>\t\tencrypt provided <password>, using provided <salt>\n");
  printf("uberforcer decrypt <hash> <args>\t\tbruteforce provided <hash>, using additional arguments <args> [optional]. available arguments: -r, -i, -a, -l, -t\n");
  printf("uberforcer benchmark <args>\t\t\tperform benchmarking, using bruteforcing arguments <args> [optional]. available arguments: -r, -i, -a, -l, -t\n");
  printf("uberforcer server <hash> <port> <args>\t\t\tstart bruteforcing server on the provided port, to bruteforce <hash>, using bruteforcing arguments <args> [optional]. available arguments: -r, -i, -a, -l\n");
  printf("uberforcer client <host> <port> <args>\t\tstart bruteforcing client to connect to server on <host>:<port>, using bruteforcing arguments <args> [optional]. available arguments: -r, -i, -t\n");
  printf("\n");
  printf("Available <args> arguments:\n");
  printf("-r or --recursive\t\t\t\tuse recursive bruteforcing algorithm; mutually exclusive with --iterative\n");
  printf("-i or --iterative\t\t\t\t[default] use iterative bruteforcing algorithm; mutually exclusive with --recursive\n");
  printf("-a <value> or --alphabet <value>\t\tavailable alphabet; default: [A-Za-z0-9]\n");
  printf("-l <value> or --length <value>\t\t\tpresumed length of password; default: 4\n");
  printf("-t <value> or --threads <value>\t\t\tNumber of threads for multithreading bruteforce; default: 1\n");
}

void encrypt_routine(config_t *config) {
  debug("Started encryption with: password=%s, salt=%s\n", config->value, config->salt);

  char *encrypted = crypt(config->value, config->salt);
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

void server_routine(config_t *config) {
  debug("Started bruteforcing server\n");

  init_network();

  generate_server_tasks(config);
  server_listener(config);
}

void client_routine(config_t *config) {
  debug("Started bruteforcing client\n");

  init_network();

  client_job(config);
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
    .num_threads = 1,
    .port = 7534
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

      case APP_MODE_SERVER:
        server_routine(&config);
        break;

      case APP_MODE_CLIENT:
        client_routine(&config);
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