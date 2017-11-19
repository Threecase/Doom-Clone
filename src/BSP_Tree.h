/*
 * The Binary Space Partition Tree data structure
 * and related functions
 *
 */

#ifndef _BSP_TREE_H
#define _BSP_TREE_H


#include <stdlib.h>


void create_BSP (BSP_Tree *N, Polygon **polys, int num_polys);

void traverse (BSP_Tree N, Viewpoint V);


#endif
