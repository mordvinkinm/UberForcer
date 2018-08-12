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
  config_t* config;
  int descriptor;
} client_listener_args_t;

/**************************************************************************
 * Function:    init_network
 *
 * Description: Initializes network operation, making appropriate system calls
 *
 *************************************************************************/
void init_network();

#endif /* NETWORK_H */