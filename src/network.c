#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "network.h"
#include "protocol.h"

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
 * Function:    init_server_listener
 *
 * Description: Initializes network part of bruteforcing server, listening
 *              for incoming connection
 *
 * Inputs:      config_t *config
 *              Pointer to application config
 *
 * Returns:     socket id or -1 if connection failed
 *
 *************************************************************************/
int init_server_listener(config_t *config) {
  struct sockaddr_in serv_addr;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    fprintf(stderr, "Error opening socket: %d (%s)\n", errno, strerror(errno));
    return sock;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(config->port);

  if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    fprintf(stderr, "Error binding socket: %d (%s)\n", errno, strerror(errno));
    return sock;
  }

  listen(sock, 10);

  return sock;
}

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
int accept_client_connection(int sock) {
  struct sockaddr_in client_addr;
  socklen_t cl_len = sizeof(client_addr);

  int newsock = accept(sock, (struct sockaddr *)&client_addr, &cl_len);
  if (newsock < 0) {
    fprintf(stderr, "Couldn't accept client connection: %s\n", strerror(errno));
    return newsock;
  }

  return newsock;
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

/**************************************************************************
 * Function:    send_task
 *
 * Description: sends bruteforcing task to a client over network.
 *              Operation performed in three stages:
 *              - Serializes message into buffer of "big" size
 *              - Writes message size into socket
 *              - Writes message itself into socket
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
int send_task(int sock, config_t *config, task_t *task) {
  trace("Preparing sending task to client: password %s, from: %d, to: %d\n", task->password, task->from, task->to);

  const unsigned int buf_size = 512;
  char buf[buf_size];
  memset(buf, 0, buf_size);

  sprintf(buf, MSG_SEND_JOB, task->password, config->hash, config->alphabet, task->from, task->to);

  uint16_t msg_size = strlen(buf);
  uint16_t network_msg_size = htons(msg_size);

  if (send(sock, (char *)&network_msg_size, sizeof(network_msg_size), 0) < 0) {
    fprintf(stderr, "Error writing task size to socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  if (send(sock, buf, msg_size, 0) < 0) {
    fprintf(stderr, "Error writing task to socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  trace("Message of size %d sent to client:\n%s\n", msg_size, buf);

  return EXIT_SUCCESS;
}

/**************************************************************************
 * Function:    read_task
 *
 * Description: receives bruteforcing task from a server over network.
 *              Operation performed in three stages:
 *              - Reads size from socket
 *              - Allocates buffer of needed size and reads message from socket
 *              - Deserializes message into output parameters
 *
 * Inputs:      int sock
 *              Socket ID
 *
 *              config_t *config
 *              Pointer to application config
 *
 *              task_t *task
 *              Pointer to resulting task. You can skip struct initialization
 *              before function call as it's done inside of method.
 *
 * Returns:     exit code - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int read_task(int sock, config_t *config, task_t *task) {
  // Read task message size
  uint16_t network_msg_size;
  if (recv(sock, (char *)&network_msg_size, sizeof(network_msg_size), 0) < 0) {
    fprintf(stderr, "Error reading message size from socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Convert network byte order to app byte order
  uint16_t msg_size = ntohs(network_msg_size);
  trace("Prepare to receive message from server of size: %d\n", msg_size);

  // Allocate buffer of appropriate size and read task message
  char *buf = malloc(sizeof(char) * msg_size);
  memset(buf, 0, msg_size);

  if (recv(sock, buf, msg_size, 0) < 0) {
    fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  trace("Message received from server:\n%s\n", buf);

  // Deserialize task
  memset(&task->password, 0, sizeof(task->password));
  sscanf(buf, MSG_SEND_JOB, &task->password, config->hash, config->alphabet, &task->from, &task->to);

  trace("Password: %s, Hash: %s, Alphabet: %s, from: %d, to: %d\n", task->password, config->hash, config->alphabet, task->from, task->to);

  return EXIT_SUCCESS;
}

/**************************************************************************
 * Function:    send_result
 *
 * Description: sends bruteforcing task result over network back to server.
 *              Operation performed in three stages:
 *              - Serializes message into allocated buffer of "big" size
 *              - Writes message size into socket
 *              - Writes message itself into socket
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
int send_result(int sock, result_t *result) {
  const int BUF_SIZE = 255;

  // Define buffer and zero it
  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE);

  // Writes message to buffer
  sprintf(buf, MSG_REPORT_RESULT, result->password, result->found);

  uint16_t actual_size = (uint16_t)strlen(buf);
  uint16_t network_actual_size = htons(actual_size);

  if (send(sock, (char *)&network_actual_size, sizeof(network_actual_size), 0) < 0) {
    fprintf(stderr, "Error sending message size: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  if (send(sock, buf, actual_size, 0) < 0) {
    fprintf(stderr, "Error writing to socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  trace("Message sent to server:\n%s\n", buf);

  return EXIT_SUCCESS;
}

/**************************************************************************
 * Function:    read_result
 *
 * Description: reads bruteforcing task that was sent to server over network.
 *              Opertion performed in three phases:
 *              - Reads message size
 *              - Allocates buffer of proper size and reads message from socket
 *              - Reads message itself
 *
 * Inputs:      int sock
 *              Socket ID
 *
 *              result_t *result
 *              Pointer to resulting task. You can skip struct initialization
 *              before function call as it's done inside the method.
 *
 * Returns:     exit code - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
 *
 *************************************************************************/
int read_result(int sock, result_t *result) {
  uint16_t network_buf_size;
  if (recv(sock, (char *)&network_buf_size, sizeof(network_buf_size), 0) < 0) {
    fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  uint16_t buf_size = ntohs(network_buf_size);
  char buf[buf_size];
  memset(buf, 0, buf_size);

  if (recv(sock, buf, buf_size, 0) < 0) {
    fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  trace("Message received from client:\n%s\n", buf);

  sscanf(buf, MSG_REPORT_RESULT, result->password, &result->found);

  return EXIT_SUCCESS;
}