/*
 * Texturing utilities
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "texture.h"
#include "read_wad.h"
#include "rawterm.h"



/* read_texture_lump: reads the TEXTURE lump(s) */
void read_texture_lump() {

    extern Texture_lump tex_data[2];
    tex_data[0].num_tex = tex_data[1].num_tex = 0;
    tex_data[0].offsets = tex_data[1].offsets = NULL;
    tex_data[0].texture = tex_data[1].texture = NULL;
    extern char (*pnames)[8];
    pnames = NULL;

    char name[9];
    int i;

    for (int l = 0; l < 2; ++l) {

        sprintf (name, "TEXTURE%i", l + 1);
        name[8] = 0;
        i = get_lump_index (name);

        // get no. textures
        lseek (LUMPS[i].fd, LUMPS[i].position, SEEK_SET);
        read (LUMPS[i].fd, &tex_data[l].num_tex, 4);
        tex_data[l].num_tex = LONG (tex_data[l].num_tex);

        // get texture offsets
        tex_data[l].offsets = malloc (4 * tex_data[l].num_tex);
        if (tex_data[l].offsets == NULL)
            fatal_error ("read_texture_lump: malloc failed!");

        read (LUMPS[i].fd, tex_data[l].offsets, 4 * tex_data[l].num_tex);
        for (int n = 0; n < tex_data[l].num_tex; ++n)
            tex_data[l].offsets[n] = LONG (tex_data[l].offsets[n]);

        // read the actual texture data
        tex_data[l].texture = malloc (sizeof(Wall_texture) * tex_data[l].num_tex);
        if (tex_data[l].texture == NULL)
            fatal_error ("read_texture_lump: malloc failed!");

        int n;
        for (n = 0; n < tex_data[l].num_tex-1; ++n) {
            read (LUMPS[i].fd, tex_data[l].texture + n, 22); 
            int amnt = (tex_data[l].offsets[n+1] - tex_data[l].offsets[n]) - 22;
            tex_data[l].texture[n].patches = malloc (amnt);
            read (LUMPS[i].fd, tex_data[l].texture[n].patches, amnt);
            tex_data[l].texture[n].width = SHORT (tex_data[l].texture[n].width);
            tex_data[l].texture[n].height = SHORT (tex_data[l].texture[n].height);
            tex_data[l].texture[n].num_pat = SHORT (tex_data[l].texture[n].num_pat);
        }
        read (LUMPS[i].fd, tex_data[l].texture + n, 22); 
        int amnt = (LUMPS[i].size - tex_data[l].offsets[n]) - 22;
        tex_data[l].texture[n].patches = malloc (amnt);
        read (LUMPS[i].fd, tex_data[l].texture[n].patches, amnt);
        tex_data[l].texture[n].width = SHORT (tex_data[l].texture[n].width);
        tex_data[l].texture[n].height = SHORT (tex_data[l].texture[n].height);
        tex_data[l].texture[n].num_pat = SHORT (tex_data[l].texture[n].num_pat);
    }


    // read PNAMES
    uint32_t num_pnames;
    i = get_lump_index ("PNAMES");
    lseek (LUMPS[i].fd, LUMPS[i].position, SEEK_SET);
    read (LUMPS[i].fd, &num_pnames, 4);

    pnames = malloc (8 * num_pnames);
    memset (pnames, 0, 8 * num_pnames);
    read (LUMPS[i].fd, pnames, 8 * num_pnames);
}

/* draw_texture: draw a texture to the screen
    FOR DEBUG PURPOSES
    (NOTE : names are SIDEDEF texture names)
    */
void draw_texture (const char *name) {

    int index;
    int fd;
    int l;

    if (!strncmp (name, "-", 8)) {
        raw_writes ("No texture!\n\r");
        return;
    }

    // get root texture index
    index = -1;
    for (l = 0; index == -1 && l < 2; ++l)
        for (int i = 0; index == -1 && i < tex_data[l].num_tex; ++i)
            if (!strncmp (name, tex_data[l].texture[i].name, 8))
                index = i;
    l--;

    if (index < 0)
        fatal_error ("Could not find wall texture %.8s!", name);


    // read image data from WAD
    for (int i = 0; i < tex_data[l].texture[index].num_pat; ++i) {

        raw_writes ("patch num : %i\n\r", i);

        // get image name
        Picture pic;
        pic.pointers = NULL;
        char patch_n[9];
        memset (patch_n, 0, 9);
        // DOORTRAK has negative pname i
        raw_writes ("patch data : xoff %i yoff %i num %i\n\r", tex_data[l].texture[index].patches[i].x_off, tex_data[l].texture[index].patches[i].y_off, tex_data[l].texture[index].patches[i].pname_num);

        if (tex_data[l].texture[index].patches[i].pname_num < 0)
            fatal_error ("draw_texture: invalid pname index (%i)!", tex_data[l].texture[index].patches[i].pname_num);
        strncpy (patch_n, pnames[tex_data[l].texture[index].patches[i].pname_num], 8);
        int pindex = get_lump_index (patch_n);
        fd = LUMPS[pindex].fd;
        raw_writes ("img name : '%.8s'\n\r", patch_n);

        // read image header
        lseek (fd, LUMPS[pindex].position, SEEK_SET);
        read (fd, &pic, 8);
        pic.width = SHORT (pic.width);
        pic.height = SHORT (pic.height);
        pic.left_off = SHORT (pic.left_off);
        pic.top_off = SHORT (pic.top_off);

        // read PLAYPAL offsets
        pic.pointers = malloc (4 * pic.width);
        if (pic.pointers == NULL)
            fatal_error ("draw_texture: malloc failed!");
        read (fd, pic.pointers, 4 * pic.width);

        // read colour data
        Picture_column *cols = malloc (sizeof(Picture_column) * pic.width);
        for (int n = 0; n < pic.width; ++n) {
            lseek (fd, LUMPS[pindex].position + pic.pointers[n], SEEK_SET);
            read (fd, cols+n, 2);
            cols[n].data = malloc (cols[n].num_pix + 2);
            if (cols[n].num_pix > 0)
                read (fd, cols[n].data, cols[n].num_pix + 2);
        }
        raw_writes ("read %i cols\n\r", pic.width);

        // draw the image
        raw_writes ("Drawing texture '%s' (%.8s)\n\r", patch_n, name);
        for (int m = 0; m < pic.width; ++m) {
            for (int n = 1; n < cols[i].num_pix-1; ++n) {
                draw_pixel (PALETTE[0]+(3 * cols[m].data[n]),
                    683 + (tex_data[l].texture[index].width - pic.left_off) + m,
                    384 + (-pic.top_off + n));
            }
        }

        for (int n = 0; n < pic.width; ++n)
            free (cols[n].data);
        free (cols);
        free (pic.pointers);
    }
}

