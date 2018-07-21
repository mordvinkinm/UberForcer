#include <string.h>

#include "config.h"
#include "common.h"
#include "crypt3.h"

void check_task(config_t* config, char* result) {
  trace("Password checked: %s\n", result);

  if (strcmp(crypt(result, config->value), config->value) == 0) {
    config->result.found = true;
    strcpy(config->result.password, result);
  }
}

void check_task_benchmark(config_t* config, char* result){
  trace("Password checked: %s\n", result);
  
  // prevent "unused" variable from optimization
  volatile int whatever = strcmp(crypt(result, "salt"), "hash");
}