/**************************************************************************
 *            Common data structures, constants and syntax sugar
 *
 *   File    : common.h
 *   Author  : Mikhail Mordvinkin
 *   Date    : July 24, 2018
 *
 **************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

// Define "bool" data type and true/false constants
#define bool short int
#define true 1
#define false 0

// Debug level - print debug messages, except of trace. Compile app with flag -DDEBUG=1  to enable DEBUG logging
#define DEBUG_LOG 1
// Debug level - print everything, even verbose. Compile app with flag -DDEBUG=2 to enable TRACE logging
#define DEBUG_TRACE 2

// If debug level set to DEBUG, then define debug function as an alias for printf() function
// Otherwise, don't even include function invocation into comppiled code (for performance purpose)
#if DEBUG >= DEBUG_LOG
    #define debug(message, ...) printf(message __VA_OPT__(, ) __VA_ARGS__)
#else
    #define debug(message, ...)
#endif

// If debug level set to TRACE, then define trace function as an alias for printf() function
// Otherwise, don't even include function invocation into comppiled code (for performance purpose)
#if DEBUG >= DEBUG_TRACE
    #define trace(message, ...) printf(message __VA_OPT__(, ) __VA_ARGS__)
#else
    #define trace(message, ...)
#endif

#endif /* COMMON_H */