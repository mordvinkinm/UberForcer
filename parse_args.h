#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "config.h"

int parse_args(int argc, char *argv[], config_t *config);

int parse_params(int start_arg_ind, int end_arg_ind, char *argv[], config_t *config);

#endif /* PARSE_ARGS_H */