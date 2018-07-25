#include <string.h>

#include "lib/crypt3.h"
#include "config.h"
#include "common.h"

void check_task(config_t* config, task_t* result) {
  char buf[CRYPT_HASH_SIZE];
  char* hash = crypt(result->password, config->value, &buf[0]);

  trace("Password checked: %s, hash: %s\n", result->password, hash);
  if (strcmp(hash, config->value) == 0) {
    debug("Result found, trying to acquire mutex: %s\n", result->password);
    pthread_mutex_lock(&config->result_mutex);
    debug("Result found, mutex acquired: %s\n", result->password);
    config->result.found = true;
    strcpy(config->result.password, result->password);
    pthread_mutex_unlock(&config->result_mutex);
    debug("Result found, mutex released: %s\n", result->password);
  }
}

void check_task_benchmark(config_t* config, task_t* result) {
  trace("Password checked: %s\n", result->password);
  
  // prevent "unused" variable from optimization
  char buf[CRYPT_HASH_SIZE];
  volatile int whatever = strcmp(crypt(result->password, "salt", &buf[0]), "hash");
}