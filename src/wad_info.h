/*
 * Interpret the info from the WAD.
 * (ie generate map, etc.)
 *
 */

#ifndef _WAD_INFO_H
#define _WAD_INFO_H


#include "read_wad.h"

/* quick access to DATA lists */
#define THING_LIST      ((Thing *)DATA[0])
#define LINEDEF_LIST    ((Linedef *)DATA[1])
#define SIDEDEF_LIST    ((Sidedef *)DATA[2])
#define VERT_LIST       ((Vertex *)DATA[3])
#define SEG_LIST        ((Seg *)DATA[4])
#define SSECTOR_LIST    ((SSector *)DATA[5])
#define NODE_LIST       ((Node *)DATA[6])
#define SECTOR_LIST     ((Sector *)DATA[7])
#define REJECT_LIST     DATA[8]
#define BLOCKMAP_LIST   DATA[9]

/* length of DATA lists */
#define NUM_THINGS      NUM_STUFF[0]
#define NUM_LINEDEFS    NUM_STUFF[1]
#define NUM_SIDEDEFS    NUM_STUFF[2]
#define NUM_VERTS       NUM_STUFF[3]
#define NUM_SEGS        NUM_STUFF[4]
#define NUM_SSECTORS    NUM_STUFF[5]
#define NUM_NODES       NUM_STUFF[6]
#define NUM_SECTORS     NUM_STUFF[7]
#define NUM_REJECTS     NUM_STUFF[8]
#define NUM_BLOCKMAPS   NUM_STUFF[9]


// VARIABLES
int NUM_STUFF[10];
void **DATA;


// FUNCTIONS
void read_map_data (int lvl_num);


#endif

