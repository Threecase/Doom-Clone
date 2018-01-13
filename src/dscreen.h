/*
 * The Dynamic Screen data structure and related functions
 *
 */

#ifndef _DSCREEN_H
#define _DSCREEN_H


#include <stdio.h>
#include <stdint.h>

#ifdef __SDL
#include <SDL2/SDL.h>
#endif


/* DYNAMIC SCREEN:
    a backbuffer for the screen,
    look up table for if a pixel
    can be drawn */
typedef struct {
    uint8_t (**pixels)[3];  // pixels on the screen
    int height, width;
} DScreen;

/* SCREEN:
    struct to hold
    screen info */
struct Screen {
#ifndef __SDL
    struct fb_fix_screeninfo *finfo;
    struct fb_var_screeninfo *vinfo;
    int fd;
    long size;

    uint8_t *fbp;
#else
    SDL_Renderer *renderer;
    SDL_Window *window;
#endif
    DScreen *dscr;
};



int dscreen_add_pixel (DScreen *scr, long x, long y, uint8_t pixel[3]);


#endif

