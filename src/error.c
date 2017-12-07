/*
 * Error handling
 *
 */

#include "error.h"


/* fatal_error: print an error message and exit */
void fatal_error (char *msg, ...) {

    va_list ap;
    va_start (ap, msg);

    fprintf (stderr, "Error: ");
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");

    va_end (ap);

    shutdown_video();
    exit (-1);
}

