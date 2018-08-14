/**************************************************************************
 *            Application arguments parsing
 *
 *   File    : parse_args.c
 *   Purpose : Provides ability to parse console application parameters
 *   Author  : Mikhail Mordvinkin
 *   Date    : July 24, 2018
 *
 ***************************************************************************
 *   There are two parts:
 *   - Parse main application run mode + required parameters
 *     * help
 *     * crypt <password> <salt>
 *     * decrypt <hash>
 *     * benchmark
 *
 *   - Parse optional run argument
 *     * Alphabet (i.e. abcdefABCDEF0123)
 *     * Length of original password
 *     * Bruteforce mode (iterative or recursive)
 *     * Number of threads for multithreading bruteforce
 **************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "bruteforce.h"
#include "check.h"
#include "config.h"

/**************************************************************************
 *
 * Description: Specific validator to ensure that provided char array
 *              can be parsed as integer and between minValue and maxValue
 *
 * Inputs:      char *s
 *              Pointer to a char array to test
 *
 *              int minValue
 *              Min value (inclusive) of an integer
 *
 *              int maxValue
 *              Max value (inclusive) of an integer
 *
 * Returns:     bool (short int alias)
 *              true or false
 *
 *************************************************************************/
bool validate_integer(char *s, int minValue, int maxValue) {
  int valueLen = strlen(s);
  if (valueLen < 1 || valueLen > 8) {
    return false;
  }

  for (int j = 0; j < valueLen; j++) {
    if (s[j] < '0' || s[j] > '9') {
      return false;
    }
  }

  int value = atoi(s);

  return value >= minValue && value <= maxValue ? true : false;
}

/**************************************************************************
 *
 * Description: Parses optional arguments to run the app:
 *               - Number of threads
 *               - Bruteforce mode
 *              etc.
 *
 *              If parsing failed, writes a message to stderr and returns
 *              EXIT_FAILURE (1). Otherwise, returns EXIT_SUCCESS (0)
 *
 * Inputs:      int start_arg_ind
 *              Index of first argument in the argv array, since half of
 *              arguments already was parsed by parse_app_mode (inclusive)
 *
 *              int end_arg_ind
 *              Index of last argument in the argv array (inclusive)
 *
 *              *argv[]
 *              Array of string parameters (argument from main func)
 *
 *              config_t *config
 *              Pointer to application config
 *
 * Returns:     int (exit status) - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int parse_run_params(int start_arg_ind, int end_arg_ind, char *argv[], config_t *config) {
  for (int i = start_arg_ind; i <= end_arg_ind; ++i) {
    if (strcmp("-r", argv[i]) == 0 || strcmp("--recursive", argv[i]) == 0) {
      config->bruteforce_mode = BF_REC;
      config->brute_function = bruteforce_rec;
    }

    if (strcmp("-i", argv[i]) == 0 || strcmp("--iterative", argv[i]) == 0) {
      config->bruteforce_mode = BF_ITER;
      config->brute_function = bruteforce_iter;
    }

    if (strcmp("-t", argv[i]) == 0 || strcmp("--threads", argv[i]) == 0) {
      if (i + 1 <= end_arg_ind) {
        if (true != validate_integer(argv[i + 1], 1, 128)) {
          fprintf(stderr, "Threads key specified, but value is invalid. It should be numeric between 1 and 128 (inclusive)");
          return EXIT_FAILURE;
        }

        config->num_threads = atoi(argv[i + 1]);
        ++i;
      }
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
        if (true != validate_integer(argv[i + 1], 1, 65535)) {
          fprintf(stderr, "Length key specified, but value is invalid. It should be numeric between 1 and 65535.");
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

/**************************************************************************
 *
 * Description: Parses command line arguments to run program:
 *              run mode (help, crypt, decrypt, ...)
 *              required parameters for selected app mode
 *              optional parameters
 *
 *              If parsing was unsuccessful for some reason, writes error to
 *              stderr and returns EXIT_FAILURE
 *
 * Inputs:      int raw_argc
 *              Number of arguments from command line (argument from main func)
 *
 *              *argv[]
 *              Array of string parameters (argument from main func)
 *
 *              config_t *config
 *              Pointer to application config
 *
 * Returns:     int (exit status) - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int parse_args(int raw_argc, char *argv[], config_t *config) {
  int args_count = raw_argc - 1;
  if (args_count < 1) {
    config->app_mode = APP_MODE_HELP;
    return EXIT_SUCCESS;
  }

  char *mode = argv[1];

  if (strcmp("help", mode) == 0) {
    config->app_mode = APP_MODE_HELP;
    return EXIT_SUCCESS;
  }

  if (strcmp("crypt", mode) == 0) {
    if (args_count < 3) {
      fprintf(stderr, "Insufficient number of arguments. Expected: %s\n", "crypt <password> <salt>");

      return EXIT_FAILURE;
    }

    config->app_mode = APP_MODE_CRYPT;
    config->password = argv[2];
    config->salt = argv[3];

    return EXIT_SUCCESS;
  }

  if (strcmp("decrypt", mode) == 0) {
    if (args_count < 2) {
      fprintf(stderr, "Insufficient number of argument. Expected: %s\n", "decrypt <hash>");

      return EXIT_FAILURE;
    }

    config->app_mode = APP_MODE_DECRYPT;
    config->check_function = check_task;
    config->hash = argv[2];

    if (EXIT_SUCCESS == parse_run_params(3, args_count, argv, config)) {
      config->check_function = config->num_threads > 1 ? check_task_r : check_task;
      return EXIT_SUCCESS;
    } else {
      return EXIT_FAILURE;
    }
  }

  if (strcmp("benchmark", mode) == 0) {
    config->app_mode = APP_MODE_BENCHMARK;
    config->check_function = check_task_benchmark;

    if (EXIT_SUCCESS == parse_run_params(2, args_count, argv, config)) {
      config->check_function = config->num_threads > 1 ? check_task_benchmark_r : check_task_benchmark;
      return EXIT_SUCCESS;
    } else {
      return EXIT_FAILURE;
    }
  }

  if (strcmp("server", mode) == 0) {
    if (args_count < 3) {
      fprintf(stderr, "Insufficient number of argument. Expected: %s\n", "server <hash> <port>");
      return EXIT_FAILURE;
    }

    if (true != validate_integer(argv[3], 1024, 65535)) {
      fprintf(stderr, "Invalid value for <port>: should be numeric between 1024 and 65535 (inclusive)\n");
      return EXIT_FAILURE;
    }

    config->app_mode = APP_MODE_SERVER;
    config->hash = argv[2];
    config->port = (unsigned short)atoi(argv[3]);

    return parse_run_params(4, args_count, argv, config);
  }

  if (strcmp("client", mode) == 0) {
    if (args_count < 3) {
      fprintf(stderr, "Insufficient number of argument. Expected: %s\n", "client <host> <port>");
      return EXIT_FAILURE;
    }

    if (true != validate_integer(argv[3], 1024, 65535)) {
      fprintf(stderr, "Invalid value for <port>: should be numeric between 1024 and 65535 (inclusive)\n");
      return EXIT_FAILURE;
    }

    config->app_mode = APP_MODE_CLIENT;
    config->host = argv[2];
    config->port = (unsigned short)atoi(argv[3]);

    return parse_run_params(4, args_count, argv, config);
  }

  fprintf(stderr, "Command not recognized: %s\n", mode);

  return EXIT_FAILURE;
}