/*
 * Misc data structure definition
 *
 */

#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

#include <stdint.h>


/* macros to swap endianness if needed */
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


/* Vertex:
 *
 *  Each vertex is 4 bytes, containing 2 fields:
 *   - x/y location of the vertex
 */
typedef struct
{   int16_t x, y;
} Vertex;



/* Thing_Flags:
 *  
 *  TF_SKILL_LVL_1_2    = Thing only appears on difficulties 1 & 2
 *  TF_SKILL_LVL_3      = Thing only appears on difficulty 3
 *  TF_SKILL_LVL_4_5    = Thing only appears on difficulties 4 & 5
 *  TF_DEAF             = Thing does not activate until it sees the player
 *  TF_MULTIPLAYER_ONLY = Thing only appears in multiplayer mode
 */
typedef enum
{
    TF_SKILL_LVL_1_2    = 0x01,
    TF_SKILL_LVL_3      = 0x02,
    TF_SKILL_LVL_4_5    = 0x04,
    TF_DEAF             = 0x08,
    TF_MULTIPLAYER_ONLY = 0x10,
    /* bits 5-15 unused */
} Thing_Flags;

/* Thing:
 *  Game entities
 *
 *  A thing has 4 fields:
 *   - x/y coordinates
 *   - angle
 *   - an ID, telling which type of thing it is
 *   - flags
 */
typedef struct
{   int16_t  x, y;
    uint16_t angle,
             type,
             flags;
} Thing;



/* Sector_Type:
 *  Special Sector types
 *
 *
 *  ST_NORMAL            = No special effects
 *  ST_RAND_BLINK        = Brightness switches from high to low randomly
 *  ST_HALF_BLINK        = Brightness switches from high to low every half second
 *  ST_FULL_BLINK        = Brightness switches from high to low every second
 *  ST_20_DAM_HALF_BLINK = Brightness switches from high to low every half second,
 *                         and player takes 20 damage per second (10 on difficulty 1)
 *  ST_10_DAM            = Player takes 10 damage per second (5 on difficulty 1)
 *  ST_5_DAM             = Player takes 5 damage per second (2 on difficulty 1)
 *  ST_OSCILLATE         = Brightness goes smoothly from high to low after ~1sec
 *  ST_SECRET            = Standing in this sector triggers a Secret
 *  ST_CEIL_LOWER_30     = 30sec after level start, the ceiling lowers like a door
 *  ST_END_CHAPTER       =
 *  ST_SYNC_HALF_BLINK   = Brightness switches from high to low randomly
 *  ST_SYNC_FULL_BLINK   = Brightness switches from high to low randomly
 *  ST_CEIL_RAISE_300    =
 *  ST_20_DAM            =
 *  ST_RAND_FLICKER      =
 *
 *  NOTE: see 1791 for in-depth explanation of these
 */
typedef enum
{
    ST_NORMAL            = 0x00,
    ST_RAND_BLINK        = 0x01,
    ST_HALF_BLINK        = 0x02,
    ST_FULL_BLINK        = 0x03,
    ST_20_DAM_HALF_BLINK = 0x04,
    ST_10_DAM            = 0x05,
    ST_5_DAM             = 0x07,
    ST_OSCILLATE         = 0x08,
    ST_SECRET            = 0x09,
    ST_CEIL_LOWER_30     = 0x0a,
    ST_END_CHAPTER       = 0x0b,
    ST_SYNC_HALF_BLINK   = 0x0c,
    ST_SYNC_FULL_BLINK   = 0x0d,
    ST_CEIL_RAISE_300    = 0x0e,
    ST_20_DAM            = 0x0f,
    ST_RAND_FLICKER      = 0x10,
} Sector_Type;

/* Sector:
 *  Areas of the map.
 *  note: a single sector may be used
 *  in different, disconnected areas
 *
 *  Each Sector is 26 bytes, in 7 fields:
 *   - Floor/ceiling heights
 *   - Floor/ceiling texture names
 *   - Brightness of the Sector
 *   - Special Sector type
 *   - Tag number (corresponding with the `tag' field in a Linedef)
 */
typedef struct
{
    int16_t     floor_h,
                ceil_h;
    char        floor_tex[8 + 1],
                 ceil_tex[8 + 1];
    uint16_t    brightness;
    Sector_Type type;
    uint16_t    tag;
} Sector;



/* Sidedef:
 *  Contains wall texture data.
 *
 *  A sidedef is 30 bytes, containing 6 fields:
 *   - x/y offset for the texture
 *   - upper texture name (or '-' for none)
 *   - lower texture name (or '-' for none)
 *   - middle texture name
 *   - number of the sector which this sidedef faces
 *
 * NOTE: see ~1464 in DOOM Spec for details on texturing
 */
typedef struct
{
    int16_t xoff,
            yoff;
    char    top_tex[8 + 1],
            low_tex[8 + 1],
            mid_tex[8 + 1];
    uint16_t sector;
} Sidedef;



