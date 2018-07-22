#include <stdlib.h>
#include <stdio.h>

#include "queue.h"


int queue_init(queue_t * q) {
  q->head = 0;
  q->tail = 0;

  sem_init(&q -> full, 0, 0);
  sem_init(&q -> empty, 0, sizeof(q->queue)/sizeof(q->queue[0]));

  pthread_mutex_init(&q -> tail_mutex, NULL);
  pthread_mutex_init(&q -> head_mutex, NULL);

  return EXIT_SUCCESS;
}

void queue_pop (queue_t * q, task_t * result) {
  sem_wait(&q->full);
  pthread_mutex_lock(&q->head_mutex);
  
  *result = q->queue[q->head];
  if (++q->head == sizeof(q->queue) / sizeof (q->queue[0]))
    q->head = 0;
  
  pthread_mutex_unlock(&q->head_mutex);
  sem_post(&q->empty);
}

void queue_push(queue_t * q, task_t * task) {
  sem_wait (&q->empty);

  pthread_mutex_lock (&q->tail_mutex);
  q->queue[q->tail] = * task;
  if (++q->tail == sizeof (q->queue) / sizeof (q->queue[0]))
    q->tail = 0;

  pthread_mutex_unlock (&q->tail_mutex);

  sem_post (&q->full);
}


