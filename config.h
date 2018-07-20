#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

typedef enum { 
  APP_MODE_HELP, 
  APP_MODE_CRYPT 
} app_mode_t;

typedef struct config_s {
  app_mode_t app_mode;

  char* value;
  char* salt;
} config_t;

#endif /* CONFIG_H */