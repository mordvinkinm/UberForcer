#ifndef RESULT_H
#define RESULT_H

#include "common.h"

#define MAX_LEN (20)

typedef char password_t[MAX_LEN + 1];

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

#endif /* RESULT_H */