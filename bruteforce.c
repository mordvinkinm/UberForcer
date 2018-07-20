#include <string.h>

#include "config.h"

void bruteforce_rec(task_t* task, config_t* config, void (*check_handler)(config_t* congfig, char* task)) {
  if (config->result.found == true) 
    return;

  int alphabet_length = strlen(config->alphabet);

  void rec(int depth) {
    if (depth == task->to) {
      if (config->result.found == true) 
        return;

      check_handler(config, task->password);
    } else {
      if (config->result.found == true) 
        return;

      for (int i = 0; i < alphabet_length; i++) {
        task->password[depth] = config->alphabet[i];
        rec(depth + 1);
      }
    }
  }

  rec(task->from);
}