/*
 * The Dynamic Screen data structure and related functions.
 *
 */

#include <stdio.h>

#include "DScreen.h"



/* render: draw the pixels in pixels_to_draw to the screen,
    if they are available in the scanline */
void render_line (char *pixels_to_draw, Scanline scanline) {

    for (int i = 0; pixels_to_draw[i] != NULL; ++i)
        if (is_available (scanline.pixels[i]))
            draw_pixel (scanline.pixels[i], pixels_to_draw[i]);

}

