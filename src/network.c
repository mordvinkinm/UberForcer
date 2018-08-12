#include <errno.h>
#include <stdlib.h>

#include "network.h"

/**************************************************************************
 * Function:    init_network
 *
 * Description: Initializes network operation, making appropriate calls:
 *
 *              Windows (x86, x64): call WSAStartup function
 *              Unix (x86, x64): nothing specific
 *
 *************************************************************************/
void init_network() {
#ifdef _WIN32
  WSADATA wsaData;
  WSAStartup(0x202, &wsaData);

  debug("Called WSAStartup to init network in Windows environment\n");
#elif _WIN64
  WSADATA wsaData;
  WSAStartup(0x202, &wsaData);
  debug("Called WSAStartup to init network in Windows environment\n");
#endif
}

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
int connect_to_server(char *host, unsigned short port, struct sockaddr_in *serv_addr) {
  serv_addr->sin_family = AF_INET;
  serv_addr->sin_port = htons(port);

  struct hostent *server = gethostbyname(host);
  memmove((char *)&serv_addr->sin_addr.s_addr, (char *)server->h_addr, server->h_length);

  debug("Establish connect to %s\n", host);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
    return -1;
  }

  if (connect(sock, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) {
    fprintf(stderr, "Error connecting to %s\n", host);
    return -1;
  }

  debug("Connected to %s\n", host);

  return sock;
}