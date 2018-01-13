/*
 * Drawing utilities
 *
 */

#include "drawing.h"

#include "rawterm.h"    // FIXME : TEMP



/* render_ssector: draw a subsector to the screen */
// TODO : IMPROVE !!!
void render_ssector (SSector ssec) {

    Point pnt;
    int outx, outy, seg_n, corner;
    uint8_t col[ssec.num_segs][3];

    long pixels_to_draw[ssec.num_segs][4][2];
    memset (pixels_to_draw, 0, ssec.num_segs * 8);

    /* loop through lines in N, translate to 3D, and
        attempt to draw the polygon to the screen */
    seg_n = 0;
    for (int i = ssec.start_seg; i < ssec.start_seg + ssec.num_segs; ++i, ++seg_n) {
        corner = 0;
        // TODO : make this less messy?
        Sector sec = SECTOR_LIST[SIDEDEF_LIST[LINEDEF_LIST[SEG_LIST[
                        ssec.start_seg].line].sides[0]].sector];

        for (int y = sec.floor_h, n = 0; n < 2; ++n, y = sec.ceil_h) {

            // start vertex
            pnt.x = VERT_LIST[SEG_LIST[i].start].x;
            pnt.y = y;
            pnt.z = VERT_LIST[SEG_LIST[i].start].z;
            outx = outy = -1;
            if (coords_3D_to_2D (pnt, player_pos, angle, &outx, &outy)) {
                // add to drawbuffer
                pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
            }
            else {
                memset (pixels_to_draw[seg_n], 0, sizeof(long) * 8);
                break;
            }

            // end vertex
            pnt.x = VERT_LIST[SEG_LIST[i].end].x;
            pnt.y = y;
            pnt.z = VERT_LIST[SEG_LIST[i].end].z;
            outx = outy = -1;
            if (coords_3D_to_2D (pnt, player_pos, angle, &outx, &outy)) {
                // add to drawbuffer
                pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
            }
            else {
                memset (pixels_to_draw[seg_n], 0, sizeof(long) * 8);
                break;
            }
            col[seg_n][0] = pnt.x % 255;
            col[seg_n][1] = 127;
            col[seg_n][2] = pnt.z % 255;
        }
    }
    for (int j = 0, draw = 0; j < ssec.num_segs; ++j, draw = 0) {
        for (int i = 0; !draw && i < 4; ++i)
            if (pixels_to_draw[j][i][0] != 0 || pixels_to_draw[j][i][1] != 0)
                draw = 1;
        if (draw)
            fill_poly (pixels_to_draw[j], col[j]);
    }
}

// not needed with SDL
#ifndef __SDL
/* pixel_colour: inline function to
    set the correct pixel colour*/
static inline uint32_t pixel_colour (uint8_t r, uint8_t g, uint8_t b,
  struct fb_var_screeninfo *vinfo) {
    return (r << vinfo->red.offset)
         | (g << vinfo->green.offset)
         | (b << vinfo->blue.offset);
}
#endif

/* draw_pixel: draw a pixel to the screen */
void draw_pixel (uint8_t p[3], long x, long y) {

#ifndef __SDL
    if (x > G_SCREEN.vinfo->xres-1 || x < 0
     || y > G_SCREEN.vinfo->yres-1 || y < 0
     || !G_VIDEO_INIT
     || p[0] < 0 || p[1] < 0 || p[2] < 0)
        return;

    long loc = (x + G_SCREEN.vinfo->xoffset)
             * (G_SCREEN.vinfo->bits_per_pixel/8)
             + (y + G_SCREEN.vinfo->yoffset)
             * G_SCREEN.finfo->line_length;
    *((uint32_t*)(G_SCREEN.fbp + loc)) = pixel_colour (p[0], p[1], p[2],
                                            G_SCREEN.vinfo);
#else
    SDL_SetRenderDrawColor (G_SCREEN.renderer, p[0], p[1], p[2], 255);
    SDL_RenderDrawPoint (G_SCREEN.renderer, x, y);
#endif
}

/* draw_render: finalize the render for this frame;
    draw and clear the dscreen */
void draw_render() {

    for (int y = 0; y < G_SCREEN.dscr->height; ++y)
        for (int x = 0; x < G_SCREEN.dscr->width; ++x) {
            draw_pixel (G_SCREEN.dscr->pixels[x][y], x, y);
            G_SCREEN.dscr->pixels[x][y][0] = 0;
            G_SCREEN.dscr->pixels[x][y][1] = 0;
            G_SCREEN.dscr->pixels[x][y][2] = 0;
        }
#ifdef __SDL
    SDL_RenderPresent (G_SCREEN.renderer);
    SDL_RenderClear (G_SCREEN.renderer);
#endif
    poly_count = 0;
}

