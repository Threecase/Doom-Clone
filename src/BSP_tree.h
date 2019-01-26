/*
 * The Binary Space Partition Tree data structure
 * and related functions
 *
 */

#ifndef __BSP_TREE_H
#define __BSP_TREE_H


#include "data_types.h"
#include "wad_info.h"


#define IS_SSECTOR  ((int16_t)0x8000)


void create_BSP (Node *N, Linedef **polys, int num_polys);

void render_tree (int16_t node_number, Level *level);


#endif

