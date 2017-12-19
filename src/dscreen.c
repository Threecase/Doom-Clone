/*
 * The Dynamic Screen data structure and related functions.
 *
 */

#include <stdio.h>
#include <time.h>

#include "dscreen.h"


/* dscreen_add_pixel: attempt to add a pixel to the dscreen */
int dscreen_add_pixel (DScreen *scr, long x, long y, uint8_t pixel[3]) {

    if (x < 0 || y < 0 || x >= scr->width || y >= scr->height
    || (!scr->pixels[0] && !scr->pixels[1] && !scr->pixels[2]))
        return 1;

    for (int i = 0; i < 3; ++i)
        scr->pixels[x][y][i] = pixel[i];

    return 0;
}

