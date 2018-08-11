#ifndef RESULT_H
#define RESULT_H

#include "common.h"

#define MAX_LEN (20)

typedef char password_t[MAX_LEN + 1];

typedef struct task_s {
  password_t password;
  int from;
  int to;
} task_t;

typedef struct result_s {
  password_t password;
  bool found;
} result_t;

#endif /* RESULT_H */