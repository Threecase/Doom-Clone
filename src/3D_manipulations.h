/*
 * Functions for manipulating points in 3D space
 *
 */

#ifndef _3D_MANIPULATIONS_H
#define _3D_MANIPULATIONS_H


#include <math.h>

#define FOV 256     /* camera FOV */


/* 3D (or 2D) points */
typedef struct Point {
    int x, y, z;
} Point;


void rotate_on_axis (int x, int y, int angle, int *newx, int *newy);

void coords_3D_to_2D (Point A, Point C, int cam_angle, int *x2D, int *y2D);


#endif

