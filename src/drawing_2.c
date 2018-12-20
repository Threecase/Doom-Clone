/*
 * High-level drawing functions
 * (ie polygon fill, etc.)
 *
 */

#include "drawing_2.h"


/* get_edge: get all the pixels along the line
    x1/y1 to x2/y2 (ie left[] to right[])
    (the values are stored in edge_table) */
void get_edge (long left[2], long right[2], long edge_table[SCREEN_WIDTH][2])
{

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
    for (int i = 0; i <= longest && i <= SCREEN_WIDTH; ++i) {
    
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
void sort_vertices (long coords[4][2]) {

    /* sort the coords */
    for (int max = 4; max > 0; --max)
        for (int i = 1; i < max; ++i)
            if (coords[i-1][0] > coords[i][0]) {
                long t[2];
                memcpy (t, coords[i], sizeof(long[2]));
                memcpy (coords[i], coords[i-1], sizeof(long[2]));
                memcpy (coords[i-1], t, sizeof(long[2]));
            }
    for (int i = 1; i < 4; i += 2)
        if (coords[i-1][1] > coords[i][1]) {
            long t[2];
            memcpy (t, coords[i], sizeof(long[2]));
            memcpy (coords[i], coords[i-1], sizeof(long[2]));
            memcpy (coords[i-1], t, sizeof(long[2]));
        }
}

/* fill_poly: fill the polygon */
void fill_poly (long coords[4][2], Colour col)
{
    sort_vertices (coords);

    /* NOTE : edges
        0 : left
        1 : top
        2 : right
        3 : bottom */

    /* FIXME dynamic allocation for the edge length?
        (or is it faster to just have a static size?) */
    long edges[4][SCREEN_WIDTH][2];

    // left
    get_edge (coords[0], coords[1], edges[0]);
    // top
    get_edge (coords[0], coords[2], edges[1]);
    // right
    get_edge (coords[2], coords[3], edges[2]);
    // bottom
    get_edge (coords[1], coords[3], edges[3]);

    // draw the face
    // TODO optimize (can take nearly 1% of a second!)
    // TODO : add texturing
    for (int j = 0; j < SCREEN_WIDTH && j <= abs (edges[0][0][0] - edges[2][0][0]); ++j)
    {   for (int y = edges[1][j][1]; y < SCREEN_HEIGHT && y <= edges[3][j][1]; ++y)
//            draw_pixel (col, edges[3][j][0], y);
        {   draw_pixel (edges[3][j][0], y, col);
        }
    }
    poly_count++;
}

