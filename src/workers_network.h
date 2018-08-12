#ifndef WORKERS_NETWORK_H
#define WORKERS_NETWORK_H

#include "config.h"

#define FAILED_TASK_THRESHOLD 3
#define BUF_SIZE 512

void server_listener(config_t * config);

void client_job(config_t * config);

#endif /* WORKERS_NETWORK_H */