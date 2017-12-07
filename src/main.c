/*
 * DOOM main loop
 *
 */

#include <unistd.h>

#include "data_types.h"
#include "drawing.h"
#include "BSP_tree.h"
#include "read_wad.h"


// FIXME TEMPORARY
#define VERTS   4


/* DOOM clone */
int main (int argc, char *argv[]) {

    init_video();
    if (argc < 2)
        read_WAD ("/home/trevor/.config/gzdoom/doom.wad");
    else
        read_WAD (argv[1]);

    Linedef **line_list = malloc (sizeof(Linedef *) * (VERTS / 2));

    Vertex vert_list[VERTS] =
                    {   { -50, 50 },    // front face
                        {  50, 50 },
                        { -50,-50 },    // back face
                        {  50,-50 },  };


    // set up line list
    for (int i = 0, j = 0; j < VERTS; ++i, j += 2) {
        line_list[i] = malloc (sizeof(Linedef));
        line_list[i]->start = &vert_list[j];
        line_list[i]->end = &vert_list[j + 1];
        line_list[i]->flags = LD_solid;
        line_list[i]->type = i;
        line_list[i]->sector = NULL;
    }

    BSP_Node node = { line_list, NULL, NULL, 2};

    render_node (node);

    //draw_render();

    pause ();


    shutdown_video();

    return 0;
}
