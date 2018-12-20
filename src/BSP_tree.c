/*
 * The Binary Space Partition Tree data structure
 * and related functions.
 *
 */

#include "BSP_tree.h"
#include "wad_info.h"



char test_line (Node n, int x, int z);


/* render_tree: Traverse the BSP tree */
void render_tree (int16_t n_num)
{
    // N is a subsector
    if (n_num & IS_SSECTOR)
    {
        if (n_num == -1)
        {   n_num = 0;
        }
        if (DRAWMODE == DRAWMODE_2D)
        {   render_ssector_2D (SSECTOR_LIST[n_num & (~IS_SSECTOR)]);
        }
        else
        {   render_ssector (SSECTOR_LIST[n_num & (~IS_SSECTOR)]);
        }
        return;
    }

    Node bsp_node = NODE_LIST[n_num];

    int i = test_line (bsp_node, player_pos.x, player_pos.z);
    render_tree (bsp_node.child[i]);
    // TODO : only if the rear is visible
    render_tree (bsp_node.child[i^1]);
}

/* test_line: figure out where x/z are with respect to
    line L (eg in front, behind, etc.) */
// TODO : make sure this is working correctly
char test_line (Node n, int x, int z)
{
    if (!n.dz)  // node line is straight along X axis
    {
        if (z > n.z)    // z in front
            return n.x + n.dx > n.x;
        return n.x + n.dx < n.x;
    }

    if (!n.dx)  // node line is straight along Z axis
    {
        if (x > n.x)    // x in front
            return n.z + n.dz > n.z;
        return n.z + n.dz < n.z;
    }

    // more advanced calculations
    double m = ((n.z + n.dz) - n.z) / ((n.x + n.dx) - n.x);
    double b = n.z - m * n.x;
    if (z > m * x + b)  // z in front
    {   return n.x + n.dx > n.x;
    }
    return n.x + n.dx < n.x;
}

