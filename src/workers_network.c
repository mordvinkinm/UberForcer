#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "network.h"
#include "protocol.h"
#include "struct.h"
#include "workers.h"
#include "workers_network.h"

void re_add_task(config_t* config, task_t* task) {
  pthread_mutex_lock(&config->num_tasks_mutex);
  ++config->num_tasks;
  pthread_mutex_unlock(&config->num_tasks_mutex);
  queue_push(&config->queue, task);

  trace("Task added to the queue: [from: %d, to: %d, password: %s]\n", task->from, task->to, task->password);
}

void* server_task_manager_job(void* raw_args) {
  client_listener_args_t* args = raw_args;

  int sock = args->descriptor;
  config_t* config = args->config;

  task_t task;
  result_t result;

  init_task(&task);
  init_result(&result);

  int failed_task_cnt = 0;
  bool task_failed = false;
  for (;;) {
    if (task_failed == false) {
      queue_pop(&config->queue, &task);
      task.to = task.from;
      task.from = 0;
    }

    if (EXIT_SUCCESS != send_task(sock, config, &task) || EXIT_SUCCESS != read_result(sock, &result)) {
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

void* server_listener_thread_job(void* raw_args) {
  worker_args_t* args = raw_args;
  config_t* config = args->config;

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

  if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
    fprintf(stderr, "Error binding socket: %d (%s)\n", errno, strerror(errno));n
    
    return NULL;
  }

  listen(sock, 10);

  debug("Waiting for incoming connections...\n");

  while (1) {
    socklen_t cl_len = sizeof(client_addr);
    int newsock = accept(sock, (struct sockaddr*)&client_addr, &cl_len);
    debug("Connection accepted\n");

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t id;

    client_listener_args_t args = {
        .descriptor = newsock,
        .config = config,
    };

    pthread_create(&id, &attr, server_task_manager_job, &args);
  }
}

void server_listener(config_t* config) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_cond_init(&config->num_tasks_cv, NULL);

  pthread_t thread;
  worker_args_t args = {.config = config, .thread_number = 1000};

  pthread_create(&thread, &attr, server_listener_thread_job, &args);
}

/**************************************************************************
 * Function:    client_job
 *
 * Description: client worker implementation, which contains of following steps:
 *              - connects to server
 *              - downloads task to bruteforce
 *              - bruteforces task, using client-size settings (like thread cnt)
 *              - reports result back (fount or not, and password if found)
 * 
 *              todo: implement "exit" command and interrupt client routine
 *              if exit command was received
 *
 * Inputs:      config_t *config
 *              Pointer to application config
 *
 * Returns:     none
 *
 *************************************************************************/
int client_job(config_t* config) {
  struct sockaddr_in serv_addr;

  int sock = connect_to_server(config->host, config->port, &serv_addr);
  if (sock < 0) {
    return EXIT_FAILURE;
  }

  for (;;) {
    task_t task;
    init_task(&task);

    read_task(sock, config, &task);

    if (config->num_threads > 1) {
      multi_brute(config, &task);
    } else {
      single_brute(config, &task);
    }

    send_result(sock, &config->result);
  }

  return EXIT_SUCCESS;
}