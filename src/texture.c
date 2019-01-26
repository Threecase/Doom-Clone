/*
 * Texturing utilities
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "texture.h"
#include "data_types.h"
#include "drawing.h"
#include "input.h"
#include "read_wad.h"



void read_picture_lumps();
void get_full_imgs();

/* read_texture_lump: reads the TEXTURE lump(s) */
void read_texture_lump() {

    extern Texture_lump tex_data;
    tex_data.num_tex = 0;
    tex_data.offsets = NULL;
    tex_data.texture = NULL;
    extern char (*pnames)[8];
    pnames = NULL;

    char name[9];
    int i;

    for (int l = 0; l < 2; ++l) {

        sprintf (name, "TEXTURE%i", l + 1);
        name[8] = 0;
        i = get_lump_index (name);
        if (i == -1)
            break;

        // get no. textures
        int32_t temp;
        lseek (LUMPS[i].fd, LUMPS[i].position, SEEK_SET);
        read (LUMPS[i].fd, &temp, 4);
        int32_t old_num = tex_data.num_tex;
        tex_data.num_tex += LONG (temp);
        if (tex_data.num_tex <= old_num)
            fatal_error ("read_texture_lump: invalid TEXTURE%i size!", l);

        // get texture offsets
        tex_data.offsets = realloc (tex_data.offsets, 4 * tex_data.num_tex);
        if (tex_data.offsets == NULL)
            fatal_error ("read_texture_lump: realloc failed!");
        read (LUMPS[i].fd, tex_data.offsets+old_num,
            4 * (tex_data.num_tex - old_num));
        for (int n = old_num; n < tex_data.num_tex; ++n)
            tex_data.offsets[n] = LONG (tex_data.offsets[n]);

        // read the actual texture data
        tex_data.texture = realloc (tex_data.texture,
            sizeof(Wall_texture) * tex_data.num_tex);
        if (tex_data.texture == NULL)
            fatal_error ("read_texture_lump: malloc failed!");

        int n;
        Wall_texture *tex;
        for (n = old_num; n < tex_data.num_tex; ++n) {
            lseek (LUMPS[i].fd, LUMPS[i].position + tex_data.offsets[n],
                SEEK_SET);
            tex = tex_data.texture + n;
            read (LUMPS[i].fd, tex, 22); 
            tex->width = SHORT (tex->width);
            tex->height = SHORT (tex->height);
            tex->num_pat = SHORT (tex->num_pat);
            tex->patches = NULL;

            tex->patches = malloc (tex->num_pat * 10);
            for (int x = 0; x < tex->num_pat; ++x) {
                read (LUMPS[i].fd, tex->patches+x, 10);
                tex->patches[x].x_off = SHORT (tex->patches[x].x_off);
                tex->patches[x].y_off = SHORT (tex->patches[x].y_off);
                tex->patches[x].pname_num = SHORT (tex->patches[x].pname_num);
            }
        }
    }

    // now read the picture lumps
    read_picture_lumps();
}

/* read_picture_lumps: read in patches from the WAD */
void read_picture_lumps() {

    // read PNAMES
    extern uint32_t num_pnames;

    int i = get_lump_index ("PNAMES");
    lseek (LUMPS[i].fd, LUMPS[i].position, SEEK_SET);
    read (LUMPS[i].fd, &num_pnames, 4);

    pnames = malloc (8 * num_pnames);
    memset (pnames, 0, 8 * num_pnames);
    read (LUMPS[i].fd, pnames, 8 * num_pnames);


    // read pictures
    extern Raw_picture *R_PICTURES;
    R_PICTURES = malloc (sizeof(Raw_picture) * num_pnames);

    for (i = 0; i < num_pnames; ++i) {

        int pindex = get_lump_index (pnames[i]);
        int fd = LUMPS[pindex].fd;

        Raw_picture *pic = R_PICTURES+i;
        pic->col_offs = NULL;
        pic->cols = NULL;

        // read image header
        lseek (fd, LUMPS[pindex].position, SEEK_SET);
        read (fd, pic, 8);
        pic->width = SHORT (pic->width);
        pic->height = SHORT (pic->height);
        pic->left_off = SHORT (pic->left_off);
        pic->top_off = SHORT (pic->top_off);

        // read column offsets
        pic->col_offs = malloc (4 * pic->width);
        if (pic->col_offs == NULL)
            fatal_error ("draw_texture: malloc failed!");
        read (fd, pic->col_offs, 4 * pic->width);

        // read columns
        pic->cols = malloc (sizeof(Picture_column) * pic->width);
        for (int n = 0; n < pic->width; ++n) {
            lseek (fd, LUMPS[pindex].position + pic->col_offs[n], SEEK_SET);
            read (fd, pic->cols+n, 2);
            pic->cols[n].data = malloc (pic->cols[n].num_pix + 2);
            if (pic->cols[n].num_pix > 0)
                read (fd, pic->cols[n].data, pic->cols[n].num_pix + 2);
        }
    }
}

