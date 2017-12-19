/*
 * Functions for manipulating points in 3D space
 *
 */

#include "3D_manipulations.h"

// FIXME : TEMP INCLUDE
#include <stdio.h>



/* rotate_on_axis: rotate x,y angle degrees around
    an axis to points newx,newy
    (2D or 3D rotations) */
void rotate_on_axis (int x, int y, int angle, int *newx, int *newy) {
    /* newx and newy mappings for rotation:
        newx = y, newy = z : rotation about x
        newx = z, newy = x : rotation about y
        newx = x, newy = y : rotation about z
        */

    double rads = angle * M_PI / 180;

    *newx = (x * cos (rads)) - (y * sin (rads));
    *newy = (x * sin (rads)) + (y * cos (rads));
}

/* 3D_to_2D: translate 3D coordinates to 2D ones */
void coords_3D_to_2D (Point A, Point C, int angle, int *x2D, int *y2D) {

    int ax, az;
    rotate_on_axis (C.x - A.x, C.z - A.z, angle, &ax, &az);
    int div = C.z + az;
    if (div == 0) {
//        puts ("coords_3D_to_2D: avoided divide by zero!");
        // causes extreme lag
        x2D = NULL;
        y2D = NULL;
        return;
    }

    *x2D = (FOV * (ax + C.x)) / div;
    *y2D = (FOV * (A.y - C.y)) / div;
}

