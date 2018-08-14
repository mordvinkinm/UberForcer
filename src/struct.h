#ifndef RESULT_H
#define RESULT_H

#include "common.h"

#define PASS_MAX_LEN (20)

typedef char password_t[PASS_MAX_LEN + 1];

/**************************************************************************
 *
 * Description: Structure to define bruteforcing task: characters of
 *              password will be bruteforced in range [from; to]
 *
 *************************************************************************/
typedef struct task_s {
  password_t password;
  int from;
  int to;
} task_t;

/**************************************************************************
 *
 * Description: Structure to define bruteforcing result - either result
 *              was found or not, plus found password (in case of success)
 *
 *************************************************************************/
typedef struct result_s {
  password_t password;
  bool found;
} result_t;

/**************************************************************************
 * Function:    init_task
 *
 * Description: Initializes task, setting its byte data to zeroes
 *
 * Inputs:      task_t *task
 *              Pointer to a task to initialize
 *
 * Returns:     none
 *
 *************************************************************************/
void init_task(task_t *task);

/**************************************************************************
 * Function:    bruteforce_rec
 *
 * Description: Initializes result, setting ts byte data to zeroes
 *
 * Inputs:      result_t *result
 *              Pointer to a result struct to initialize
 *
 * Returns:     none
 *
 *************************************************************************/
void init_result(result_t *result);

#endif /* RESULT_H */