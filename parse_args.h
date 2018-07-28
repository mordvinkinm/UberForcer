#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "config.h"


/**************************************************************************
* Function:    parse_args
*
* Description: Parses required arguments to run program:
*              run mode and required parameters for the selected app mode.
*              
* Inputs:      int argc
*              Number of arguments from command line (argument from main func)
*
*              *argv[]
*              Array of string parameters (argument from main func)
*
*              config_t *config
*              Pointer to application config
*
* Returns:     int (exit status) - EXIT_SUCCESS (0) or EXIT_FAILURE (1)
* 
*************************************************************************/
int parse_args(int argc, char *argv[], config_t *config);


#endif /* PARSE_ARGS_H */