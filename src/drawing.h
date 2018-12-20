/*
 * Drawing utilities
 *
 */

#ifndef __DRAWING_H
#define __DRAWING_H

#include "data_types.h"
#include "3D_manipulations.h"
#include "error.h"
#include "wad_info.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


typedef struct Colour
{   uint8_t r, g, b, a;
} Colour;

struct Screen
{   Colour **pixels;
    unsigned long width, height;
} backbuffer;

enum DRAWMODE
{   DRAWMODE_2D,
    DRAWMODE_3D,
} DRAWMODE;

enum DRAWFLAGS
{
    FLAG_FULLSCREEN = 0x01,
};



// FIXME move these
int angle;
Point player_pos;


Colour PALETTE[14][256];

unsigned SCREEN_WIDTH, SCREEN_HEIGHT;



void render_ssector (SSector ssec);
void render_ssector_2D (SSector ssec);

void draw_frame(void);
void draw_pixel(unsigned long x, unsigned long y, Colour c);

void init_palette(void);
void init_video (unsigned long w, unsigned long h, uint32_t flags);
void shutdown_video(void);


#endif

