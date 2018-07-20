#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  fprintf(stderr, "Command not recognized: %s\n", mode);

  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  config_t config = {.app_mode = APP_MODE_HELP};

  if (EXIT_SUCCESS == parse_args(argc, argv, &config)) {
    if (APP_MODE_HELP == config.app_mode) {
      help_routine();
    } else if (APP_MODE_CRYPT == config.app_mode) {
      encrypt_routine(&config);
    }
  } else {
    printf("\n");
    help_routine();
  }

  return EXIT_SUCCESS;
}