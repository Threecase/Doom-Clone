/*
 * Functions for manipulating points in 3D space
 *
 */

#ifndef _3D_MANIPULATIONS_H
#define _3D_MANIPULATIONS_H


void rotate_on_axis (int x, int y, int angle, int *newx, int *newy);

int coords_3D_to_2D (Point A, Point C, int cam_angle, int *x2D, int *y2D);


#endif

