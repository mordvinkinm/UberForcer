#include <string.h>
    
#include "struct.h"

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
void init_task(task_t *task) {
  task->from = 0;
  task->to = 0;

  memset(&task->password, 0, sizeof(task->password));
}

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
void init_result(result_t *result) {
  result->found = false;

  memset(&result->password, 0, sizeof(result->password));
}