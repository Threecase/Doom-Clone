/*
 * Drawing utilities
 *
 */

#include "drawing.h"
#include "rawterm.h"



/* draw_line_2D: draw a 2D line */
void draw_line_2D (uint8_t col[3], int start[2], int end[2]) {

    int x, y;
    int deltax = end[0] - start[0];
    int deltay = end[1] - start[1];
    int run1, rise1, run2, rise2;
    int longest, shortest, numerator;

    run1 = run2 = (deltax < 0)? -1 : 1;
    rise1 = (deltay < 0)? -1 : 1;
    rise2 = 0;

    longest = abs (deltax);
    shortest = abs (deltay);

    if (longest < shortest) {
        longest = abs (deltay);
        shortest = abs (deltax);

        rise2 = (deltay < 0)? -1 : 1;
        run2 = 0;
    }

    x = start[0];
    y = start[1];

    numerator = longest / 2;
    for (int i = 0; i <= longest && i <= SCREEN_WIDTH; ++i) {
    
        dscreen_add_pixel (G_SCREEN.dscr, x, y, col);

        numerator += shortest;
        if (numerator >= longest) {
            numerator -= longest;
            x += run1;
            y += rise1;
        }
        else {
            x += run2;
            y += rise2;
        }
    }
}

/* render_ssector: draw a subsector to the screen (2D version) */
void render_ssector_2D (SSector ssec) {

//    printf ("%i %i\n", player_pos.x, player_pos.z);

    uint8_t col[3];
    int start[2], end[2];

    col[0] = col[1] = col[2] = 255;

    /* loop through lines in N, draw them to the screen */
    for (int i = ssec.start_seg; i < ssec.start_seg + ssec.num_segs; ++i) {

        start[0] = VERT_LIST[SEG_LIST[i].start].x - player_pos.x;
        start[1] = VERT_LIST[SEG_LIST[i].start].z + player_pos.z;
        end[0] = VERT_LIST[SEG_LIST[i].end].x - player_pos.x;
        end[1] = VERT_LIST[SEG_LIST[i].end].z + player_pos.z;

        rotate_on_axis (start[0], start[1], angle, start, start+1);
        rotate_on_axis (end[0], end[1], angle, end, end+1);

        start[0] += SCREEN_WIDTH / 2;
        start[1] += SCREEN_HEIGHT / 2;
        end[0] += SCREEN_WIDTH / 2;
        end[1] += SCREEN_HEIGHT / 2;

        draw_line_2D (col, start, end);
//        raw_writes ("%i %i\n\r", start[0], start[1]);
    }

    // player dot
    for (int i = -1; i < 2; ++i) {
        dscreen_add_pixel (G_SCREEN.dscr, i + SCREEN_WIDTH / 2, -1 + SCREEN_HEIGHT / 2, (uint8_t [3]){ 255,0,0 });
        dscreen_add_pixel (G_SCREEN.dscr, i + SCREEN_WIDTH / 2, 0 + SCREEN_HEIGHT / 2, (uint8_t [3]){ 255,0,0 });
        dscreen_add_pixel (G_SCREEN.dscr, i + SCREEN_WIDTH / 2, 1 + SCREEN_HEIGHT / 2, (uint8_t [3]){ 255,0,0 });
    }
}

