/*
 * Drawing utilities
 *
 */

#ifndef _TEXTURE_H
#define _TEXTURE_H


#include <stdint.h>



typedef struct {
    int8_t offset;
    uint8_t num_pix;
    uint8_t *data;      // NOTE : first and last bytes unused
} Picture_column;

typedef struct {
    int16_t width;
    int16_t height;
    int16_t left_off;
    int16_t top_off;    /* if left_off and top_off are negative,
                            they are absolute coordinates from the
                            top left of the screen (320x200)
                            */
    uint32_t *col_offs;
    Picture_column *cols;
} Raw_picture;

typedef struct {
    int16_t x_off;
    int16_t y_off;
    int16_t pname_num;
    uint16_t pad1;          // unused
    uint16_t pad2;          // unused
} Wall_patch;

typedef struct {
    char name[8];
    uint16_t pad1[2];       // unused
    int16_t width;
    int16_t height;
    uint16_t pad2[2];       // unused
    int16_t num_pat;
    Wall_patch *patches;
} Wall_texture;

typedef struct {
    int32_t num_tex;
    int32_t *offsets;
    Wall_texture *texture;
} Texture_lump;

typedef struct {
    int16_t width, height;
    uint8_t **data;
} Picture;


// VARIABLES
Texture_lump tex_data;

uint32_t num_pnames;
char (*pnames)[8];

Raw_picture *R_PICTURES;
Picture *PICTURES;


// FUNCTIONS
void read_texture_lump();

void get_full_imgs();

int get_tex_index (const char *name);

void draw_texture (const char *name);


#endif

