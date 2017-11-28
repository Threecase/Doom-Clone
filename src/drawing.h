/*
 * Drawing utilities
 *
 */

#ifndef _DRAWING_H
#define _DRAWING_H


#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "dscreen.h"
#include "data_types.h"
#include "3D_manipulations.h"


struct Screen G_SCREEN;

char G_VIDEO_INIT;

int SCREEN_WIDTH, SCREEN_HEIGHT;


void render (BSP_Node node_to_render);

void draw_render();

void draw_pixel (uint8_t pixel[3], long x, long y);

void init_video();

void shutdown_video();


#endif

