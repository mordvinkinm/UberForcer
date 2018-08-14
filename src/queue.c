/**************************************************************************
 *            Thread-safe efficient queue
 *
 *   File    : queue.c
 *   Purpose : Provides thread-safe queue functionality
 *   Author  : Mikhail Mordvinkin
 *   Date    : July 24, 2018
 *
 ***************************************************************************
 *   This queue is array-based and has fixed size (by default - 10)
 *   due to efficiency concerns. Linked list-based or expandable array-based
 *   queues can't provide such level of performance.
 *
 *   Implemented only two operations from queue interface - push and pop
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

/**************************************************************************
 * Function:    queue_init
 *
 * Description: initializes queue, setting mutexes, semaphores and head/tail
 *
 * Inputs:      queue_t *q
 *              Pointer to queue structure
 *
 *************************************************************************/
void queue_init(queue_t* q) {
  q->head = 0;
  q->tail = 0;

  sem_init(&q->full, 0, 0);
  sem_init(&q->empty, 0, sizeof(q->queue) / sizeof(q->queue[0]));

  pthread_mutex_init(&q->tail_mutex, NULL);
  pthread_mutex_init(&q->head_mutex, NULL);
}

/**************************************************************************
 * Function:    queue_push
 *
 * Description: adds element to the end of queue
 *              if queue is full, wait on semaphore until at least one
 *              element will be removed (using sem_wait).
 *
 * Inputs:      queue_t *q
 *              Pointer to array-based queue of fixed size
 *
 *              task_t *result
 *              Pointer to task to be added into the queue
 *
 *************************************************************************/
void queue_push(queue_t* q, task_t* task) {
  sem_wait(&q->empty);

  pthread_mutex_lock(&q->tail_mutex);
  q->queue[q->tail] = *task;
  if (++q->tail == sizeof(q->queue) / sizeof(q->queue[0]))
    q->tail = 0;

  pthread_mutex_unlock(&q->tail_mutex);

  sem_post(&q->full);
}

/**************************************************************************
 * Function:    queue_pop
 *
 * Description: removes first element from the queue.
 *              If there are no elements in the queue, wait until at least
 *              one element will be added (using sem_wait)
 *
 * Inputs:      queue_t *q
 *              Pointer to array-based queue of fixed size
 *
 *              task_t *result
 *              Out buffer in which removed task will be stored
 *
 *************************************************************************/
void queue_pop(queue_t* q, task_t* result) {
  sem_wait(&q->full);
  pthread_mutex_lock(&q->head_mutex);

  *result = q->queue[q->head];
  if (++q->head == sizeof(q->queue) / sizeof(q->queue[0]))
    q->head = 0;

  pthread_mutex_unlock(&q->head_mutex);
  sem_post(&q->empty);
}
