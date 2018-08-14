/**************************************************************************
 *            Hash checking algorithms
 *
 *   File    : check.c
 *   Purpose : Provides ability to check if password matches target hash
 *   Author  : Mikhail Mordvinkin
 *   Date    : July 24, 2018
 *
 ***************************************************************************
 *   Function to check password against target hash
 *   Function to check bruteforcing performance
 *
 *   Both functions have thread-unsafe and reentrant versions
 **************************************************************************/

#include <string.h>

#include "../lib/crypt3.h"
#include "common.h"
#include "config.h"

/**************************************************************************
 *
 * Description: Thread-unsafe version of hash checker.
 *              Calls "crypt" function and checks if result equals to target
 *              hash; if equals, then fills config.task's found property.
 *
 * Inputs:      config_t *config
 *              Pointer to application config
 *
 *              [out] task_t *task
 *              Pointer to task with password to check
 *
 *************************************************************************/
void check_task(config_t* config, task_t* task) {
  char* hash = crypt(task->password, config->hash);
  trace("Password checked: %s, hash: %s\n", task->password, hash);

  if (strcmp(hash, config->hash) == 0) {
    config->result.found = true;
    strcpy(config->result.password, task->password);
  }
}

/**************************************************************************
 *
 * Description: Thread-unsafe function to calculate bruteforcing performance.
 *              Calls "crypt" function, but ignores result of "crypt" call.
 *              Because of that, bruteforce process will not be stopped after
 *              result is found.
 *
 * Inputs:      config_t *config
 *              Pointer to application config
 *
 *              task_t *task
 *              Pointer to task with generated password
 *
 *************************************************************************/
void check_task_benchmark(config_t* config, task_t* task) {
  trace("Password checked: %s\n", task->password);

  // prevent "unused" variable from optimization
  volatile int whatever = strcmp(crypt(task->password, "salt"), "hash");
}

/**************************************************************************
 *
 * Description: Reentrant version of hash checker.
 *              Under the hood, calls crypt_r and checks result against
 *              target hash.
 *
 * Inputs:      config_t *config
 *              Pointer to application config
 *
 *              task_t *task
 *              Pointer to task with generated password
 *
 *************************************************************************/
void check_task_r(config_t* config, task_t* task) {
  char iobuf[CRYPT_HASH_SIZE];
  char* hash = crypt_r(task->password, config->hash, &iobuf);
  trace("Password checked: %s, hash: %s\n", task->password, hash);

  if (strcmp(hash, config->hash) == 0) {
    pthread_mutex_lock(&config->result_mutex);
    config->result.found = true;
    strcpy(config->result.password, task->password);
    pthread_mutex_unlock(&config->result_mutex);
  }
}

/**************************************************************************
 *
 * Description: Reentrant function to calculate bruteforcing performance.
 *              Calls "crypt_r", but ignores the result (thus bruteforce
 *              will not be stopped after result is found)
 *
 * Inputs:      config_t *config
 *              Pointer to application config
 *
 *              task_t *task
 *              Pointer to task with generated password
 *
 *************************************************************************/
void check_task_benchmark_r(config_t* config, task_t* task) {
  trace("Password checked: %s\n", task->password);

  // prevent "unused" variable from optimization
  char iobuf[CRYPT_HASH_SIZE];
  volatile int whatever = strcmp(crypt_r(task->password, "salt", &iobuf), "hash");
}