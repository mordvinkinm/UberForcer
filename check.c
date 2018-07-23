#include <string.h>

#include "config.h"
#include "common.h"
#include "crypt3.h"

void check_task(config_t* config, task_t* result) {
  trace("Password checked: %s\n", result->password);

  char buf[CRYPT_HASH_SIZE];
  if (strcmp(crypt(result->password, config->value, &buf[0]), config->value) == 0) {
    pthread_mutex_lock(&config->result_mutex);
    config->result.found = true;
    strcpy(config->result.password, result->password);
    pthread_mutex_unlock(&config->result_mutex);
  }
}

void check_task_benchmark(config_t* config, task_t* result) {
  trace("Password checked: %s\n", result->password);
  
  // prevent "unused" variable from optimization
  char buf[CRYPT_HASH_SIZE];
  volatile int whatever = strcmp(crypt(result->password, "salt", &buf[0]), "hash");
}