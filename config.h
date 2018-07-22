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
  void (*brute_function) (task_t *, struct config_s *, void (*check_handler) (struct config_s * config, char * pass));
  void (*check_function) (struct config_s *config, char *result);

  app_mode_t app_mode;
  bf_mode_t bruteforce_mode;

  result_t result;

  char* alphabet;
  int length;

  char* value;
  char* salt;
} config_t;

#endif /* CONFIG_H */