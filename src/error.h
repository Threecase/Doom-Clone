/*
 * Error handling
 *
 */

#ifndef _ERROR_H
#define _ERROR_H


#define debug(msg, ...)    ({ DOOM_debug (__func__, msg, ##__VA_ARGS__); })



void DOOM_debug (char const *func, char const *msg, ...);
void fatal_error (char const *msg, ...);


#endif

