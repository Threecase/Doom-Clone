/*
 * Misc data structure definition
 *
 */

#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

#include <stdint.h>


// swap endianness if needed
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define SHORT(x)    ((x >> 8) | (x << 8))
#define LONG(x)     ((x >> 24)\
                    | ((x >> 8) & 0xff00)\
                    | ((x << 8) & 0xff0000)\
                    | (x << 24))

#else

#define SHORT(x)    (x)
#define LONG(x)     (x)

#endif


/* VERTEX:
    most basic part of the map,
    just two x/y coords.
    */
typedef struct {
    int16_t x, z;
} Vertex;



/* THINGS:
    Game entities.
  eg. Barrels, monsters, etc.
  */
typedef struct {
    int16_t x, z;   // x/y (z) coords
    uint16_t angle; // angle
    uint16_t type;  // thing ID
    uint16_t flags; // flags
} Thing;

/* thing flags */
typedef enum {
    TF_sklvl_12   = 1,
    TF_sklvl_3    = 2,
    TF_sklvl_45   = 4,
    TF_deaf       = 8,
    TF_no_singlep = 16
    // bits 5-16 (32 - 65536) unused
} Thing_Flags;



/* SECTOR:
    Areas of the map.
  note: a single sector may be used
  in different, disconnected areas
  */
typedef struct {
    int16_t floor_h, ceil_h;    // floor/ceiling height
    char floor_tex[8];          // floor texture
    char ceil_tex[8];           // ceiling texture
    uint16_t brightness;        // light level
    uint16_t type;              // sector type
    uint16_t tag;               /*  same as Linedef type, sectors
                                     with the same tag as the
                                     Linedef will be affected       */
} Sector;

/* sector types */
typedef enum {
    ST_normal = 0,
    ST_rand_blink,
    ST_half_blink,
    ST_full_blink,
    ST_20_dam_half_blink,   // 10% on skill 1, else 20%
    ST_10_dam,              // 5%  on skill 1, else 10%
    ST_5_dam = 7,           // 2%  on skill 1, else 5%
    ST_oscillate,
    ST_Secret,
    ST_ceil_lower_30,       // after 30s, this ceiling closes
    ST_end_chapter,         /* 10% on skill 1, else 20%,
                                if player health < 11%, the level ends */
    ST_sync_half_blink,
    ST_sync_full_blink,
    ST_ceil_raise_300,      // after 300s, this ceiling raises
    ST_20_dam = 16,         // 10% on skill 1, else 20%
    ST_rand_flicker
} Sector_Type;



/* SIDEDEFS:
    contains wall texture data.
    */
typedef struct {
    int16_t xoff;
    int16_t yoff;       // x/y offset of texture
    char top_tex[8];    // top texture    ("-" for none)
    char mid_tex[8];    // middle texture
    char low_tex[8];    // bottom texture ("-" for none)
    uint16_t sector;    // front sector
} Sidedef;



/* LINEDEFS:
    basic structure of the map.
    */
typedef struct {
    uint16_t start;
    uint16_t end;           // start/end vertices
    uint16_t flags;         // flags
    uint16_t type;          // type (ie door, elevator, etc.)
    uint16_t tag;           // sector tag
    int16_t sides[2];       // right/left sidedefs (all lines have a right side)
} Linedef;

/* Flags for Linedefs */
typedef enum {
    LD_impassable       = 1,
    LD_block_monster    = 2,
    LD_two_sided        = 4,
    LD_top_unpeg        = 8,
    LD_bot_unpeg        = 16,
    LD_secret           = 32,
    LD_block_sound      = 64,
    LD_no_map           = 128,
    LD_on_map           = 256
    // bits 9-15 (512 - 32768) unused
} Linedef_Flags;



/* SEGS:
    linedef segments, used by the BSP.
    */
typedef struct {
    uint16_t start, end;    // start/end vertices
    int16_t angle;          // angle of the line
    uint16_t line;          // linedef this is a seg of
    uint16_t direction;     // left(0) or right(1) side of the linedef?
    int16_t offset;         // offset to start the seg at
} Seg;



/* SUBSECTORS:
    used by the BSP (as the polygons to
    be rendered). made up of segs. */
typedef struct {
    uint16_t num_segs;
    uint16_t start_seg;
} SSector;



/* NODES:
    the nodes of the BSP Tree.
    */
typedef struct {
    int16_t x, z;
    int16_t dx, dz;

    int16_t bbox[2][4];  // bounding boxes (top, bottom, left, right)

    uint16_t child[2];  // l/r children
} Node;



/* REJECT:
    size = ceil ((num_sectors^2) / 8)
    array of bits
    note: read left-to-right (least-to-most significant)    */

/* BLOCKMAP:
    used for collisions (walls + things)    */

typedef struct {
    int16_t x, z;                   // bottom left corner of the blockmap
    uint16_t x_blocks, z_blocks;    // number of blocks in the blockmap
} BMap_Header;

typedef struct {
    BMap_Header head;
    uint16_t **offset;  /* lseek offset of blockLIST (*NOT* block or blockmap)
                            (goes east [+x], then up [_z])  */
    uint16_t *blocklists;   // starts with 0, ends with -1 (empty is [0, -1])
} BMap;


#endif

