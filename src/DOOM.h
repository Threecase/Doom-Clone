/*
 * DOOM functions
 *
 */

#ifndef __DOOM_H
#define __DOOM_H


#include "drawing.h"

#include <stdint.h>


#define PI          3.14159265
#define RADS(deg)   ((deg * PI) / 180.0)
#define DEGS(rad)   ((rad * 180.0) / PI)


/* game:
 *  global stuff about the game
 */
struct
{   struct
    {   int width, height;
        uint32_t flags;
    } window;

    char *wadname;
    enum
    {   DRAWMODE_2D,
        DRAWMODE_3D,
    } drawmode;

    float map_scale;
    float map_scale_delta;
    float FOV;

} game;

/* player:
 *  stuff about the player
 */
struct
{
    Point pos;
    Point vector;
    float angle;
    float angle_vector;

    float speed;
    float rot_speed;
    float mouse_sensitivity;
} player;



void handle_arguments (int argc, char *argv[]);
void DOOM_init(void);
void DOOM_start (void (*update_function)(int));

void DOOM_update_and_render (int n);
void DOOM_get_input(void);
void DOOM_update (void);
void DOOM_render (void);

void DOOM_exit (int status);




/* FIXME: where should these be declared? */
void call_in_1_60th_of_a_second (void (*fn)(int));
void start_mainloop(void);

/* FIXME: TEMP */
void set_palette (Colour palette[256]);


#endif

