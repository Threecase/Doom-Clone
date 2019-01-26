/*
 * Interpret the info from the WAD.
 * (ie generate map, etc.)
 *
 */

#include "wad_info.h"
#include "data_types.h"


/* byte sizes of each data type in the WAD */
#define THING_SIZE      10
#define LINEDEF_SIZE    14
#define SIDEDEF_SIZE    30
#define VERTEX_SIZE      4
#define SEG_SIZE        12
#define SSECTOR_SIZE     4
#define NODE_SIZE       28
#define SECTOR_SIZE     26
/* TODO: implement these two */
#define REJECT_SIZE      1
#define BLOCKMAP_SIZE    1


int generic_read (char lvl_name[5], char lump_name[9], uint8_t **data,
                  int data_size);
int read_things   (char lvl_name[5], Thing   **data);
int read_linedefs (char lvl_name[5], Linedef **data);
int read_sidedefs (char lvl_name[5], Sidedef **data);
int read_vertices (char lvl_name[5], Vertex  **data);
int read_segs     (char lvl_name[5], Seg     **data);
int read_ssectors (char lvl_name[5], SSector **data);
int read_nodes    (char lvl_name[5], Node    **data);
int read_sectors  (char lvl_name[5], Sector  **data);



/* read_map_data: read the map data from the WAD */
/* TODO: episode number */
/* TODO: add reject and blockmap once implemented */
Level read_map_data (int lvl_num)
{
    Level level = { 0 };

    void *data[10];
    int   length[10];

    char *lump_name[10] =
      { "THINGS", "LINEDEFS", "SIDEDEFS", "VERTEXES", "SEGS",
        "SSECTORS", "NODES", "SECTORS", "REJECT", "BLOCKMAP" };

    int typesize[10] =
     {  THING_SIZE, LINEDEF_SIZE, SIDEDEF_SIZE, VERTEX_SIZE,
        SEG_SIZE, SSECTOR_SIZE, NODE_SIZE, SECTOR_SIZE,
        REJECT_SIZE, BLOCKMAP_SIZE
     };

    char lvl_name[5];
    sprintf (lvl_name, "E1M%i", lvl_num);

    for (int n = 0; n < 10; ++n)  /* FIXME : read the REJECT and BLOCKMAP */
    {
        int idx = get_lump_from_level (lvl_name, lump_name[n]);

        /* calculate the number of items in the lump */
        int size = get_lump_size (idx);
        length[n] = size / typesize[n];

        if (length[n] != (float)size / (float)typesize[n])
        {
            fatal_error ("Bad size for %s lump in level %s -- %d/%d = %f",
                   lump_name[n], lvl_name, size, typesize[n],
                   (float)size / (float)typesize[n]);
        }

        /* read the data from each lump */
        data[n] = malloc (size);
        read_lump_index (idx, data[n]);
    }

    level.num_things   = read_things   (lvl_name, &level.things);
    level.num_linedefs = read_linedefs (lvl_name, &level.linedefs);
    level.num_sidedefs = read_sidedefs (lvl_name, &level.sidedefs);
    level.num_vertices = read_vertices (lvl_name, &level.vertices);
    level.num_segs     = read_segs     (lvl_name, &level.segs);
    level.num_ssectors = read_ssectors (lvl_name, &level.ssectors);
    level.num_nodes    = read_nodes    (lvl_name, &level.nodes);
    level.num_sectors  = read_sectors  (lvl_name, &level.sectors);

    return level;
}

/* generic_read: read a lump */
int generic_read (char lvl_name[5], char lump_name[9], uint8_t **data,
                  int data_size)
{
    int idx = get_lump_from_level (lvl_name, lump_name);

    /* calculate the number of items in the lump */
    int size = get_lump_size (idx);
    int length = size / data_size;

    if (length != (float)size / (float)data_size)
    {
        fatal_error ("Bad size for %s lump in level %s -- %d/%d = %f",
               lump_name, lvl_name, size, data_size,
               (float)size / (float)data_size);
    }

    /* read data from the lump */
    (*data) = malloc (size);
    read_lump_index (idx, *data);

    return length;
}

/* read_things: read the THINGS lump */
int read_things (char lvl_name[5], Thing **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "THINGS", &raw_data, THING_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (THING_SIZE * i);

        (*data)[i].x = *(int16_t*)ptr;  ptr += 2;
        (*data)[i].y = *(int16_t*)ptr;  ptr += 2;

        (*data)[i].angle = *(uint16_t*)ptr;    ptr += 2;
        (*data)[i].type  = *(uint16_t*)ptr;    ptr += 2;
        (*data)[i].flags = *(uint16_t*)ptr;
    }
    return length;
}