/* init_video: initialize graphics */
void init_video() {

    extern struct Screen G_SCREEN;
    extern unsigned SCREEN_WIDTH, SCREEN_HEIGHT; 
    extern char (*PALETTE)[3];

#ifndef __SDL
    G_SCREEN.fd = open ("/dev/fb0", O_RDWR);
    if (G_SCREEN.fd == -1)
        fatal_error ("Cannot open framebuffer!");

    // get variable screen info
    G_SCREEN.vinfo = malloc (sizeof(struct fb_var_screeninfo));
    ioctl (G_SCREEN.fd, FBIOGET_VSCREENINFO, G_SCREEN.vinfo);

    // set colourspace
    G_SCREEN.vinfo->grayscale = 0;
    G_SCREEN.vinfo->bits_per_pixel = 32;
    ioctl (G_SCREEN.fd, FBIOPUT_VSCREENINFO, G_SCREEN.vinfo);
    ioctl (G_SCREEN.fd, FBIOGET_VSCREENINFO, G_SCREEN.vinfo);

    // get fixed screen info
    G_SCREEN.finfo = malloc (sizeof(struct fb_fix_screeninfo));
    ioctl (G_SCREEN.fd, FBIOGET_FSCREENINFO, G_SCREEN.finfo);

    G_SCREEN.size = G_SCREEN.vinfo->yres_virtual*G_SCREEN.finfo->line_length;

    G_SCREEN.fbp = mmap (0, G_SCREEN.size, PROT_READ | PROT_WRITE,
                    MAP_SHARED, G_SCREEN.fd, (off_t)0);

    SCREEN_WIDTH = G_SCREEN.vinfo->xres;
    SCREEN_HEIGHT = G_SCREEN.vinfo->yres;
#else
    if (SDL_Init (SDL_INIT_VIDEO))
        fatal_error ("SDL_Init failed: %s", SDL_GetError());

    SDL_DisplayMode mode;
    // FIXME : possible errors assuming display=0
    if (SDL_GetCurrentDisplayMode (0, &mode))
        fatal_error ("Failed to get display mode: %s", SDL_GetError());

    SCREEN_WIDTH = 640;
    SCREEN_HEIGHT = 480;

    // TODO : configure res / fullscreen etc.
    G_SCREEN.window = SDL_CreateWindow ("Doom", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
        /*SDL_WINDOW_FULLSCREEN*/0);
    if (G_SCREEN.window == NULL)
        fatal_error ("Failed to create SDL window: %s", SDL_GetError());

    SDL_GetWindowSize (G_SCREEN.window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    G_SCREEN.renderer = SDL_CreateRenderer (G_SCREEN.window, -1,
        SDL_RENDERER_ACCELERATED);
    if (G_SCREEN.renderer == NULL)
        fatal_error ("Failed to create SDL renderer: %s", SDL_GetError());
#endif

    G_SCREEN.dscr = malloc (sizeof(DScreen));
    G_SCREEN.dscr->width = SCREEN_WIDTH;
    G_SCREEN.dscr->height = SCREEN_HEIGHT;
    G_SCREEN.dscr->pixels = calloc (G_SCREEN.dscr->width,
        sizeof(uint8_t (*)[3]));
    for (int i = 0; i < G_SCREEN.dscr->width; ++i)
        G_SCREEN.dscr->pixels[i] = calloc (G_SCREEN.dscr->height,
                                    sizeof(uint8_t (*)[3]));

    // read in palettes
    PALETTE = malloc (14 * 768);    /* the 14 palettes are
                                        always 768 bytes long */
    if (PALETTE == NULL)
        fatal_error ("init_video: failed to alloc palette!");

    int i = get_lump_index ("PLAYPAL");
    if (i < 0)
        fatal_error ("WAD missing PLAYPAL lump!");
    read_lump (i, PALETTE);


    extern char G_VIDEO_INIT;
    G_VIDEO_INIT = 1;
}

/* shutdown_video: cleanup graphics variables */
void shutdown_video() {

    for (int i = 0; i < G_SCREEN.dscr->width; ++i)
        free (G_SCREEN.dscr->pixels[i]);
    free (G_SCREEN.dscr->pixels);
    free (G_SCREEN.dscr);
#ifndef __SDL
    free (G_SCREEN.vinfo);
    free (G_SCREEN.finfo);
    munmap (G_SCREEN.fbp, G_SCREEN.size);
    close (G_SCREEN.fd);
#else
    SDL_DestroyRenderer (G_SCREEN.renderer);
    G_SCREEN.renderer = NULL;
    SDL_DestroyWindow (G_SCREEN.window);
    G_SCREEN.window = NULL;
    SDL_Quit();
#endif

    free (PALETTE);

    G_VIDEO_INIT = 0;

}

