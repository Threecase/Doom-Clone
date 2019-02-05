/*
 * Drawing utilities
 *
 */

#include "drawing.h"
#include "error.h"
#include "wad_info.h"

#include "DOOM.h"   /* FIXME: temp */

#include <string.h>
#include <stdlib.h>



/* init_palette: initialize graphics */
void init_palette(void)
{
    uint8_t *bytes = read_lump ("PLAYPAL");

    /* there are 14 palettes, which are 768 bytes long each */
    for (int p = 0; p < 14; ++p)
    {
        for (int c = 0; c < 256; c++)
        {
            int ptr = (p * 256) + (c*3);
            PALETTE[p][c].r = bytes[ptr+0];
            PALETTE[p][c].g = bytes[ptr+1];
            PALETTE[p][c].b = bytes[ptr+2];
            PALETTE[p][c].a = 255;
        }
    }
    free (bytes);
}

