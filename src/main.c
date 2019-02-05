/*
 * DOOM main loop
 *
 */

#include "data_types.h"
#include "drawing.h"
#include "read_wad.h"
#include "wad_info.h"
#include "BSP_tree.h"
#include "input.h"
#include "texture.h"
#include "DOOM.h"

#include <unistd.h>
#include <stdbool.h>
#include <math.h>


void draw_sidedef_texture_as_quad (Sidedef sd);/* FIXME: temp */



Level level;
int lvl_num = 1;

/* FIXME: temp until I figure out how to do it better */
bool mouse_moved_this_frame = false;



/* DOOM clone */
int main (int argc, char *argv[])
{
    player.pos.x = 0;
    player.pos.y = 0;
    player.pos.z = 0;
    player.angle     = 0;

    player.speed     = 5;
    player.rot_speed = 3;
    player.mouse_sensitivity = 1.2;

    game.drawmode  = DRAWMODE_3D;
    game.map_scale = 1.0;
    game.FOV       = 90;
    game.map_scale_delta = 1.0;


    handle_arguments (argc, argv);
    DOOM_init();

    DOOM_start (DOOM_update_and_render);

    DOOM_exit (0);
    return 0;
}

/* handle_arguments: handle command-line arguments */
void handle_arguments (int argc, char *argv[])
{
    game.wadname = NULL;

    game.window.width  = 800;
    game.window.height = 600;
    game.window.flags  =  0 ;


    /* argv handling */
    for (int i = 1; i < argc; ++i)
    {   /* wad to read */
        if (!strcmp (argv[i], "-w") && i+1 < argc)
        {
            int l = strlen (argv[++i]);
            game.wadname = malloc (l);
            memcpy (game.wadname, argv[i], l);
        }
        else if (!strcmp (argv[i], "-f"))
        {   game.window.flags |= FLAG_FULLSCREEN;
        }
        /* set resolution */
        else if (!strcmp (argv[i], "-r") && i+2 < argc)
        {
            game.window.width  = atoi (argv[++i]);
            game.window.height = atoi (argv[++i]);
        }
    }
    if (game.wadname == NULL)
    {   game.wadname = "doom.wad";
    }
}

/* DOOM_init: initialize the game */
void DOOM_init(void)
{
    /* read the supplied WAD */
    read_WAD (game.wadname);

    /* initialize stuff */
    init_texture_directories();
    init_pnames();
    init_video();
    init_input();
    init_palette();


    lvl_num = 1;
    level = read_map_data (lvl_num);
}

/* DOOM_start */
void DOOM_start (void (*update_function)(int))
{
    update_function (0);
    start_mainloop();
}

/* DOOM_update: do logic + ouput a frame */
void DOOM_update_and_render (int n)
{
    /* run the game */
#if 1
    DOOM_get_input();
    DOOM_update();
    DOOM_render();
    /* just render a texture (for testing) */
#else
    draw_frame();
    draw_sidedef_texture_as_quad (level.sidedefs[0]);
    draw_frame();
#endif
    call_in_1_60th_of_a_second (DOOM_update_and_render);
}

/* DOOM_update: do logic for 1 frame */
void DOOM_update(void)
{
    player.pos.x += player.vector.x *  sin (RADS(player.angle));
    player.pos.y += player.vector.x * -cos (RADS(player.angle));

    player.pos.x += player.vector.y * cos (RADS(player.angle));
    player.pos.y += player.vector.y * sin (RADS(player.angle));


    player.pos.z += player.vector.z;

    player.angle += player.angle_vector;
    player.angle = fmodf (player.angle, 360.f);

    if (mouse_moved_this_frame)
    {   player.angle_vector = 0;
    }


    game.map_scale *= game.map_scale_delta;


#if 0
    if (player.vector.x >= 10)      player.vector.x -= 10;
    if (player.vector.y >= 10)      player.vector.y -= 10;
    if (player.vector.z >= 10)      player.vector.z -= 10;

    if (player.vector.x <= -10)     player.vector.x += 10;
    if (player.vector.y <= -10)     player.vector.y += 10;
    if (player.vector.z <= -10)     player.vector.z += 10;

    if (abs (player.vector.x) <= 10)    player.vector.x = 0;
    if (abs (player.vector.y) <= 10)    player.vector.y = 0;
    if (abs (player.vector.z) <= 10)    player.vector.z = 0;
#endif
}

/* DOOM_get_input: get user input */
/* TODO: clean this up */
void DOOM_get_input(void)
{
    mouse_moved_this_frame = false;

    Event e = NULL_EVENT;
    do
    {
        e = input();
        switch (e.type)
        {
        /* key pressed */
        case EVENT_KEYPRESS:
            switch (e.key)
            {
            /* FIXME : temp keybinds */
            /* quit */
            case 'q':
            case 'Q':
                DOOM_exit (0);
                break;
            /* load next level */
            case 'm':
            case 'M':
                lvl_num = (lvl_num + 1) % 9;
                read_map_data (lvl_num);
                break;

            /* toggle automap */
            case '\t':
                game.drawmode = game.drawmode^1;
                break;

            /* Movement */
            /* forward */
            case 'w':
            case 'W':
                player.vector.y += player.speed;
                break;
            /* backward */
            case 's':
            case 'S':
                player.vector.y += -player.speed;
                break;

            /* right move */
            case 'd':
            case 'D':
                player.vector.x += -player.speed;
                break;
            /* left move */
            case 'a':
            case 'A':
                player.vector.x += player.speed;
                break;

            /* zoom in */
            case 'z':
            case 'Z':
                game.map_scale_delta *= 1.1;
                break;
            /* zoom out */
            case 'x':
            case 'X':
                game.map_scale_delta /= 1.1;
                break;

            /* go up */
            case 'e':
            case 'E':
                player.vector.z = player.speed;
                break;
            /* go down */
            case 'r':
            case 'R':
                player.vector.z = -player.speed;
                break;
            }
            break;

        /* key released */
        case EVENT_KEYRELEASE:
            switch (e.key)
            {
            case 'w':
            case 'W':
                player.vector.y -= player.speed;
                break;
            case 's':
            case 'S':
                player.vector.y -= -player.speed;
                break;

            case 'd':
            case 'D':
                player.vector.x -= -player.speed;
                break;
            case 'a':
            case 'A':
                player.vector.x -= player.speed;
                break;

            case 'e':
            case 'E':
                player.vector.z -= player.speed;
                break;
            case 'r':
            case 'R':
                player.vector.z -= -player.speed;
                break;

            case 'z':
            case 'Z':
                game.map_scale_delta /= 1.1;
                break;
            case 'x':
            case 'X':
                game.map_scale_delta *= 1.1;
                break;
            }
            break;

        /* mouse motion */
        case EVENT_MOUSEMOTION:
            player.angle_vector = e.motion.dx * player.mouse_sensitivity;
            mouse_moved_this_frame = true;
            break;


        case EVENT_NONE:
            break;

        default:
            debug ("bad event type: %d", e.type);
            break;
        }
    } while (e.type != EVENT_NONE);
}

/* DOOM_render: ouput a frame */
void DOOM_render(void)
{
    render_tree (level.num_nodes - 1, &level);
    draw_frame();
}

/* DOOM_exit: clean everything up and exit */
void DOOM_exit (int status)
{
    shutdown_video();
    shutdown_input();

    /* free globals */
    free (level.things);
    free (level.linedefs);
    free (level.sidedefs);
    free (level.vertices);
    free (level.segs);
    free (level.ssectors);
    free (level.nodes);
    free (level.sectors);
    free (LUMPS);

    exit (status);
}

