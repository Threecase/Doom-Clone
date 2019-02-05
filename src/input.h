/*
 * Terminal raw mode (ie no newline needed for input)
 *
 */

#ifndef __INPUT_H
#define __INPUT_H



/* Event:
 *  user input events, etc.
 */
typedef struct
{
    enum
    {   EVENT_NONE,
        EVENT_KEYPRESS,
        EVENT_KEYRELEASE,
        EVENT_MOUSEMOTION,
    } type;

    union
    {   char key;
        struct
        {   int dx,
                dy;
        } motion;
    };
} Event;

static Event const NULL_EVENT = { EVENT_NONE, { 0 } };


void init_input(void);
void shutdown_input(void);
void print (char *format, ...);
Event input(void);


#endif

