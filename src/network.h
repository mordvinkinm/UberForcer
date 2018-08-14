/**************************************************************************
 *            Cross-platform encapsulation of requried network operation
 *
 *   File    : network.h
 *   Purpose : Encapsulates definitions for cross-platform compilation of
 *             network operations
 *   Author  : Mikhail Mordvinkin
 *   Date    : July 24, 2018
 *
 ***************************************************************************
 *   Due to platform-specific nature of sockets, use "includes" and definitions
 *   specific for target platform
 ***********************************************************************/

#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"
#include "config.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#elif _WIN64
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined __linux__
#error "Linux is not supported yet"
#elif defined __unix__
#error "Unix is not supported yet"
#elif defined TARGET_OS_MAC
#error "Macos is not supported yet"
#else
#error "Unknown target platform"
#endif

/**************************************************************************
 *
 * Description: Structure to pass arguments into a thread worker
 *
 *************************************************************************/
typedef struct client_listener_args_s {
  config_t *config;
  int descriptor;
} client_listener_args_t;

/**************************************************************************
 * Function:    init_network
 *
 * Description: Initializes network operation, making appropriate system calls
 *
 *************************************************************************/
void init_network();

/**************************************************************************
 * Function:    init_server_listener
 *
 * Description: Initializes network part of bruteforcing server, listening
 *              for incoming connection
 *
 * Inputs:      config_t *config
 *              pointer to application config
 *
 * Returns:     socket id or -1 if connection failed
 *
 *************************************************************************/
int init_server_listener(config_t *config);

/**************************************************************************
 * Function:    accept_client_connection
 *
 * Description: accepts client connection to the bruteforcing server
 *
 * Inputs:      int sock
 *              socket_id that listens for incoming connections
 *
 * Returns:     new socket id or -1 if connection failed
 *
 *************************************************************************/
int accept_client_connection(int sock);

/**************************************************************************
 * Function:    connect_to_server
 *
 * Description: Connects client to a bruteforcing server.
 *
 * Inputs:      char *host
 *              Pointer to bruteforcing server host
 *
 *              unsigned short port
 *              Port number. Should be between 1024 and 65535 (inclusive)
 *
 * Returns:     socket id or -1 if connection failed
 *
 *************************************************************************/
int connect_to_server(char *host, unsigned short port, struct sockaddr_in *serv_addr);

/**************************************************************************
 * Function:    send_task
 *
 * Description: sends bruteforcing task to a client over network
 *
 * Inputs:      int sock
 *              Socket ID
 *
 *              config_t *config
 *              Pointer to application config
 *
 *              task_t *task
 *              Pointer to task that will be sent to a client
 *
 * Returns:     exit code - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int send_task(int sock, config_t *config, task_t *task);

/**************************************************************************
 * Function:    read_task
 *
 * Description: receives bruteforcing task from a server over network
 *
 * Inputs:      int sock
 *              Socket ID
 *
 *              config_t *config
 *              Pointer to application config
 *
 *              task_t *task
 *              Pointer to resulting task
 *
 * Returns:     exit code - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int read_task(int sock, config_t *config, task_t *task);

/**************************************************************************
 * Function:    send_result
 *
 * Description: sends bruteforcing task result over network back to server
 *
 * Inputs:      int sock
 *              Socket ID
 *
 *              result_t *result
 *              Pointer to resulting task
 *
 * Returns:     exit code - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int send_result(int sock, result_t *result);

/**************************************************************************
 * Function:    read_result
 *
 * Description: reads bruteforcing task that was sent to server over network
 *
 * Inputs:      int sock
 *              Socket ID
 *
 *              result_t *result
 *              Pointer to resulting task
 *
 * Returns:     exit code - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int read_result(int sock, result_t *result);

#endif /* NETWORK_H */