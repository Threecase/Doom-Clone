/*
 * The Binary Space Partition Tree data structure
 * and related functions.
 *
 */

#include "BSP_tree.h"
#include "wad_info.h"
#include "drawing.h"
#include "3D_manipulations.h"

#include <stdlib.h>
#include <stdbool.h>


bool test_line (Node n, int x, int z);


/* render_tree: Traverse the BSP tree */
void render_tree (int16_t n_num, Level *level)
{
    /* N is a subsector */
    if (n_num & IS_SSECTOR)
    {
        if (n_num == -1)
        {   n_num = 0;
        }
        if (DRAWMODE == DRAWMODE_2D)
        {   render_ssector_2D (level->ssectors[n_num & (~IS_SSECTOR)], level);
        }
        else
        {   render_ssector (level->ssectors[n_num & (~IS_SSECTOR)], level);
        }
    }
    else
    {
        Node bsp_node = level->nodes[n_num];

        bool i = test_line (bsp_node, player_pos.x, player_pos.y);
        if (i == false)
        {
            render_tree (bsp_node.left_child, level);
            render_tree (bsp_node.right_child, level);
        }
        else
        {   render_tree (bsp_node.right_child, level);
            render_tree (bsp_node.left_child, level);
        }
    }
}

/* test_line: figure out where x/z are with respect to
              line L (eg in front, behind, etc.) */
/* TODO: make sure this is working correctly */
bool test_line (Node n, int x, int z)
{
    /* node line is straight along X axis */
    if (n.dy == 0)
    {   /* z in front */
        if (z > n.y)
        {   return n.x + n.dx > n.x;
        }
        return n.x + n.dx < n.x;
    }

    /* node line is straight along Z axis */
    if (n.dx == 0)
    {   /* x in front */
        if (x > n.x)
        {   return n.y + n.dy > n.y;
        }
        return n.y + n.dy < n.y;
    }

    /* otherwise, we need to do more advanced calculations */
    double m = ((n.y + n.dy) - n.y) / ((n.x + n.dx) - n.x);
    double b = n.y - m * n.x;

    /* z in front */
    if (z > m * x + b)
    {   return n.x + n.dx > n.x;
    }
    return n.x + n.dx < n.x;
}

