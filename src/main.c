/*
 * DOOM main loop
 *
 */

#include <unistd.h>

#include "data_types.h"
#include "drawing.h"
#include "read_wad.h"
#include "wad_info.h"
#include "BSP_tree.h"
#include "rawterm.h"


#define SPEED   100
#define ROTSP   90
#define DEF_WAD_LOC "doom.wad"



/* DOOM clone */
int main (int argc, char *argv[]) {

    extern Point player_pos;
    player_pos.x = 0;
    player_pos.y = 127;
    player_pos.z = 0;
    extern int angle;
    angle = 0;
    extern int poly_count;
    poly_count = 0;

    int c;
    char quit = 0;

    init_video();
    term_raw();

    if (argc < 2)
        read_WAD (DEF_WAD_LOC);
    else
        read_WAD (argv[1]);

    int lvl_num = 1;
    read_map_data (lvl_num);

    while (!quit) {
        c = raw_input();
        if (c <= 0)
            fatal_error ("raw_input failed : %i!\n", c);

//        raw_writes ("%i\n", c);
        switch (c) {
        // FIXME : temp keybinds
        // quit 
        case 'q':
        case 'Q':
            quit = 1;
            break;
        // load next level
        case '\t':
            lvl_num = (lvl_num + 1) % 9;
            read_map_data (lvl_num);
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

        if (!quit) {
            render_tree (NUM_NODES - 1);
            draw_render();
//            raw_writes ("%i %i\n\r", player_pos.x, player_pos.z);
        }
    }

    shutdown_video();
    term_noraw();

    return 0;
}