/* read_linedefs: read the LINEDEFS lump */
int read_linedefs (char lvl_name[5], Linedef **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "LINEDEFS", &raw_data,
                               LINEDEF_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (LINEDEF_SIZE * i);

        (*data)[i].start = *(uint16_t*)ptr;     ptr += 2;
        (*data)[i].end   = *(uint16_t*)ptr;     ptr += 2;

        (*data)[i].flags = *(uint16_t*)ptr;     ptr += 2;
        (*data)[i].type  = *(uint16_t*)ptr;     ptr += 2;
        (*data)[i].tag   = *(uint16_t*)ptr;     ptr += 2;

        (*data)[i].right = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].left  = *(int16_t*)ptr;
    }
    return length;
}

/* read_sidedefs: read the SIDEDEFS lump */
int read_sidedefs (char lvl_name[5], Sidedef **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "SIDEDEFS", &raw_data, SIDEDEF_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (SIDEDEF_SIZE * i);

        (*data)[i].xoff = *(int16_t*)ptr;   ptr += 2;
        (*data)[i].yoff = *(int16_t*)ptr;   ptr += 2;

        memcpy ((*data)[i].top_tex, (char*)ptr, 8);
        (*data)[i].top_tex[8] = '\0';
        ptr += 8;

        memcpy ((*data)[i].low_tex, (char*)ptr, 8);
        (*data)[i].low_tex[8] = '\0';
        ptr += 8;

        memcpy ((*data)[i].mid_tex, (char*)ptr, 8);
        (*data)[i].mid_tex[8] = '\0';
        ptr += 8;

        (*data)[i].sector_num = *(uint16_t*)ptr;
    }
    return length;
}

/* read_vertices: read the VERTEXES lump */
int read_vertices (char lvl_name[5], Vertex **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "VERTEXES", &raw_data, VERTEX_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (VERTEX_SIZE * i);

        (*data)[i].x = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].y = *(int16_t*)ptr;
    }
    return length;
}

/* read_segs: read the SEGS lump */
int read_segs (char lvl_name[5], Seg **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "SEGS", &raw_data, SEG_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (SEG_SIZE * i);

        (*data)[i].start = *(uint16_t*)ptr;     ptr += 2;
        (*data)[i].end   = *(uint16_t*)ptr;     ptr += 2;

        (*data)[i].angle     = *(uint16_t*)ptr;     ptr += 2;
        (*data)[i].line      = *(uint16_t*)ptr;     ptr += 2;
        (*data)[i].direction = *(uint16_t*)ptr;     ptr += 2;

        (*data)[i].offset    = *(int16_t*)ptr;
    }
    return length;
}

/* read_ssectors: read the SSECTORS lump */
int read_ssectors (char lvl_name[5], SSector **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "SSECTORS", &raw_data, SSECTOR_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (SSECTOR_SIZE * i);

        (*data)[i].num_segs  = *(uint16_t*)ptr;     ptr += 2;
        (*data)[i].start_seg = *(uint16_t*)ptr;
    }
    return length;
}

/* read_nodes: read the NODES lump */
int read_nodes (char lvl_name[5], Node **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "NODES", &raw_data, NODE_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (NODE_SIZE * i);

        (*data)[i].x = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].y = *(int16_t*)ptr;      ptr += 2;

        (*data)[i].dx = *(int16_t*)ptr;     ptr += 2;
        (*data)[i].dy = *(int16_t*)ptr;     ptr += 2;

        (*data)[i].right_bbox[0] = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].right_bbox[1] = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].right_bbox[2] = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].right_bbox[3] = *(int16_t*)ptr;      ptr += 2;

        (*data)[i].left_bbox[0]  = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].left_bbox[1]  = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].left_bbox[2]  = *(int16_t*)ptr;      ptr += 2;
        (*data)[i].left_bbox[3]  = *(int16_t*)ptr;      ptr += 2;

        (*data)[i].right_child = *(uint16_t*)ptr;       ptr += 2;
        (*data)[i].left_child  = *(uint16_t*)ptr;
    }
    return length;
}

/* read_sectors: read the SECTORS lump */
int read_sectors (char lvl_name[5], Sector **data)
{
    uint8_t *raw_data = NULL;

    int length = generic_read (lvl_name, "SECTORS", &raw_data, SECTOR_SIZE);

    (*data) = malloc (sizeof(**data) * length);
    for (int i = 0; i < length; ++i)
    {
        uint8_t *ptr = raw_data + (SECTOR_SIZE * i);

        (*data)[i].floor_h = *(int16_t*)ptr;    ptr += 2;
        (*data)[i].ceil_h  = *(int16_t*)ptr;    ptr += 2;

        memcpy ((*data)[i].floor_tex, (char*)ptr, 8);
        (*data)[i].floor_tex[8] = '\0';
        ptr += 8;

        memcpy ((*data)[i].ceil_tex, (char*)ptr, 8);
        (*data)[i].ceil_tex [8] = '\0';
        ptr += 8;

        (*data)[i].brightness = *(uint16_t*)ptr;    ptr += 2;
        (*data)[i].type       = *(uint16_t*)ptr;    ptr += 2;
        (*data)[i].tag        = *(uint16_t*)ptr;
    }
    return length;
}

