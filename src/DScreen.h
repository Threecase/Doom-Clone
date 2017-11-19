/*
 * The Dynamic Screen data structure and related functions
 *
 */

#ifndef _DSCREEN_H
#define _DSCREEN_H


#include <stdio.h>


typedef struct Scanline {
    char *pixels;           // pixels
    struct Scanline *next;  // next scanline block
} Scanline;

typedef struct {
    Scanline *lines;        // list of scanlines
} DScreen;


void render_line (char *pixels_to_draw, Scanline scanline);


#endif

