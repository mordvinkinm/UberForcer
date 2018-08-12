#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <semaphore.h>

#include "struct.h"

#define QUEUE_SIZE (10)

/**************************************************************************
 * Struct:      queue_t
 *
 * Description: Stucture to implement array-based thread-safe queue.
 *
 *              Contains:
 *              array of data
 *              reference to head / tail (index of head/tail elements)
 *              sync primitives
 *
 *************************************************************************/
typedef struct queue_s {
  task_t queue[QUEUE_SIZE];

  int head;
  int tail;

  sem_t full;
  sem_t empty;

  pthread_mutex_t head_mutex;
  pthread_mutex_t tail_mutex;
} queue_t;

/**************************************************************************
 * Function:    queue_init
 *
 * Description: initializes queue of tasks referenced by a provided pointer
 *
 * Inputs:      queue_t *q
 *              Pointer to array-based queue of fixed size
 *
 * Returns:     none
 *
 *************************************************************************/
void queue_init(queue_t *q);

/**************************************************************************
 * Function:    queue_push
 *
 * Description: adds a task to the end of queue
 *
 * Inputs:      queue_t *q
 *              Pointer to queue
 *
 *              task_t *t
 *              Task to add to the queue
 *
 * Returns:     none
 *
 *************************************************************************/
void queue_push(queue_t *q, task_t *t);

/**************************************************************************
 * Function:    queue_pop
 *
 * Description: removes first task from the queue
 *
 * Inputs:      queue_t *q
 *              Pointer to array-based queue of fixed size
 *
 *              task_t *t
 *              Out buffer in which removed task will be stored
 *
 * Returns:     none
 *
 *************************************************************************/
void queue_pop(queue_t *q, task_t *t);

#endif /*QUEUE_H*/
