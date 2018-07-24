#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#define bool short int
#define true 1
#define false 0

#define DEBUG_LOG 1
#define DEBUG_TRACE 2

// Define debug method
#if DEBUG >= DEBUG_LOG
    #define debug(message, ...) printf(message __VA_OPT__(,) __VA_ARGS__)
#else
    #define debug(message, ...)
#endif

#if DEBUG >= DEBUG_TRACE
    #define trace(message, ...) printf(message __VA_OPT__(,) __VA_ARGS__)
#else
    #define trace(message, ...)
#endif 


#endif /* COMMON_H */