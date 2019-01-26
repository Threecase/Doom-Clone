/*
 * Terminal raw mode (ie no newline needed for input)
 *
 */

#ifndef __INPUT_H
#define __INPUT_H



void init_input(void);
void shutdown_input(void);
void print (char *format, ...);
int input(void);


#endif

