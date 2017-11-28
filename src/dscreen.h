/*
 * The Dynamic Screen data structure and related functions
 *
 */

#ifndef _DSCREEN_H
#define _DSCREEN_H


#include <stdio.h>
#include <stdint.h>


/* DYNAMIC SCREEN:
    a backbuffer for the screen,
    look up table for if a pixel
    can be drawn */
typedef struct {
    uint8_t (**pixels)[3];  // pixels on the screen
    int height, width;
} DScreen;

/* SCREEN:
    struct to hold framebuffer
    screen info */
struct Screen {
    struct fb_fix_screeninfo* finfo;
    struct fb_var_screeninfo* vinfo;
    int fd;
    long size;

    uint8_t *fbp;
    DScreen *dscr;
};



void dscreen_add_pixel (DScreen *scr, int x, int y, uint8_t pixel[3]);


#endif

