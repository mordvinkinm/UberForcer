#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "check.h"
#include "bruteforce.h"

bool validate_integer(char* s) {
  int valueLen = strlen(s);
  if (valueLen < 1 || valueLen > 8){
    return EXIT_FAILURE;
  }

  for (int j = 0; j < valueLen; j++) {
    if (s[j] < '0' || s[j] > '9'){
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int parse_params(int start_arg_ind, int end_arg_ind, char *argv[], config_t *config) {
  for (int i = start_arg_ind; i <= end_arg_ind; ++i){
    if (strcmp("-r", argv[i]) == 0 || strcmp("--recursive", argv[i]) == 0) {
      config->bruteforce_mode = BF_REC;
      config->brute_function = bruteforce_rec;
    }

    if (strcmp("-i", argv[i]) == 0 || strcmp("--iterative", argv[i]) == 0) {
      config->bruteforce_mode = BF_ITER;
      config->brute_function = bruteforce_iter;
    }

    if (strcmp("-a", argv[i]) == 0 || strcmp("--alphabet", argv[i]) == 0) {
      if (i + 1 <= end_arg_ind) {
        config->alphabet = argv[i + 1];
        ++i;
      } else {
        fprintf(stderr, "Value of alphabet key is not provided");
        return EXIT_FAILURE;
      }
    }

    if (strcmp("-l", argv[i]) == 0 || strcmp("--length", argv[i]) == 0) {
      if (i + 1 <= end_arg_ind) {
        if (EXIT_SUCCESS != validate_integer(argv[i + 1])) {
          fprintf(stderr, "Length key specified, but value is invalid. It should be numeric between 1 and 99 999 999.");
          return EXIT_FAILURE;
        }

        config->length = atoi(argv[i + 1]);
        ++i;
      } else {
        fprintf(stderr, "Length key specified, but value is not provided");
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
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
    config->check_function = check_task;
    config->value = argv[2];
    
    return parse_params(3, arg_cnt, argv, config);
  }

  if (strcmp("benchmark", mode) == 0) {
    config->app_mode = APP_MODE_BENCHMARK;
    config->check_function = check_task_benchmark;

    return parse_params(2, arg_cnt, argv, config);
  }

  fprintf(stderr, "Command not recognized: %s\n", mode);

  return EXIT_FAILURE;
}