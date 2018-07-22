#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "bruteforce.h"
#include "check.h"
#include "config.h"
#include "crypt3.h"

void help_routine() {
  printf("Available commands:\n\n");
  printf("uberforcer help\t\t\t\t\tshow help file\n");
  printf("uberforcer crypt <password> <salt>\t\tcall crypt function\n");
  printf("uberforcer decrypt <hash> <args>\t\tcall decrypt function\n");
  printf("uberforcer benchmark <args>\t\t\tperform benchmarking\n");
  printf("\n");
  printf("Available arguments:\n");
  printf("-r or --recursive\t\tuse recursive bruteforcing algorithm; mutually exclusive with --iterative\n");
  printf("-i or --iterative\t\t[default] use iterative bruteforcing algorithm; mutually exclusive with --recursive\n");
}

void encrypt_routine(config_t *config) {
  debug("Started encryption with: password=%s, salt=%s\n", config->value, config->salt);

  char buf[CRYPT_HASH_SIZE];
  char *encrypted = crypt(config->value, config->salt, &buf[0]);
  printf("%s", encrypted);
}

void decrypt_routine(config_t *config) {
  debug("Started decryption in %s mode\n", config->bruteforce_mode == BF_ITER ? "iterative" : "recursive");

  task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  for (int i = 0; i < initial_task.to; i++){
    initial_task.password[i] = config->alphabet[0];
  }

  config->brute_function(&initial_task, config, check_task);

  if (config->result.found != false) {
    printf("Result found: %s\n", config->result.password);
  } else {
    printf("Result not found\n");
  }
}

void benchmark_routine(config_t *config) {
  debug("Started benchmark in %s mode\n", config->bruteforce_mode == BF_ITER ? "iterative" : "recursive");

  task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  for (int i = 0; i < initial_task.to; i++){
    initial_task.password[i] = config->alphabet[0];
  }

  struct timeval start;
  struct timeval end;

  gettimeofday(&start, NULL);

  config->brute_function(&initial_task, config, check_task_benchmark);

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

int parse_params(int start_arg_ind, int end_arg_ind, char*argv[], config_t *config){
  for (int i = start_arg_ind; i <= end_arg_ind; ++i){
    if (strcmp("-r", argv[i]) == 0 || strcmp("--recursive", argv[i]) == 0) {
      config->bruteforce_mode = BF_REC;
      config->brute_function = bruteforce_rec;
    }

    if (strcmp("-i", argv[i]) == 0 || strcmp("--iterative", argv[i]) == 0) {
      config->bruteforce_mode = BF_ITER;
      config->brute_function = bruteforce_iter;
    }
  }
}

int parse_args(int argc, char *argv[], config_t *config) {
  int arg_cnt = argc - 1;
  if (arg_cnt < 1) {
    config->app_mode = APP_MODE_HELP;
    return EXIT_SUCCESS;
  }

  char *mode = argv[1];

  if (strcmp("help", mode) == 0) {
    config->app_mode = APP_MODE_HELP;
    return EXIT_SUCCESS;
  }

  if (strcmp("crypt", mode) == 0) {
    if (arg_cnt < 3) {
      fprintf(stderr, "Insufficient number of arguments. Expected: %s\n", "crypt <password> <salt>");

      return EXIT_FAILURE;
    }

    config->app_mode = APP_MODE_CRYPT;
    config->value = argv[2];
    config->salt = argv[3];

    return EXIT_SUCCESS;
  }

  if (strcmp("decrypt", mode) == 0) {
    if (arg_cnt < 2) {
      fprintf(stderr, "Insufficient number of argument. Expected: %s\n", "decrypt <hash>");

      return EXIT_FAILURE;
    }

    config->app_mode = APP_MODE_DECRYPT;
    config->value = argv[2];
    
    parse_params(3, arg_cnt, argv, config);

    return EXIT_SUCCESS;
  }

  if (strcmp("benchmark", mode) == 0) {
    config->app_mode = APP_MODE_BENCHMARK;

    parse_params(2, arg_cnt, argv, config);

    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Command not recognized: %s\n", mode);

  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  config_t config = {
    .app_mode = APP_MODE_HELP,

    .bruteforce_mode = BF_ITER,
    .brute_function = bruteforce_iter,

    // Password sacred knowledge (i.e. possible characters, possible length)
    .alphabet = "abcdefghijklmnopqrstuvwxyz",
    .length = 4
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