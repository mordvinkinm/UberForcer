#ifndef COMMON_H
#define COMMON_H

#define bool short int
#define true 1
#define false 0

// Define debug method
#ifdef DEBUG
#define debug(message, ...) printf(message __VA_OPT__(,) __VA_ARGS__)
#endif

#ifndef DEBUG
#define debug(message, ...)
#endif

#endif /* COMMON_H */