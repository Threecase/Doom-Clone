/*
 * DOOM main loop
 *
 */

#include <unistd.h>

#include "data_types.h"
#include "drawing.h"
#include "BSP_tree.h"


// FIXME TEMPORARY
#define VERTS   4


/* DOOM clone */
int main() {

    init_video();


    Linedef *line_list[VERTS];
    Vertex vert_list[VERTS] = { {  50, 50 }, { -50, 50 },
                                {  50,-50 }, { -50,-50 }  };

    for (int i = 0; i < VERTS; ++i) {
        Linedef line = { &vert_list[i], &vert_list[(i+1)%4], LD_solid, 0, NULL,
                        {0,0} };
        line_list[i] = &line;
    }

    BSP_Node node = { line_list, NULL, NULL, VERTS };


    render (node);


    draw_render();

    sleep (5);


    shutdown_video();

    return 0;
}
