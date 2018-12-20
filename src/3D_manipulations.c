/*
 * Functions for manipulating points in 3D space
 *
 */

#include "3D_manipulations.h"



/* rotate_on_axis: rotate x,y angle degrees around
    an axis to points newx,newy
    (2D or 3D rotations) */
void rotate_on_axis (int x, int y, int angle, int *newx, int *newy) {
    /* newx and newy mappings for rotation:
        newx = y, newy = z : rotation about x
        newx = z, newy = x : rotation about y
        newx = x, newy = y : rotation about z
        */

    double rads = angle * M_PI / 360;

    *newx = (x * cos (rads)) - (y * sin (rads));
    *newy = (y * cos (rads)) + (x * sin (rads));
}

/* 3D_to_2D: translate 3D coordinates to 2D ones */
int coords_3D_to_2D (Point A, Point C, int angle, int *x2D, int *y2D) {

    int ax, az;
    A.x += C.x;
    A.z += C.z;
    rotate_on_axis (A.z, A.x, angle, &az, &ax);
    if (az == 0)
        return 0;

    *x2D = (FOV * ax) / az;
    *y2D = (FOV * (A.y - C.y)) / az;
    return 1;
}

