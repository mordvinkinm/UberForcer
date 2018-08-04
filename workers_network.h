#ifndef WORKERS_NETWORK_H
#define WORKERS_NETWORK_H

#include "config.h"

void server_listener();

void client_job(config_t * config);

#endif /* WORKERS_NETWORK_H */