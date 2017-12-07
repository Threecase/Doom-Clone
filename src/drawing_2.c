/*
 * High-level drawing functions
 * (ie polygon fill, etc.)
 *
 */

#include "drawing_2.h"


/* get_edge: get all the pixels along the line
    x1/y1 to x2/y2 (ie left[] to right[])
    (the values are stored in edge_table) */
void get_edge (long left[2], long right[2], long (*edge_table)[2]) {

    /*  iterate down a Bresenham line,
        add each point to the edge
        */

    int x, y;
    int deltax = right[0] - left[0];
    int deltay = right[1] - left[1];
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

    x = left[0];
    y = left[1];

    numerator = longest / 2;
    for (int i = 0; i <= longest; ++i) {
    
        edge_table[i][0] = x;
        edge_table[i][1] = y;

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

/* sort vertices: sort the supplied vertices in the order:
    0 - top left
    1 - bottom left
    2 - top right
    3 - bottom right  */
void sort_vertices (long coords[4][2], long **sort) {

    /* sort the coords */
    char s_i, swap;
    
    sort[0][0] = SCREEN_WIDTH + 1;
    sort[0][1] = SCREEN_HEIGHT + 1;
    sort[1][0] = SCREEN_WIDTH + 1;
    sort[1][1] = SCREEN_HEIGHT + 1;
    sort[2][0] = -1;
    sort[2][1] = SCREEN_HEIGHT + 1;
    sort[3][0] = -1;
    sort[3][1] = -1;


    for (int i = 0, swap = 1; swap || i > 0; i = (i + 1) % 4) {
        swap = 0;
        s_i = -1;
        // left
        if (coords[i][0] <= sort[0][0] || coords[i][0] <= sort[1][0]) {
            // top
            if (coords[i][0] <= sort[0][0] && coords[i][1] <= sort[0][1])
                s_i = 0;
            // bottom
            else
                s_i = 1;
        }
        // right
        else {
            // top
            if (coords[i][0] >= sort[1][0] && coords[i][1] <= sort[2][1])
                s_i = 2;
            // bottom
            else
                s_i = 3;
        }
        if (s_i != -1 && memcmp (coords[i], sort[s_i], sizeof(long) * 2)) {
            sort[s_i][0] = coords[i][0];
            sort[s_i][1] = coords[i][1];
            swap = 1;
            i = -1;
        }
    }
}

/* fill_poly: fill the polygon */
/* NOTE : coords
    0 : top left
    1 : bottom left
    2 : top right
    3 : bottom right */
void fill_poly (long coords[4][2], uint8_t col[3]) {

/*    for (int i = 0; i < 4; ++i)
        printf ("%i,%i\n", coords[i][0], coords[i][1]);
    putchar ('\n');*/

    long **scoords = calloc (4, sizeof(long *));
    for (int i = 0; i < 4; ++i)
        scoords[i] = calloc (2, sizeof(long));
    sort_vertices (coords, scoords);

/*    for (int i = 0; i < 4; ++i)
        printf ("%i,%i\n", scoords[i][0], scoords[i][1]);
    putchar ('\n');*/

    /* NOTE : edges
        0 : left
        1 : top
        2 : right
        3 : bottom */

    // FIXME dynamic allocation for the edge length?
    // (or is it faster to just have a static size?)
    long edges[4][SCREEN_WIDTH][2];

    // left TODO start the x coord here
    get_edge (scoords[0], scoords[0], edges[0]);
    // top
    get_edge (scoords[0], scoords[2], edges[1]);
    // right TODO end the x coord here
    get_edge (scoords[0], scoords[0], edges[2]);
    // bottom
    get_edge (scoords[1], scoords[3], edges[3]);

    // draw the face
    // TODO GENERALIZE THIS TO ANY FACE
    for (int j = 0; j <= scoords[2][0] - scoords[0][0]; ++j)
        for (int y = edges[1][j][1]; y <= edges[3][j][1]; ++y)
            //draw_pixel (col, edges[3][j][0], y);
            dscreen_add_pixel (G_SCREEN.dscr, edges[3][j][0], y, col);

}

