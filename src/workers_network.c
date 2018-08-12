#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "workers_network.h"
#include "workers.h"
#include "config.h"
#include "protocol.h"
#include "network.h"

void re_add_task(config_t* config, task_t* task) {
  pthread_mutex_lock(&config->num_tasks_mutex);
  ++config->num_tasks;
  pthread_mutex_unlock(&config->num_tasks_mutex);
  queue_push(&config->queue, task);

  trace("Task added to the queue: [from: %d, to: %d, password: %s]\n", task->from, task->to, task->password);
}

int communicate_client(config_t * config, task_t * task, int sock, result_t * result) {
  trace("Preparing sending task to client: password %s, from: %d, to: %d\n", task->password, task->from, task->to);

  char * buf = malloc(sizeof(char) * BUF_SIZE);
  memset(buf, 0, BUF_SIZE);

  sprintf(buf, MSG_SEND_JOB, task->password, config->hash, config->alphabet, task->from, task->to);

  if (send(sock, buf, strlen(buf), 0) < 0) {
    fprintf(stderr, "Error writing to socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  trace("Message sent to client:\n%s\n", buf);
    
  memset(buf, 0, BUF_SIZE);

  if (recv(sock, buf, BUF_SIZE, 0) < 0) {
    fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  trace("Message received from client:\n%s\n", buf);

  sscanf(buf, MSG_REPORT_RESULT, result->password, &result->found);

  free(buf);

  return EXIT_SUCCESS;
}

void* server_task_manager_job (void * raw_args) {
  client_listener_args_t * args = raw_args;

  int sock = args->descriptor;
  config_t * config = args->config;

  task_t task;
  result_t result;
  
  int failed_task_cnt = 0;
  bool task_failed = false;
  for (;;) {
    if (task_failed == false) {
      queue_pop(&config->queue, &task);
      task.to = task.from;
      task.from = 0;
    }

    if (EXIT_SUCCESS != communicate_client(config, &task, sock, &result)) {
      ++failed_task_cnt;
      task_failed = true;

      if (failed_task_cnt >= FAILED_TASK_THRESHOLD) {
        fprintf(stderr, "Exceeded maximum amount of failed tasks. Client disconnected, re-adding task to the queue...\n");
        re_add_task(config, &task);
        pthread_exit(NULL);
      } else {
        fprintf(stderr, "Communication failed. Retrying this task...\n");
        continue;
      }
    } else {
      failed_task_cnt = 0;
      task_failed = false;
    }

    if (result.found == true) {
      printf("Result found: %s\n", result.password);

      strcpy(config->result.password, result.password);
      config->result.found = result.found;

      pthread_exit(NULL);
    }

    pthread_mutex_lock(&config->num_tasks_mutex);
    --config->num_tasks;
    pthread_mutex_unlock(&config->num_tasks_mutex);

    if (config->num_tasks == 0)
      pthread_cond_signal(&config->num_tasks_cv);
  }
}

void* server_listener_thread_job(void * raw_args) {
  worker_args_t * args = raw_args;
  config_t * config = args->config;

  debug("Started server listener on port %d\n", config->port);

  struct sockaddr_in serv_addr, client_addr;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    fprintf(stderr, "Error opening socket: %d (%s)\n", errno, strerror(errno));
    return NULL;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(config->port);

  if (bind (sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr))){
      fprintf(stderr, "Error binding socket: %d (%s)\n", errno, strerror(errno));
      return NULL;
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

void server_listener(config_t * config) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_cond_init(&config->num_tasks_cv, NULL);

  pthread_t thread;
  worker_args_t args = {
    .config = config,
    .thread_number = 1000
  };

  pthread_create (&thread, &attr, server_listener_thread_job, &args);
}

int client_job (config_t * config){
  struct sockaddr_in serv_addr;
  
  int sock = connect_to_server(config->host, config->port, &serv_addr);
  if (sock < 0) {
    return EXIT_FAILURE;
  }

  for (;;) {
    // TASK PASSWORD
    char * buf = malloc(sizeof(char) * BUF_SIZE);
    memset(buf, 0, BUF_SIZE);
    
    if (recv(sock, buf, BUF_SIZE, 0) < 0) {
      fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    trace("Message received from server:\n%s\n", buf);

    task_t task;
    memset(task.password, 0, sizeof(task.password));

    sscanf (buf, MSG_SEND_JOB, task.password, config->hash, config->alphabet, &task.from, &task.to);

    trace("Password: %s, Hash: %s, Alphabet: %s, from: %d, to: %d\n", task.password, config->hash, config->alphabet, task.from, task.to);

    if (config->num_threads > 1) {
      multi_brute(config, &task);
    } else {
      single_brute(config, &task);
    }

    memset(buf, 0, BUF_SIZE);
    sprintf(buf, MSG_REPORT_RESULT, config->result.password, config->result.found);

    if (send(sock, buf, strlen(buf), 0) < 0){
      fprintf(stderr, "Error writing to socket: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    trace("Message sent to server:\n%s\n", buf);
  }

  return EXIT_SUCCESS;
}