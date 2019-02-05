/*
 * Drawing utilities
 *
 */

#include "drawing.h"
#include "wad_info.h"   /* for VERT_LIST */

#include <stdlib.h>



/* draw_line_2D: draw a 2D line */
void draw_line_2D (Colour col, int start[2], int end[2])
{
    int x = 0,
        y = 0;

    int deltax = end[0] - start[0],
        deltay = end[1] - start[1];

    int run1  = 0,
        rise1 = 0,
        run2  = 0,
        rise2 = 0;

    int longest   = 0,
        shortest  = 0,
        numerator = 0;


    run1 = run2 = (deltax < 0)? -1 : 1;
    rise1 = (deltay < 0)? -1 : 1;
    rise2 = 0;

    longest = abs (deltax);
    shortest = abs (deltay);

    if (longest < shortest)
    {
        longest = abs (deltay);
        shortest = abs (deltax);

        rise2 = (deltay < 0)? -1 : 1;
        run2 = 0;
    }

    x = start[0];
    y = start[1];

    numerator = longest / 2;
    for (int i = 0; i <= longest && i <= SCREEN_WIDTH; ++i)
    {
        draw_pixel (x,y, col);

        numerator += shortest;
        if (numerator >= longest)
        {   numerator -= longest;
            x += run1;
            y += rise1;
        }
        else
        {   x += run2;
            y += rise2;
        }
    }
}

/* render_ssector: draw a subsector to the screen (2D version) */
void render_ssector_2D (SSector ssec, Level *level)
{
//    printf ("%i %i\n", player_pos.x, player_pos.z);

    Colour col;
    int start[2], end[2];

    col.r = col.g = col.b = col.a = 255;

    /* loop through lines in N, draw them to the screen */
    for (int i = ssec.start_seg; i < ssec.start_seg + ssec.num_segs; ++i)
    {
        start[0] = level->vertices[level->segs[i].start].x - player_pos.x;
        start[1] = level->vertices[level->segs[i].start].y + player_pos.y;
        end[0]   = level->vertices[level->segs[i].end].x - player_pos.x;
        end[1]   = level->vertices[level->segs[i].end].y + player_pos.y;

        rotate_on_axis (start[0], start[1], angle, start, start+1);
        rotate_on_axis (end[0], end[1], angle, end, end+1);

        start[0] += SCREEN_WIDTH / 2;
        start[1] += SCREEN_HEIGHT / 2;
        end[0] += SCREEN_WIDTH / 2;
        end[1] += SCREEN_HEIGHT / 2;

        draw_line_2D (col, start, end);
//        raw_writes ("%i %i\n\r", start[0], start[1]);
    }

    /* player dot */
    for (int i = -1; i < 2; ++i)
    {
        draw_pixel (i + SCREEN_WIDTH / 2, -1 + SCREEN_HEIGHT / 2,
                    (Colour){ 255,0,0,0 });
        draw_pixel (i + SCREEN_WIDTH / 2, 0 + SCREEN_HEIGHT / 2,
                    (Colour){ 255,0,0,0 });
        draw_pixel (i + SCREEN_WIDTH / 2, 1 + SCREEN_HEIGHT / 2,
                    (Colour){ 255,0,0,0 });
    }
}