/* Linedef_Flags
 *
 *
 *  LD_IMPASSABLE       = Monsters and players cannot pass this line
 *  LD_BLOCK_MONSTERS   = Monsters cannot pass this line
 *  LD_TWO_SIDED        = This line is 'transparent', meaning it can have a
 *                        transparent texture, bullets can pass through it,
 *                        and monsters will see and attack through this line.
 *  LD_TOP_UNPEGGED     = TODO -- texturing stuff; will look at later
 *  LD_BOT_UNPEGGED     = TODO -- texturing stuff; will look at later
 *  LD_SECRET           = Appears as a regular wall on the automap, used to hide secret areas
 *                        (have a peek at line 1052 in DOOM Spec for details)
 *  LD_BLOCK_SOUND      = Blocks sound from traveling through it
 *                        (sound can pass through one of these lines,
 *                        but dies after the second one)
 *  LD_UNMAPPED         = This line is not shown on the automap, even
 *                        if the show map powerup is acquired
 *  LD_MAPPED           = This line is always on the automap,
 *                        even if it hasn't been seen yet
 */
typedef enum
{
    LD_IMPASSABLE       = 0x001,
    LD_BLOCK_MONSTERS   = 0x002,
    LD_TWO_SIDED        = 0x004,
    LD_TOP_UNPEGGED     = 0x008,
    LD_BOT_UNPEGGED     = 0x010,
    LD_SECRET           = 0x020,
    LD_BLOCK_SOUND      = 0x040,
    LD_UNMAPPED         = 0x080,
    LD_MAPPED           = 0x100,
    /* bits 9-15 unused */
} Linedef_Flags;

/* Linedef:
 *  Basic structure of the map.
 *
 *  Each linedef is 14 bytes, containing 7 fields:
 *   - start and end vertex numbers
 *   - flags
 *   - type
 *   - tag number
 *   - right and left sidedefs
 *
 * All linedefs MUST have a right side. If there
 * were a linedef without a right side, the original
 * DOOM.EXE would crash.
 *
 *
 *          left side               right side
 * start -----------------> end <----------------- start
 *          right side              left side
 */
typedef struct
{
    uint16_t      start,
                  end;
    Linedef_Flags flags;    /* 2 bytes */
    uint16_t      type;
    uint16_t      tag;
    int16_t       right,
                  left;
} Linedef;



/* Seg:
 *  Linedef segments, used by the BSP.
 *
 *  Each Seg is 12 bytes, containing 6 fields:
 *   - Numbers of the start and end vertices
 *   - Angle (0x0000 = east, 0x4000 = north,
 *            0x8000 = west, 0xC000 = south)
 *   - Linedef number this segment is along
 *   - Direction of the seg: 0 = RIGHT side, 1 = LEFT side
 *   - Distance along the linedef to the start of the Seg (???)
 */
typedef struct
{
    uint16_t start, end;    /* start/end vertices */
    int16_t angle;          /* angle of the line */
    uint16_t linedef;       /* linedef this is a seg of */
    uint16_t direction;     /* left(0) or right(1) side of the linedef? */
    int16_t offset;         /* offset to start the seg at */
} Seg;



/* SSector:
 *  Used by the BSP (as the polygons to
 *  be rendered). Made up of segs.
 *
 *  Each SSector is 4 bytes in 2 fields:
 *   - Number of Segs in the SSector
 *   - Number of the first Seg
 */
typedef struct
{
    uint16_t num_segs;
    uint16_t start_seg;
} SSector;



/* Node:
 *  The nodes of the BSP Tree.
 *
 *  Each Node is 28 bytes in 14 fields:
 *   - x/y coord of partition line start
 *   - deltax/deltay of partition line
 *   - right bounding box ==> upper Y, lower Y, lower X, upper X
 *   - left  bounding box ==> upper Y, lower Y, lower X, upper X
 *   - Node or SSector number of the right/left child
 *     (if bit 15 of the number is 1, then the number
 *      is an SSector; otherwise it is a Node number)
 */
typedef struct
{
    int16_t  x,  y;
    int16_t dx, dy;

    int16_t right_bbox[4];
    int16_t  left_bbox[4];

    uint16_t right_child,
              left_child;
} Node;



/* TODO: properly implement these */
/* REJECT:
 *  size = ceil ((num_sectors^2) / 8)
 *  array of bits
 *  note: read left-to-right (least-to-most significant)
 */

/* BLOCKMAP:
 *  used for collisions (walls + things)
 */

typedef struct
{
    int16_t x, z;                   /* bottom left corner of the blockmap */
    uint16_t x_blocks, z_blocks;    /* number of blocks in the blockmap */
} BMap_Header;

typedef struct
{
    BMap_Header head;
    uint16_t **offset;      /* lseek offset of blockLIST (*NOT* block or blockmap)
                                (goes east [+x], then up [-z])  */
    uint16_t *blocklists;   /* starts with 0, ends with -1 (empty is [0, -1]) */
} BMap;



/* Picture:
 *  DOOM picture (sprite, texture, etc.)
 *
 *  Each picture is made up of 5 fields.
 *   - width of the picture
 *   - height of the picture
 *   - left offset; the number of columns left of the origin
 *   - top offset; the number of rows above the origin
 *   - columns of pixel data
 *
 *  The pixel data columns are structured as such:
 *   - 1 byte describing how many pixels to move down from top_offset
 *   - 1 byte count of the number of non-transparent pixels in the column
 *   - 1 byte which does nothing
 *   - N bytes of colour indices
 *   - 1 byte which does nothing
 *
 */
typedef struct
{
    uint16_t width, height,
             left_offset, top_offset;
    int          *post_count;
    struct Post **columns;              /* WIDTHxHEIGHT */
} Picture;

typedef struct Post
{
    uint8_t  top_offset;
    uint8_t  pixel_count;
    uint8_t *pixels;
} Post;





/* Point:
 *  3D point
 */
typedef struct Point
{   float x, y, z;
} Point;


#endif

