/*
 * Drawing utilities
 *
 */

#warning DEPRECATED
#include "drawing.h"



/* render_ssector: draw a subsector to the screen */
// TODO : IMPROVE !!!
void render_ssector (SSector ssec) {

    Point pnt;
    int outx, outy, seg_n, corner;
    uint8_t col[ssec.num_segs][3];

    long pixels_to_draw[ssec.num_segs][4][2];
    memset (pixels_to_draw, 0, ssec.num_segs * 8);

    /* loop through lines in N, translate to 3D, and
        attempt to draw the polygon to the screen */
    seg_n = 0;
    for (int i = ssec.start_seg; i < ssec.start_seg + ssec.num_segs; ++i, ++seg_n) {
        corner = 0;
        // TODO : make this less messy?
        Sector sec = SECTOR_LIST[SIDEDEF_LIST[LINEDEF_LIST[SEG_LIST[
                        ssec.start_seg].line].sides[0]].sector];

        for (int y = sec.floor_h, n = 0; n < 2; ++n, y = sec.ceil_h) {

            // start vertex
            pnt.x = VERT_LIST[SEG_LIST[i].start].x;
            pnt.y = y;
            pnt.z = VERT_LIST[SEG_LIST[i].start].z;
            outx = outy = -1;
            if (coords_3D_to_2D (pnt, player_pos, angle, &outx, &outy)) {
                // add to drawbuffer
                pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
            }
            else {
                memset (pixels_to_draw[seg_n], 0, sizeof(long) * 8);
                break;
            }

            // end vertex
            pnt.x = VERT_LIST[SEG_LIST[i].end].x;
            pnt.y = y;
            pnt.z = VERT_LIST[SEG_LIST[i].end].z;
            outx = outy = -1;
            if (coords_3D_to_2D (pnt, player_pos, angle, &outx, &outy)) {
                // add to drawbuffer
                pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
            }
            else {
                memset (pixels_to_draw[seg_n], 0, sizeof(long) * 8);
                break;
            }
            col[seg_n][0] = pnt.x % 255;
            col[seg_n][1] = 127;
            col[seg_n][2] = pnt.z % 255;
        }
    }
    for (int j = 0, draw = 0; j < ssec.num_segs; ++j, draw = 0) {
        for (int i = 0; !draw && i < 4; ++i)
            if (pixels_to_draw[j][i][0] != 0 || pixels_to_draw[j][i][1] != 0)
                draw = 1;
        if (draw)
        {   fill_poly (pixels_to_draw[j], col[j]);
        }
    }
}

/* init_palette: initialize graphics */
void init_palette(void)
{
    /* there are 14 palettes, which are 768 bytes long each */
    uint8_t *bytes = malloc (14 * 768);
    if (PALETTE == NULL)
    {   fatal_error ("init_video: failed to alloc palette!");
    }

    for (int p = 0; p < 14; ++p)
    {
        for (int c = 0; c < 256; c += 3)
        {
            int ptr = (p*256) + c;
            Colour col = { bytes[ptr],
                           bytes[ptr+1],
                           bytes[ptr+2],
                           255 };
            PALETTE[p][c] = col;
        }
    }

    int i = get_lump_index ("PLAYPAL");
    if (i < 0)
        fatal_error ("WAD missing PLAYPAL lump!");
    read_lump (i, PALETTE);
}

