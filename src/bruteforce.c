/**************************************************************************
 *            Bruteforcing algorithms
 *
 *   File    : bruteforce.c
 *   Purpose : Provides ability to bruteforce all strings
 *             of provided alphabet and length
 *   Author  : Mikhail Mordvinkin
 *   Date    : July 24, 2018
 *
 ***************************************************************************
 *   Two bruteforcing algorithms - recursive and iterative
 *
 *   They are equal from performance perspective.
 **************************************************************************/

#include <string.h>

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
 *              Pointer to function that will be run for each bruteforce result
 *              (for example, function to check hash; or another bruteforce
 *              function if we need to parallelize bruteforce)
 *
 * Returns:     none
 *
 *************************************************************************/
void bruteforce_rec(task_t* task, config_t* config, void (*check_handler)(config_t* congfig, task_t* task)) {
  if (config->result.found != false)
    return;

  int alphabet_length = strlen(config->alphabet);

  void rec(int depth) {
    if (depth == task->to) {
      if (config->result.found != false)
        return;

      check_handler(config, task);
    } else {
      if (config->result.found != false)
        return;

      for (int i = 0; i < alphabet_length; i++) {
        task->password[depth] = config->alphabet[i];
        rec(depth + 1);
      }
    }
  }

  rec(task->from);
}

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
 *              Pointer to function that will be run for each bruteforce result
 *              (for example, function to check hash; or another bruteforce
 *              function if we need to parallelize bruteforce)
 *
 * Returns:     none
 *
 *************************************************************************/
void bruteforce_iter(task_t* task, config_t* config, void (*check_handler)(config_t* config, task_t* task)) {
  int i;
  int alphabet_length = strlen(config->alphabet);
  int new_text[MAX_LEN + 1];

  for (i = task->from; i < task->to; ++i)
    new_text[i] = 0;

  for (;;) {
    if (config->result.found != false)
      return;

    check_handler(config, task);

    for (i = task->to - 1; (i >= task->from) && (new_text[i] == alphabet_length - 1); --i)
      task->password[i] = config->alphabet[new_text[i] = 0];

    if (i < task->from)
      break;

    task->password[i] = config->alphabet[++new_text[i]];
  }
}