#include <string.h>

#include "config.h"
#include "common.h"
#include "crypt3.h"

void check_task(config_t* config, char* result) {
  debug("Password checked: %s\n", result);

  if (strcmp(crypt(result, config->value), config->value) == 0) {
    config->result.found = true;
    strcpy(config->result.password, result);
  }
}