/* get_full_imgs: copy patch data into full images */
void get_full_imgs() {
/*

    extern Picture *PICTURES;
    PICTURES = malloc (sizeof(Picture) * tex_data.num_tex);
    for (int i = 0; i < tex_data.num_tex; ++i) {

        Picture *pic = PICTURES+i;
        pic->width = tex_data.texture[i].width;
        pic->height = tex_data.texture[i].height;
        fprintf (stderr, "pic #%i (%ix%i)\n", i, pic->width, pic->height);
        pic->data = malloc (sizeof(uint8_t *) * pic->width);
        for (int x = 0; x < pic->width; ++x)
            pic->data[x] = malloc (pic->height);

        // copy patch data
        for (int n = 0; n < tex_data.texture[i].num_pat; ++n) {

            int f = tex_data.texture[i].patches[n].pname_num;
            Raw_picture p = R_PICTURES[f];

            fprintf (stderr, "patch #%i (%ix%i)\n", n, p.width, p.height);

            // draw the image
            for (int x = 0; x < p.width; ++x) {
                fprintf (stderr, "x %i\n", x);//(pic->width - p.left_off) + x);
                fprintf (stderr, "y %i\n", p.top_off + p.cols[x].offset);
//(-p.top_off + n));
                memcpy (pic->data[(pic->width - p.left_off) + x]
                    +p.cols[x].offset, p.cols[x].data+1,
                    p.cols[x].num_pix-2);
            }
        }
    }
    */
}

/* get_tex_index: returns the texture lump index
    of the texture name (or -1 if the texture
    was not found   */
int get_tex_index (const char *name) {

    if (!strncmp (name, "-", 8))
        return -1;

    int index = -1;
    for (int i = 0; index == -1 && i < tex_data.num_tex; ++i)
        if (!strncmp (name, tex_data.texture[i].name, 8))
            index = i;

    return index;
}

/* draw_texture: draw a raw texture to the screen
    FOR DEBUG PURPOSES
    (NOTE : names are SIDEDEF texture names)
    */
void draw_texture (const char *name) {

    print ("%.8s\n", name);

    int index;

    index = get_tex_index (name);

    if (index < 0)
        fatal_error ("Could not find wall texture %.8s!", name);

    print ("%ix%i\n", tex_data.texture[index].width, tex_data.texture[index].height);

    // draw texture patches
    for (int i = 0; i < tex_data.texture[index].num_pat; ++i) {
        // no. 5 has -16 yf

        int f = tex_data.texture[index].patches[i].pname_num;
        int xf, yf;
        xf = tex_data.texture[index].patches[i].x_off;
        yf = tex_data.texture[index].patches[i].y_off;
        print ("%i : %i %i\n", i, xf, yf);

        Raw_picture pic = R_PICTURES[f];

        // draw the image
        for (int m = 0; m < pic.width; ++m) {
            for (int n = 0; n < pic.cols[i].num_pix; ++n) {
                int x, y;
                x = (xf/* + pic.left_off*/) + m;
                y = (yf/* + pic.top_off*/) + pic.cols[m].offset + n;

                if (x < 0 || x > tex_data.texture[index].width
                 || y < 0 || y > tex_data.texture[index].height) {
                    fprintf (stderr, "coords out of range: %i %i (%ix%i)",
                        x, y, tex_data.texture[index].width,
                        tex_data.texture[index].height);
                    continue;
                }
                print ("%i %i\n", x, y);
                draw_pixel (x, y, PALETTE[0][3 * pic.cols[m].data[n+1]]);
//#ifdef __SDL
//                draw_frame();
//#endif
            }
        }
        print ("\n");
    }
}

