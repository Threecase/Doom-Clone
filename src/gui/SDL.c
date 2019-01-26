/*
 * SDL layer
 *
 */

#include "../drawing.h"
#include "../error.h"

#include <SDL2/SDL.h>


void draw_real_pixel (long x, long y, Colour c);

/* FIXME: TEMP */
void draw_pixel (unsigned long x, unsigned long y, Colour c)
{
    if (x < backbuffer.width && y < backbuffer.height)
    {   backbuffer.pixels[y][x] = c;
    }
}


SDL_Window   *window = NULL;
SDL_Renderer *renderer = NULL;



/* init_video: initialize SDL */
void init_video (unsigned long w, unsigned long h, uint32_t flags)
{
    if (SDL_Init (SDL_INIT_VIDEO))
    {   fatal_error ("SDL_Init failed: %s", SDL_GetError());
    }

    SDL_DisplayMode mode;
    /* FIXME : possible errors assuming display=0 */
    if (SDL_GetCurrentDisplayMode (0, &mode))
    {   fatal_error ("Failed to get display mode: %s", SDL_GetError());
    }

    if (w <= 0 || h <= 0)
    {   w = 640;
        h = 480;
    }
    SCREEN_WIDTH  = w;
    SCREEN_HEIGHT = h;

    window = SDL_CreateWindow ("Doom",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               (flags & FLAG_FULLSCREEN)? SDL_WINDOW_FULLSCREEN : 0);
    if (window == NULL)
    {   fatal_error ("Failed to create SDL window: %s", SDL_GetError());
    }

    SDL_GetWindowSize (window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {   fatal_error ("Failed to create SDL renderer: %s", SDL_GetError());
    }
    SDL_RenderClear (renderer);


    backbuffer.width  = w;
    backbuffer.height = h;
    backbuffer.pixels = calloc (backbuffer.height, sizeof(*backbuffer.pixels));
    for (unsigned long y = 0; y < h; ++y)
    {
        backbuffer.pixels[y] = calloc (backbuffer.width, sizeof(*backbuffer.pixels[y]));
    }
}

/* shutdown_video: shut down SDL */
void shutdown_video(void)
{
    SDL_DestroyRenderer (renderer);
    renderer = NULL;

    SDL_DestroyWindow (window);
    window = NULL;

    SDL_Quit();
}

/* draw_frame: flip backbuffer to screen */
void draw_frame(void)
{
    for (unsigned long y = 0; y < backbuffer.height; ++y)
    {   for (unsigned long x = 0; x < backbuffer.height; ++x)
        {   draw_real_pixel (x,y, backbuffer.pixels[y][x]);
            backbuffer.pixels[y][x] = (Colour){ 0 };
        }
    }
    SDL_RenderPresent (renderer);
    SDL_RenderClear (renderer);
}


/* init_input:  */
void init_input(void) { }

/* shutdown_input:  */
void shutdown_input(void) { }

/* print: print characters to screen */
void print (char *format, ...)
{
    va_list ap;
    va_start (ap, format);

    vprintf (format, ap);

    va_end (ap);
}

/* input: read a character from user */
int input(void)
{
    SDL_Event e;
    if (SDL_PollEvent (&e))
    {   if (e.type == SDL_KEYDOWN)
        {
            /* FIXME: add some proper Shift behaviour (ie convert to uppercase) */
            if (e.key.keysym.mod & KMOD_SHIFT)
            {   return e.key.keysym.sym - 32;
            }
            else
            {   return e.key.keysym.sym;
            }
        }
    }
    return '\0';
}



/* draw_pixel: draw a pixel to the real screen */
void draw_real_pixel (long x, long y, Colour c)
{
    SDL_SetRenderDrawColor (renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawPoint (renderer, x, y);
}

