#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

#define MAX_LEN (20)

typedef enum { 
  APP_MODE_HELP, 
  APP_MODE_CRYPT,
  APP_MODE_DECRYPT
} app_mode_t;

typedef char password_t[MAX_LEN + 1];

typedef struct result_s {
  password_t password;
  bool found;
} result_t;

typedef struct task_s {
  password_t password;
  int from;
  int to;
} task_t;

typedef struct config_s {
  app_mode_t app_mode;
  result_t result;

  char* alphabet;
  int length;

  char* value;
  char* salt;
} config_t;

#endif /* CONFIG_H */