#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include "config.h"

/**************************************************************************
* Function:    bruteforce_rec
*
* Description: Performs recursive bruteforce
*
* Inputs:      task_t *task
*              Bruteforcing task - initial password string and bruteforcing
*              boundaries (for example, task "password, from: 2, to: 4")
*
*              config_t *config
*              Pointer to application config
*
*              void (*check_handler)(config_t *config, task_t *task)
*              Pointer to function that checks generated result against 
*              target hash
*
* Returns:     none
* 
*************************************************************************/
void bruteforce_rec(task_t* task, config_t* config, void (*check_handler)(config_t* config, task_t* task));


/**************************************************************************
* Function:    bruteforce_iter
*
* Description: Performs iterative bruteforce
*
* Inputs:      task_t *task
*              Bruteforcing task - initial password string and bruteforcing
*              boundaries (for example, task "password, from: 2, to: 4")
*
*              config_t *config
*              Pointer to application config
*
*              void (*check_handler)(config_t *config, task_t *task)
*              Pointer to function that checks generated result against 
*              target hash
*
* Returns:     none
* 
*************************************************************************/
void bruteforce_iter(task_t* task, config_t* config, void (*check_handler)(config_t* config, task_t* task));

#endif /* BRUTEFORCE_H */