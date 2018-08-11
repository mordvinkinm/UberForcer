#ifndef CHECK_H
#define CHECK_H

#include "config.h"

/**************************************************************************
* Function:    check_task
*
* Description: Thread-unsafe version of hash checker
*
* Inputs:      config_t *config
*              Pointer to application config
*
*              task_t *result
*              Pointer to task with generated password
*
* Returns:     none
* 
*************************************************************************/
void check_task(config_t* config, task_t* result);


/**************************************************************************
* Function:    check_task_benchmark
*
* Description: Thread-unsafe function to calculate bruteforcing performance
*
* Inputs:      config_t *config
*              Pointer to application config
*
*              task_t *result
*              Pointer to task with generated password
*
* Returns:     none
* 
*************************************************************************/
void check_task_benchmark(config_t* config, task_t* result);


/**************************************************************************
* Function:    check_task_r
*
* Description: Reentrant version of hash checker
*
* Inputs:      config_t *config
*              Pointer to application config
*
*              task_t *result
*              Pointer to task with generated password
*
* Returns:     none
* 
*************************************************************************/
void check_task_r(config_t* config, task_t* result);


/**************************************************************************
* Function:    check_task_benchmark_r
*
* Description: Reentrant function to calculate bruteforcing performance
*
* Inputs:      config_t *config
*              Pointer to application config
*
*              task_t *result
*              Pointer to task with generated password
*
* Returns:     none
* 
*************************************************************************/
void check_task_benchmark_r(config_t* config, task_t* result);

#endif /* CHECK_H */