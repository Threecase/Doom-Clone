/*
 * Terminal raw mode (ie no newline needed for input)
 *
 */

#ifndef _RAWTERM_H
#define _RAWTERM_H


#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


struct termios def_term;


int term_raw();

void term_noraw();

int raw_input();

void raw_writec (char c);

void raw_writes (char *str);


#endif

