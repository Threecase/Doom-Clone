/*
 * Error handling
 *
 */

#include "error.h"
#include "input.h"
#include "drawing.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>



/* DOOM_debug: print a debug message */
void DOOM_debug (char const *func, char const *msg, ...)
{

    va_list ap;
    va_start (ap, msg);

    fprintf (stderr, "Debug: %s -- ", func);
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");

    va_end (ap);
}

/* fatal_error: print an error message and exit */
void fatal_error (char const *msg, ...)
{
    va_list ap;
    va_start (ap, msg);

    shutdown_input();

    fprintf (stderr, "Error: ");
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");

    va_end (ap);

    shutdown_video();
    exit (-1);
}

