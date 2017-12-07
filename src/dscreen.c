/*
 * The Dynamic Screen data structure and related functions.
 *
 */

#include <stdio.h>

#include "dscreen.h"


/* dscreen_add_pixel: attempt to add a pixel to the dscreen */
void dscreen_add_pixel (DScreen *scr, int x, int y, uint8_t pixel[3]) {

    /*if (scr->pixels[x][y][0] == 0 && scr->pixels[x][y][1] == 0
     && scr->pixels[x][y][2] == 0)*/
        for (int i = 0; i < 3; ++i)
            scr->pixels[x][y][i] = pixel[i];
}

