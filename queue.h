#ifndef QUEUE_H
#define QUEUE_H

#include <semaphore.h>
#include <pthread.h>

#include "struct.h"

#define QUEUE_SIZE (10)

typedef struct queue_s {
  task_t queue[QUEUE_SIZE];
  int head;
  int tail;
  sem_t full;
  sem_t empty;
  pthread_mutex_t head_mutex;
  pthread_mutex_t tail_mutex;
} queue_t;
 
void queue_push(queue_t * q, task_t * pass);

int queue_init(queue_t * q);

void queue_pop(queue_t *q, task_t * task);

#endif /*QUEUE_H*/
