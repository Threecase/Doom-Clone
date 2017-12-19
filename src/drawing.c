/*
 * Drawing utilities
 *
 */

#include "drawing.h"


/* pixel_colour: inline function to
    set the correct pixel colour*/
static inline uint32_t pixel_colour (uint8_t r, uint8_t g, uint8_t b,
  struct fb_var_screeninfo *vinfo) {
    return (r << vinfo->red.offset)
         | (g << vinfo->green.offset)
         | (b << vinfo->blue.offset);
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
    for (int x = 0; x < G_SCREEN.dscr->height; ++x)
        memset (G_SCREEN.dscr->pixels[x], 0, G_SCREEN.dscr->width);
}

/* render_ssec: draw a subsector to the screen */
void render_ssector (SSector ssec, Point camera) {

    Point pnt;
    int outx, outy, seg_n, corner;
    uint8_t col[ssec.num_segs][3];

    long pixels_to_draw[ssec.num_segs][4][2];

    /* loop through lines in N, translate to 3D, and
        attempt to draw the polygon to the screen */
    seg_n = 0;
    for (int i = ssec.start_seg; i < ssec.start_seg + ssec.num_segs; ++i) {
        corner = 0;
        for (int y = 0; y < 255; y += 254) {    // FIXME hardcoded y limit

            // start vertex
            pnt.x = VERT_LIST[SEG_LIST[i].start].x;
            pnt.y = y;
            pnt.z = VERT_LIST[SEG_LIST[i].start].z;
            if (pnt.z < camera.z) {
                coords_3D_to_2D (pnt, camera, angle, &outx, &outy);
                if (&outx != NULL && &outy != NULL) {
                    // add to drawbuffer
                    pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                    pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
                    col[seg_n][0] = col[seg_n][1] = col[seg_n][2] = pnt.z % 255;
                }
            }
            else {
                pixels_to_draw[seg_n][corner][0] = 0;
                pixels_to_draw[seg_n][corner++][1] = 0;
                pixels_to_draw[seg_n][corner][0] = 0;
                pixels_to_draw[seg_n][corner++][1] = 0;
                continue;
            }

            // end vertex
            pnt.x = VERT_LIST[SEG_LIST[i].end].x;
            pnt.y = y;
            pnt.z = VERT_LIST[SEG_LIST[i].end].z;
            if (pnt.z < camera.z) {
                coords_3D_to_2D (pnt, camera, angle, &outx, &outy);
                if (&outx == NULL || &outy == NULL)
                    continue;
                // add to drawbuffer
                pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
                col[seg_n][0] = col[seg_n][1] = col[seg_n][2] = pnt.z % 255;
            }
            else {
                pixels_to_draw[seg_n][corner-1][0] = 0;
                pixels_to_draw[seg_n][corner-1][1] = 0;
                pixels_to_draw[seg_n][corner][0] = 0;
                pixels_to_draw[seg_n][corner++][1] = 0;
                continue;
            }
        }
        seg_n++;
    }
    for (int j = 0; j < ssec.num_segs; ++j) {
        /*for (int i = 0; i < 4; ++i)
            draw_pixel (col, pixels_to_draw[j][i][0], pixels_to_draw[j][i][1]);*/
        fill_poly (pixels_to_draw[j], col[j]);
    }
}

/* draw_pixel: draw a pixel to the screen */
void draw_pixel (uint8_t p[3], long x, long y) {

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
}

/* init_video: initialize graphics */
void init_video() {

    extern struct Screen G_SCREEN;

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

    G_SCREEN.dscr = malloc (sizeof(DScreen));
    G_SCREEN.dscr->width = G_SCREEN.vinfo->xres;
    G_SCREEN.dscr->height = G_SCREEN.vinfo->yres;
    G_SCREEN.dscr->pixels = calloc (G_SCREEN.dscr->width,
        sizeof(uint8_t (*)[3]));
    for (int i = 0; i < G_SCREEN.dscr->width; ++i)
        G_SCREEN.dscr->pixels[i] = calloc (G_SCREEN.dscr->height,
                                    sizeof(uint8_t (*)[3]));


    extern unsigned SCREEN_WIDTH, SCREEN_HEIGHT; 
    SCREEN_WIDTH = G_SCREEN.vinfo->xres;
    SCREEN_HEIGHT = G_SCREEN.vinfo->yres;

    extern char G_VIDEO_INIT;
    G_VIDEO_INIT = 1;
}

/* shutdown_video: cleanup graphics variables */
void shutdown_video() {

    free (G_SCREEN.vinfo);
    free (G_SCREEN.finfo);
    free (G_SCREEN.dscr);
    munmap (G_SCREEN.fbp, G_SCREEN.size);
    close (G_SCREEN.fd);

    G_VIDEO_INIT = 0;

}

