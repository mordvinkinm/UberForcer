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