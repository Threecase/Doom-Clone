/*
 * The Binary Space Partition Tree data structure
 * and related functions
 *
 */

#ifndef _BSP_TREE_H
#define _BSP_TREE_H


#include <stdlib.h>

#include "data_types.h"
#include "drawing.h"
#include "3D_manipulations.h"


#define IS_SSECTOR  ((int16_t)0x8000)


void create_BSP (Node *N, Linedef **polys, int num_polys);

void render_tree (int16_t node_number);


#endif
