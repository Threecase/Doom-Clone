/*
 * Error handling
 *
 */

#include "error.h"
#include "input.h"
#include "drawing.h"
#include "DOOM.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


void DOOM_print  (FILE *const stream, char *const msg, ...);
void DOOM_vprint (FILE *const stream, char *const msg, va_list ap);



/* DOOM_debug: print a debug message */
void DOOM_debug (char *const func, char *const msg, ...)
{
    va_list ap;
    va_start (ap, msg);

    DOOM_print  (stderr, "Debug: %s -- ", func);
    DOOM_vprint (stderr, msg, ap);
    DOOM_print  (stderr, "\n");

    va_end (ap);
}

/* DOOM_fatal_error: print an error message and exit */
void DOOM_fatal_error (char *const func, char *const msg, ...)
{
    va_list ap;
    va_start (ap, msg);

    DOOM_print  (stderr, "Error: %s -- ", func);
    DOOM_vprint (stderr, msg, ap);
    DOOM_print  (stderr, "\n");

    va_end (ap);

    DOOM_exit (1);
}



/* DOOM_print: printf-style wrapper */
void DOOM_print (FILE *const stream, char *const msg, ...)
{
    va_list ap;
    va_start (ap, msg);

    DOOM_vprint (stream, msg, ap);

    va_end (ap);
}

/* DOOM_vprint: vprintf-style wrapper */
void DOOM_vprint (FILE *const stream, char *const msg, va_list ap)
{
    vfprintf (stream, msg, ap);
}
