/*
 * Error handling
 *
 */

#ifndef _ERROR_H
#define _ERROR_H


#define debug(msg, ...)         ({ DOOM_debug       ((char *const)__func__, msg, ##__VA_ARGS__); })
#define fatal_error(msg, ...)   ({ DOOM_fatal_error ((char *const)__func__, msg, ##__VA_ARGS__); })



void DOOM_debug       (char *const func, char *const msg, ...);
void DOOM_fatal_error (char *const func, char *const msg, ...);


#endif

