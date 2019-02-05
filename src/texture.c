


#include "data_types.h"
#include "read_wad.h"

#include "drawing.h"    /* FIXME: TEMP(?) */

#include <stdio.h>
#include <stdlib.h>



/* Texture:
 *  A Sidedef wall texture;
 *  made up of wall patches
 */
typedef struct Texture
{
    char name[8 + 1];
    uint16_t width,
             height;

    uint16_t patch_count;
    struct PatchDef *patches;
} Texture;

/* PatchDef:
 *  Wall patch definition
 *  (only used in Textures)
 */
typedef struct PatchDef
{
    uint16_t x_offset,
             y_offset;
    uint16_t pname_number;
} PatchDef;


static uint32_t number_of_textures = 0;
static Texture *textures = NULL;

static uint32_t pname_count = 0;
static char (*pnames)[8 + 1] = NULL;



/* init_texture_directories:  */
void init_texture_directories(void)
{
    /* TODO: read TEXTURE2 as well */
    uint8_t *data = read_lump ("TEXTURE1");

    number_of_textures = ((uint32_t*)data)[0];
    textures = calloc (sizeof(*textures), number_of_textures);

    for (unsigned i = 0; i < number_of_textures; ++i)
    {
        uint32_t texture_ptr = (((uint32_t*)data) + 1)[i];

        uint8_t *texdef = data + texture_ptr;

        memcpy (textures[i].name, texdef, 8);
        textures[i].name[8] = '\0';
        //debug ("TEXTURE1[%u] = '%s'", i, textures[i].name);

        texdef += 8;
        texdef += 4;

        textures[i].width  = *((uint16_t*)texdef);
        texdef += 2;
        textures[i].height = *((uint16_t*)texdef);
        texdef += 2;
        texdef += 4;

        textures[i].patch_count = *((uint16_t*)texdef);
        textures[i].patches = calloc (sizeof(*textures[i].patches),
                                      textures[i].patch_count);

        for (int p = 0; p < textures[i].patch_count; ++p)
        {
            uint8_t *patch = texdef + (10 * p);

            textures[i].patches[p].x_offset     = ((uint16_t*)patch)[0];
            textures[i].patches[p].y_offset     = ((uint16_t*)patch)[1];
            textures[i].patches[p].pname_number = ((uint16_t*)patch)[2];
        }
    }
}

/* init_pnames:  */
void init_pnames(void)
{
    uint8_t *data = read_lump ("PNAMES");

    pname_count = ((uint32_t*)data)[0];
    pnames = calloc (sizeof(*pnames), pname_count);

    for (unsigned i = 0; i < pname_count; ++i)
    {
        memcpy (pnames[i], (data+4) + (i * 8), 8);
        pnames[i][8] = '\0';
        //debug ("PNAMES[%u] = '%s'", i, pnames[i]);
    }
}

/* read_picture: convert WAD picture data to internal Picture data format */
Picture read_picture (uint8_t *data)
{
    Picture p = { 0 };

    p.width       = ((uint16_t*)data)[0];
    p.height      = ((uint16_t*)data)[1];
    p.left_offset = ((uint16_t*)data)[2];
    p.top_offset  = ((uint16_t*)data)[3];

    p.post_count   = calloc (sizeof(*p.post_count), p.width);
    p.columns      = calloc (sizeof(*p.columns), p.width);

    uint32_t *ptrs = calloc (sizeof(*ptrs), p.width);

    /* get the data for each column */
    for (int i = 0; i < p.width; ++i)
    {
        uint8_t *column = data + ptrs[i];

        int post_i = 0;
        struct Post *posts = NULL;

        /* get the post data for the column */
        do
        {   posts = realloc (posts, sizeof(*posts) * (post_i + 1));

            struct Post *post = posts + post_i;

            post->top_offset  = *(column++);
            post->pixel_count = *(column++);
            
            post->pixels = calloc (sizeof(*(posts->pixels)),
                                   post->pixel_count);

            /* skip the null byte before the pixel data */
            column++;
            memcpy (post->pixels, column, post->pixel_count);
            /* there's also an unused byte AFTER the pixel data */
            column += post->pixel_count + 1;

        } while (*column != 0xFF);

        p.post_count[i] = post_i + 1;
        p.columns[i] = posts;
    }
    return p;
}

/* convert_picture_to_pixels:  */
Colour *convert_picture_to_pixels (Picture pic)
{
    uint8_t *data = malloc (pic.width * pic.height);

    for (int i = 0; i < pic.width; ++i)
    {
        Post *column = pic.columns[i];

        uint8_t *column_pixels = data + (i * pic.height);


        for (int p = 0; p < pic.post_count[i]; ++p)
        {
            Post po = column[p];

            for (int y = 0; y < po.pixel_count; ++y)
            {
                column_pixels[po.top_offset + y] = po.pixels[y];
            }
        }
    }


    Colour *pixels = calloc (sizeof(*pixels), pic.width * pic.height);

    for (int y = 0; y < pic.height; ++y)
    {
        for (int x = 0; x < pic.width; ++x)
        {
            uint8_t pix = data[(y * pic.width) + x];
            pixels[(y * pic.width) + x] = PALETTE[0][pix];
        }
    }

    return pixels;
}

/* get_texture: get the texture data */
Colour *get_texture (char *const texture)
{
    Colour *data = NULL;

    /* find the texture */
    Texture *tex = NULL;
    for (unsigned i = 0; i < number_of_textures; ++i)
    {
        if (strcmp (texture, textures[i].name) == 0)
        {
            tex = &textures[i];
            break;
        }
    }
    if (tex == NULL)
    {   fatal_error ("failed to find Sidedef texture '%s'", texture);
        return NULL;
    }

    debug ("texture '%s' is %dx%d pixels",
           texture, tex->width, tex->height);
    /* alloc memory for raw texture data */
    data = calloc (sizeof(*data), tex->width * tex->height);

    /* write the patches into the texture memory */
    for (unsigned i = 0; i < tex->patch_count; ++i)
    {
        PatchDef p = tex->patches[i];

        char *const pname = pnames[p.pname_number];
        uint8_t *patch_data = read_lump (pname);
        Picture pic = read_picture (patch_data);
        Colour *pic_data = convert_picture_to_pixels (pic);


        for (int x = 0; x < pic.width; ++x)
        {
            for (int y = 0; y < pic.height; ++y)
            {
                Colour pic_pixel = pic_data[(pic.width * y) + x];

                int abs_x = (p.x_offset + pic.width) - pic.left_offset;
                int abs_y = (p.x_offset + pic.width) - pic.left_offset;

                data[((abs_y + y) * tex->width) + (abs_x + x)] = pic_pixel;
            }
        }
    }

    return data;
}

/* get_middle_texture: returns the middle texture of a sidedef */
Colour *get_middle_texture (Sidedef sdef)
{
    /* if the sidedef has no middle texture, return empty */
    if (strcmp (sdef.mid_tex, "-") == 0)
    {   return NULL;
    }
    Colour *data = get_texture (sdef.mid_tex);
    return data;
}

