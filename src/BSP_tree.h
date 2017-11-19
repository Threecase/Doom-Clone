/*
 * The Binary Space Partition Tree data structure
 * and related functions
 *
 */

#ifndef _BSP_TREE_H
#define _BSP_TREE_H


#include <stdlib.h>

#include "data_types.h"


void create_BSP (BSP_Node *N, SSector **polys, int num_polys);

void traverse (BSP_Node N, Thing V);


#endif
