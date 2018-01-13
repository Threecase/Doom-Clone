/*
 * Drawing utilities
 *
 */

#ifndef _TEXTURE_H
#define _TEXTURE_H


#include <stdint.h>


// defines


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
    uint32_t *pointers;    // length of this = width
} Picture;

typedef struct {
    int16_t x_off;
    int16_t y_off;
    int16_t pname_num;
    uint32_t pad;           // unused
} Wall_patch;

typedef struct {
    char name[8];
    uint32_t pad_1;         // unused
    int16_t width;
    int16_t height;
    uint32_t pad_2;         // unused
    int16_t num_pat;
    Wall_patch *patches;
} Wall_texture;

typedef struct {
    int32_t num_tex;
    int32_t *offsets;
    Wall_texture *texture;
} Texture_lump;

// VARIABLES
Texture_lump tex_data[2];
char (*pnames)[8];

// FUNCTIONS
void read_texture_lump();

void draw_texture (const char *name);


#endif

