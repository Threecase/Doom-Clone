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
#include "texture.h"


#define SPEED   100
#define ROTSP   1
#define DEF_WAD_LOC "/home/trevor/.config/gzdoom/doom.wad"




/* DOOM clone */
int main (int argc, char *argv[]) {

    extern Point player_pos;
    player_pos.x = 0;
    player_pos.y = 127;
    player_pos.z = 0;

    extern int angle;
    angle = 0;

    extern char DRAW_MODE;
    DRAW_MODE = DRAW_MODE_3D;

    // FIXME : temp
    extern int poly_count;
    poly_count = 0;

#ifndef __SDL
    int c;
#else
    SDL_Event e;
#endif
    char quit = 0;

    if (argc < 2)
        read_WAD (DEF_WAD_LOC);
    else
        read_WAD (argv[1]);

    init_video();
#ifndef __SDL
    term_raw();
#endif

    int lvl_num = 1;
    read_map_data (lvl_num);

    while (!quit) {
#ifndef __SDL
// framebuffer controls
        c = raw_input();
        if (c <= 0)
            fatal_error ("raw_input failed : %i!\n", c);
        //raw_writes ("%i\n", c);
        switch (c) {
#else
// SDL controls
    while (SDL_PollEvent (&e))
        switch (e.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
#endif
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
            DRAW_MODE = DRAW_MODE^1;
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

#ifndef __SDL
        // right move
        case 'D':
            player_pos.x += SPEED;
            break;
#endif
        // right rotate
        case 'd':
#ifdef __SDL
            if (e.key.keysym.mod & KMOD_SHIFT) {
                player_pos.x += SPEED;
                break;
            }
#endif
            angle = (angle - ROTSP) % 360;
            break;

#ifndef __SDL
        // left move
        case 'A':
            player_pos.x -= SPEED;
            break;
#endif
        // left rotate
        case 'a':
#ifdef __SDL
            if (e.key.keysym.mod & KMOD_SHIFT) {
                player_pos.x -= SPEED;
                break;
            }
#endif
            angle = (angle + ROTSP) % 360;
            break;
        }
#ifdef __SDL
    }
#endif
        if (!quit) {
            render_tree (NUM_NODES - 1);
            draw_render();
//            raw_writes ("%i %i\n\r", player_pos.x, player_pos.z);
        }
    }

    shutdown_video();
#ifndef __SDL
    term_noraw();
#endif

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

