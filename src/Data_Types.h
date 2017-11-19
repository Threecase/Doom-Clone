/*
 * Misc data structure definition
 *
 */

#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H


typedef struct {
    int x, y;
} Vertex;



/* THINGS:
    Game entities.
  eg. Barrels, monsters, etc.
  */
typedef struct {
    int x, y;       // x/y coords
    unsigned angle; // angle
    short type;     // thing ID
    char flags;     // flags
} Thing;

/* thing flags */
typedef enum {
    TF_sklvl_12   = 1,
    TF_sklvl_3    = 2,
    TF_sklvl_45   = 4,
    TF_deaf       = 8,
    TF_no_singlep = 16
} Thing_Flags;



/* SECTOR:
    Areas of the map.
  note: a single sector may be used
  in different, disconnected areas
  */
typedef struct {
    short floor_h, ceil_h;      // floor/ceiling height
    char *floor_tex, *ceil_tex; // floor/ceiling texture
    char brightness;            // light level
    char type;                  // sector type
    //char tag; ???
} Sector;

/* sector types */
typedef enum {
    ST_normal,
    ST_rand_blink,
    ST_half_blink,
    ST_full_blink,
    ST_20_dam_half_blink,
    ST_10_dam,
    ST_5_dam,
    ST_oscil_blink,
    ST_ceil_lower_30,
    ST_end_chapter,
    ST_sync_half_blink,
    ST_sync_full_blink,
    ST_ceil_raise_300,
    ST_20_dam,
    ST_rand_flicker
} Sector_Type;



/* SIDEDEFS:
    contains wall texture data.
    */
typedef struct {
    short xoff, yoff;       // x/y offset of texture
    char *upp, *mid, *low;  // texture names
    Sector *sector;         // sector properties
} Sidedef;



/* LINEDEFS:
    basic structure of the map.
    */
typedef struct {
    Vertex *start, *end;    // start/end vertices
    short flags;            // flags
    //short type; ???
    Sector *sector;         // sector lindef is in
    Sidedef sides[2];       // left/right sidedefs
} Linedef;

/* Flags for Linedefs */
typedef enum {
    LD_solid    = 1,
    LD_solid_m  = 2,
    LD_twosided = 4,
    LD_upegtop  = 8,
    LD_upegbot  = 16,
    LD_secret   = 32,
    LD_nosound  = 64,
    LD_nomap    = 128,
    LD_yesmap   = 256
    // 512 - 32768 unused
} Linedef_Flags;



/* SEGS:
    linedef segments, used by the BSP.
    */
typedef struct {
    Vertex *start, *end;// start/end vertices
    unsigned angle;     // angle of the line
    Linedef *line;      // linedef this is a seg of
    char direction;     // left or right side of the linedef?
    //int offset; ??? What is this for?
} Seg;



/* SUBSECTORS:
    used by the BSP (as the polygons to
    be rendered). made up of segs. */
typedef struct {
    Seg *segs;
    int num_segs;
} SSector;



/* NODES:
    the nodes of the BSP Tree.
    */
typedef struct BSP_Node {
    int x, y;
    int dx, dy;
    int l_bbox[4];
    int r_bbox[4];
    struct BSP_Node *r_child;
    struct BSP_Node *l_child;
    SSector *ssectors;
} BSP_Node;


#endif

