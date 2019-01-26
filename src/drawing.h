/*
 * Drawing utilities
 *
 */

#ifndef __DRAWING_H
#define __DRAWING_H

#include "data_types.h"
#include "3D_manipulations.h"
#include "wad_info.h"

#include <stdint.h>


/* Colour:
 *  struct for colours
 */
typedef struct Colour
{   uint8_t r, g, b, a;
} Colour;

/* Screen:
 *  Screen stuff
 */
struct Screen
{   Colour **pixels;
    unsigned long width, height;
} backbuffer;


/* DRAWMODE:
 *  Whether we're rendering in 3D
 *  or 2D (ie for automap)
 */
enum
{   DRAWMODE_2D,
    DRAWMODE_3D,
} DRAWMODE;

/* DRAWFLAGS:
 *  Flags for the renderer; like are
 *  we drawing fullscreen or not
 */
enum
{   FLAG_FULLSCREEN = 0x01,
} DRAWFLAGS;



/* FIXME: move these */
int angle;
Point player_pos;


Colour PALETTE[14][256];

unsigned SCREEN_WIDTH, SCREEN_HEIGHT;



void render_ssector (SSector ssec, Level *level);
void render_ssector_2D (SSector ssec, Level *level);

void draw_frame(void);
void draw_pixel(unsigned long x, unsigned long y, Colour c);

void init_palette(void);
void init_video (unsigned long w, unsigned long h, uint32_t flags);
void shutdown_video(void);


#endif

