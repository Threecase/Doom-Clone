/*
 * Drawing utilities
 *
 */

#ifndef _DRAWING_H
#define _DRAWING_H


#ifndef __SDL

#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#else
#include <SDL2/SDL.h>
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "dscreen.h"
#include "data_types.h"
#include "3D_manipulations.h"
#include "drawing_2.h"
#include "error.h"
#include "wad_info.h"


#define DRAW_MODE_3D    0
#define DRAW_MODE_2D    1


// VARIABLES
// FIXME move these
int angle;
Point player_pos;


char DRAW_MODE;

char (*PALETTE)[3];

struct Screen G_SCREEN;

char G_VIDEO_INIT;

unsigned SCREEN_WIDTH, SCREEN_HEIGHT;


// FUNCTIONS
void render_ssector (SSector ssec);
void render_ssector_2D (SSector ssec);

void draw_render();

void draw_pixel (uint8_t pixel[3], long x, long y);

void init_video();

void shutdown_video();


#endif

