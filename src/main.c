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



/* DOOM clone */
int main (int argc, char *argv[]) {

    extern int angle;
    angle = 0;

    int c;
    char quit = 0;

    long player_x, player_z;
    player_x = 0;
    player_z = 0;

    init_video();
    term_raw();

    if (argc < 2)
        read_WAD ("/home/trevor/.config/gzdoom/doom.wad");
    else
        read_WAD (argv[1]);

    read_map_data();

    while (!quit) {
        c = raw_input();
        if (c <= 0)
            fatal_error ("raw_input failed : %i!\n", c);

//        printf ("%i\n", c);
        switch (c) {
        // quit FIXME : temp keybind
        case 'q':
        case 'Q':
        case 1393:
            quit = 1;
            break;
        // Movement
        // forward
        case 'w':
        case 'W':
        case 1399:
            player_z += 50;
            break;
        // backward
        case 's':
        case 'S':
        case 1395:
            player_z -= 50;
            break;
        // right
        case 'd':
        case 'D':
        case 1377:
            player_x += 50;
//            angle = (angle - 1) % -359;
            break;
        // left
        case 'a':
        case 'A':
        case 1380:
            player_x -= 50;
//            angle = (angle + 1) % 359;
            break;
        }

        render_tree (NUM_NODES - 1, (Point){ player_x, 90, player_z });
        draw_render();
    }

    shutdown_video();
    term_noraw();

    return 0;
}

