#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "bruteforce.h"
#include "parse_args.h"
#include "check.h"
#include "config.h"
#include "crypt3.h"

void bruteforce(config_t *config) {
 task_t initial_task = {
    .from = 0, 
    .to = config->length
  };

  for (int i = 0; i < initial_task.to; i++){
    initial_task.password[i] = config->alphabet[0];
  }

  config->brute_function(&initial_task, config, config->check_function);
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