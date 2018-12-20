/*
 * Framebuffer layer
 *
 */

#include "../drawing.h"

#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


void draw_real_pixel (long x, long y, Colour c);
static inline uint32_t pixel_colour (uint8_t r, uint8_t g, uint8_t b,
                                     struct fb_var_screeninfo *vinfo);

/* FIXME: TEMP */
void draw_pixel (unsigned long x, unsigned long y, Colour c)
{   backbuffer.pixels[y][x] = c;
}


struct fb_fix_screeninfo *finfo;
struct fb_var_screeninfo *vinfo;
int fd;
long size;
uint8_t *fbp;

struct termios def_term;



/* init_video: initialize framebuffer stuff */
void init_video(unsigned long w, unsigned long h, uint32_t flags)
{
    fd = open ("/dev/fb0", O_RDWR);
    if (fd == -1)
    {   fatal_error ("Cannot open framebuffer!");
    }

    /* get variable screen info */
    vinfo = malloc (sizeof(struct fb_var_screeninfo));
    ioctl (fd, FBIOGET_VSCREENINFO, vinfo);

    /* set colourspace */
    vinfo->grayscale = 0;
    vinfo->bits_per_pixel = 32;
    ioctl (fd, FBIOPUT_VSCREENINFO, vinfo);
    ioctl (fd, FBIOGET_VSCREENINFO, vinfo);

    /* get fixed screen info */
    finfo = malloc (sizeof(struct fb_fix_screeninfo));
    ioctl (fd, FBIOGET_FSCREENINFO, finfo);

    size = vinfo->yres_virtual*finfo->line_length;

    fbp = mmap (0, size, PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, (off_t)0);

    SCREEN_WIDTH = vinfo->xres;
    SCREEN_HEIGHT = vinfo->yres;
}

/* shutdown_video: cleanup framebuffer stuff */
void shutdown_video(void)
{
    free (vinfo);
    free (finfo);
    munmap (fbp, size);
    close (fd);
}

/* draw_frame: flip backbuffer to screen */
void draw_frame(void)
{
    for (long y = 0; y < backbuffer.height; ++y)
    {   for (long x = 0; x < backbuffer.height; ++x)
        {   draw_real_pixel (x,y, backbuffer.pixels[y][x]);
            backbuffer.pixels[y][x] = (Colour){ 0 };
        }
    }
}


/* init_input: set the terminal to raw mode */
void init_input(void)
{
    extern struct termios def_term;
    struct termios raw_term;


    if (!isatty (STDIN_FILENO))
        return term_error ("Not in a tty!", 2);

    if (tcgetattr (STDIN_FILENO, &def_term) < 0)
        return term_error ("Couldn't get tty settings!", 2);

    if (atexit (term_noraw) != 0)
        return term_error ("Couldn't register term_noraw atexit!", 2);

    raw_term = def_term;
    cfmakeraw (&raw_term);
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &raw_term);

    term_israw = 1;

    return 0;
}

/* shutdown_input: set term back to buffered mode */
void shutdown_input(void)
{
    tcflush (STDIN_FILENO, TCIOFLUSH);
    tcsetattr (STDIN_FILENO, TCSANOW, &def_term);
    term_israw = 0;
}

/* print: print characters to screen */
void print (char *format, ...)
{
    va_list ap;
    va_start (ap, format);

    /* FIXME: this is brittle */
    char str[1000];
    vsprintf (str, format, ap);

    for (int i = 0; str+i != NULL && str[i] != '\0'; ++i)
    {   write (STDERR_FILENO, str+i, 1);
    }
    va_end (ap);
}

/* input: read a character from the user */
int input(void)
{
    int bytes_read = 0,
        c = 0;

    tcflush (STDIN_FILENO, TCIFLUSH);
    bytes_read = read (STDIN_FILENO, &c, 1);

    if (bytes_read <= 0)
    {   return 0;   // input error
    }
    else
    {   return c;   // read a character
    }
}




/* Helper Functions */
/* draw_pixel: draw a pixel to the real screen */
void draw_real_pixel (long x, long y, Colour c)
{
    if (x < vinfo->xres && x >= 0 && y < vinfo->yres && y >= 0)
    {
        long loc = (x + vinfo->xoffset)
                 * (vinfo->bits_per_pixel/8)
                 + (y + vinfo->yoffset)
                 * finfo->line_length;
        *((uint32_t*)(fbp+loc)) = pixel_colour (c.r, c.g, c.b, vinfo);
    }
}

/* pixel_colour: get the correct pixel colour in the framebuffer */
static inline uint32_t pixel_colour (uint8_t r, uint8_t g, uint8_t b,
  struct fb_var_screeninfo *vinfo) {
    return (r << vinfo->red.offset)
         | (g << vinfo->green.offset)
         | (b << vinfo->blue.offset);
}

