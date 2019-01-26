/*
 * Interpret the info from the WAD.
 * (ie generate map, etc.)
 *
 */

#ifndef _WAD_INFO_H
#define _WAD_INFO_H


#include "data_types.h"
#include "read_wad.h"


typedef struct Level
{
    Thing   *things;
    Linedef *linedefs;
    Sidedef *sidedefs;
    Vertex  *vertices;
    Seg     *segs;
    SSector *ssectors;
    Node    *nodes;
    Sector  *sectors;

    int num_things,
        num_linedefs,
        num_sidedefs,
        num_vertices,
        num_segs,
        num_ssectors,
        num_nodes,
        num_sectors;

    /* TODO: Reject and Blockmap */
} Level;



Level read_map_data (int lvl_num);


#endif

