#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "workers_network.h"
#include "workers.h"
#include "config.h"
#include "protocol.h"
#include "network.h"

void communicate_client(config_t * config, task_t * task, int sock) {
  int msglen;
  if (read (sock, &msglen, sizeof(msglen))) 
      fprintf (stderr, "Error reading config\n");

  char * buf = malloc(sizeof(char) * msglen);
  sprintf(buf, MSG_SEND_JOB, task->password, config->value, config->alphabet, task->from, task->to);
  write(sock, buf, msglen);
    
  if (read (sock, &msglen, sizeof(msglen)))
    fprintf(stderr, "Error reading config\n");

  read(sock, buf, msglen);
  char * password = malloc(sizeof(char) * msglen);
  int found;
  sscanf(buf, MT_REPORT_RESULT, password, &found);

  trace("Result: %s, Found: %d", paessword, found);
}

void server_task_manager(int sock, config_t * config){
  task_t task;
  for (;;) {
    queue_pop(&config->queue, &task);
    task.to = task.from;
    task.from = 0;

    communicate_client(config, &task, sock);

    pthread_mutex_lock(&config->num_tasks_mutex);
    --config->num_tasks;
    pthread_mutex_unlock(&config->num_tasks_mutex);

    if (config->num_tasks == 0)
      pthread_cond_signal(&config->num_tasks_cv);
  }
}

void * server_task_manager_job (void * raw_args) {
  client_listener_args_t * args = raw_args;

  int sock = args->descriptor;
  config_t * config = args->config;

  server_task_manager(sock, config);

  return (NULL);
}

void server_listener(config_t * config) {
  debug("Started server listener on port %d\n", config->port);

  struct sockaddr_in serv_addr, client_addr;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    fprintf(stderr, "Error opening socket: %d (%s)\n", errno, strerror(errno));
    return;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(config->port);

  if (bind (sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr))){
      fprintf(stderr, "Error binding socket: %d (%s)\n", errno, strerror(errno));
      return;
  }

  listen (sock, 10);

  debug("Waiting for incoming connections...\n");

  while (1) {
    socklen_t cl_len = sizeof (client_addr);
    int newsock = accept(sock, (struct sockaddr *) &client_addr, &cl_len);
    debug("Connection accepted\n");

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t id;

    client_listener_args_t args = {
      .descriptor = newsock,
      .config = config,
    };

    pthread_create (&id, &attr, server_task_manager_job, &args);    
  }
}

void client_job (config_t * config){
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(config->port);

  struct hostent * server;
  server = gethostbyname (config->host);
  memmove ((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);

  printf ("Establish connect to %s\n", config->host);

  int sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    fprintf(stderr, "Error creating socket\n");
    return;
  }

  if (connect (sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
    fprintf (stderr, "Error connecting to %s\n", config->host);
    return;
  }

  printf ("Connected to %s\n", config->host);

  // Reading config from server
  int msglen;
  if (read (sock, &msglen, sizeof(msglen))) {
      fprintf (stderr, "Error reading config\n");
      return;
  }

  // TASK PASSWORD
  char * buf = malloc(sizeof(char) * msglen);
  read (sock, buf, msglen);

  task_t task;
  sscanf (buf, MSG_SEND_JOB, &task.password, config->value, config->alphabet, &task.from, &task.to);

  if (config->num_threads > 1) {
    multi_brute(config, &task);
  } else {
    single_brute(config, &task);
  }
}