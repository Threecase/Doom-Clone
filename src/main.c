/*
 * DOOM main loop
 *
 */

#include <unistd.h>
#include <SDL2/SDL.h>   /* FIXME: TEMP */

#include "data_types.h"
#include "drawing.h"
#include "read_wad.h"
#include "wad_info.h"
#include "BSP_tree.h"
#include "input.h"
#include "texture.h"


#define SPEED   100
#define ROTSP   1
#define DEF_WAD_LOC "/home/trevor/.config/gzdoom/doom.wad"




/* DOOM clone */
int main (int argc, char *argv[])
{

    extern Point player_pos;
    player_pos.x = 0;
    player_pos.y = 127;
    player_pos.z = 0;

    extern int angle;
    angle = 0;

    DRAWMODE = DRAWMODE_3D;

    // FIXME : temp
    extern int poly_count;
    poly_count = 0;

    int c;
    char quit = 0;

    char *wadname = NULL;
    int swidth, sheight;
    swidth = sheight = 0;
    uint32_t windowflags = 0;
    // argv handling
    for (int i = 1; i < argc; ++i) {
        // wad to read
        if (!strcmp (argv[i], "-w") && i+1 < argc) {
            int l = strlen (argv[++i]);
            wadname = malloc (l);
            memcpy (wadname, argv[i], l);
        }
        else if (!strcmp (argv[i], "-f"))
            windowflags |= FLAG_FULLSCREEN;
        // set resolution
        else if (!strcmp (argv[i], "-r") && i+2 < argc) {
            swidth = atoi (argv[++i]);
            sheight = atoi (argv[++i]);
        }
    }
    if (wadname == NULL)
        wadname = DEF_WAD_LOC;
    read_WAD (wadname);

    init_video (swidth, sheight, windowflags);
    init_input();
    init_palette();

    int lvl_num = 1;
    read_map_data (lvl_num);

    while (!quit) {
        while ((c = input()))
        {
            switch (c)
            {
            // FIXME : temp keybinds
            // quit 
            case 'q':
            case 'Q':
                quit = 1;
                break;
            // load next level
            case 'm':
            case 'M':
                lvl_num = (lvl_num + 1) % 9;
                read_map_data (lvl_num);
                break;

            // toggle automap
            case '\t':
                DRAWMODE = DRAWMODE^1;
                break;

            // Movement
            // forward
            case 'w':
            case 'W':
                player_pos.z += SPEED;
                break;
            // backward
            case 's':
            case 'S':
                player_pos.z -= SPEED;
                break;

            // right move
            case 'D':
                player_pos.x += SPEED;
                break;
            // right rotate
            case 'd':
                angle = (angle - ROTSP) % 360;
                break;

            // left move
            case 'A':
                player_pos.x -= SPEED;
                break;
            // left rotate
            case 'a':
                angle = (angle + ROTSP) % 360;
                break;
            }
        }

        if (!quit) {
            render_tree (NUM_NODES - 1);
            draw_frame();
//            raw_writes ("%i %i\n\r", player_pos.x, player_pos.z);
        }
    }

    shutdown_video();
    shutdown_input();

    // free globals
    for (int i = 0; i < 8; ++i)
        free (DATA[i]);
    free (DATA);
    free (LUMPS);

/*
    free (pnames);
    free (tex_data[0].offsets);
    free (tex_data[0].texture);
    free (tex_data[1].offsets);
    free (tex_data[1].texture);
    */

    return 0;
}

