/*
 * Drawing utilities
 *
 */

#ifndef __DRAWING_H
#define __DRAWING_H

#include "data_types.h"
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

/* DRAWFLAGS:
 *  Flags for the renderer; like are
 *  we drawing fullscreen or not
 */
enum
{   FLAG_FULLSCREEN = 0x01,
} DRAWFLAGS;



Colour PALETTE[14][256];

unsigned SCREEN_WIDTH, SCREEN_HEIGHT;



void render_ssector (Level *level, uint16_t ssector_number);
void render_ssector_2D (Level *level, uint16_t ssector_number);

void draw_frame(void);
void draw_pixel(unsigned long x, unsigned long y, Colour c);

void init_palette(void);
void init_video(void);
void shutdown_video(void);


#endif

