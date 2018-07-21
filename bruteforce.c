#include <string.h>

#include "config.h"

void bruteforce_rec(task_t* task, config_t* config, void (*check_handler)(config_t* congfig, char* task)) {
  if (config->result.found != false) 
    return;

  int alphabet_length = strlen(config->alphabet);

  void rec(int depth) {
    if (depth == task->to) {
      if (config->result.found != false) 
        return;

      check_handler(config, task->password);
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

void bruteforce_iter (task_t * task, config_t * config, void (*check_handler) (config_t * config, task_t * task)) {
  int i;
  int alphabet_length = strlen(config->alphabet);
  int new_text[MAX_LEN + 1];

  for (i = task->from; i < task->to; ++i)
    new_text[i] = 0;
 
  for(;;) {
    if (config->result.found != false)
      return;

    check_handler (config, task);

    for (i = task -> to - 1; (i >= task -> from) && (new_text[i] == alphabet_length - 1); --i)
      task -> password[i] = config -> alphabet[new_text[i] = 0];

    if (i < task -> from)
      break;

    task -> password[i] = config->alphabet[++new_text[i]];
  }
}