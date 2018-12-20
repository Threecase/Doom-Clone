/*
 * Terminal raw mode (ie no newline needed for input)
 *
 */

#ifndef __RAWTERM_H
#define __RAWTERM_H



void init_input(void);
void shutdown_input(void);
void print (char *format, ...);
int input(void);


#endif

