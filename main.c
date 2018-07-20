#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "bruteforce.h"
#include "check.h"
#include "config.h"
#include "crypt3.h"

void help_routine() {
  printf("Available parameters:\n");
  printf("uberforcer help\t\t\t\t\tshow help file\n");
  printf("uberforcer crypt <password> <salt>\t\tcall crypt function\n");
}

void encrypt_routine(config_t *config) {
  char *encrypted = crypt(config->value, config->salt);
  printf("%s", encrypted);
}

void decrypt_routine(config_t *config) {
  task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  bruteforce_rec(&initial_task, config, check_task);

  if (config->result.found == true) {
    printf("Result found: %s\n", config->result.password);
  } else {
    printf("Result not found\n");
  }
}

void benchmark_routine(config_t *config) {
  task_t initial_task = {.from = 0, .to = config->length};

  struct timeval start;
  struct timeval end;

  gettimeofday(&start, NULL);

  bruteforce_rec(&initial_task, config, check_task_benchmark);

  gettimeofday(&end, NULL);

  int alphabet_len = strlen(config->alphabet);
  long long cnt = 1;
  for (int i = 0; i < config->length; i++){
    cnt *= alphabet_len;
  }

  float elapsed = (end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec) / 1000.0f;
  printf("Bruteforced %ld items. Time elapsed: %.4f\n", cnt, elapsed);
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

    return EXIT_SUCCESS;
  }

  if (strcmp("benchmark", mode) == 0){
    config->app_mode = APP_MODE_BENCHMARK;
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Command not recognized: %s\n", mode);

  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  config_t config = {
    .app_mode = APP_MODE_HELP,

    .alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890",
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