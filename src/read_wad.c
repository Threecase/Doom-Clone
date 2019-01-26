/*
 * Read a WAD file
 *
 */

#include "read_wad.h"
#include "data_types.h"
#include "error.h"



int NUM_LUMPS = 0;
LumpInfo *LUMPS = NULL;


/* read_WAD: read a WAD file */
void read_WAD (char const *wadname)
{
    WadInfo wadheader = { 0 };
    int wad_fd = -1;
    int startlumps = 0;


    wad_fd = open (wadname, O_RDONLY);
    if (wad_fd == -1)
    {   fatal_error ("Could not open file '%s'", wadname);
    }


    startlumps = NUM_LUMPS;

    /* read the wad header */
    read (wad_fd, wadheader.id, 4);
    wadheader.id[4] = '\0';
    read (wad_fd, &wadheader.numlumps, 4);
    read (wad_fd, &wadheader.start_lumps, 4);

    /* make sure this is an IWAD or PWAD */
    if (strncmp (wadheader.id, "IWAD", 4) != 0)
    {   if (strncmp (wadheader.id, "PWAD", 4) != 0)
        {   fatal_error ("'%s' is neither an IWAD nor PWAD!",
                         wadname);
        }
    }

    /* fix endianness if needed */
    wadheader.numlumps    = LONG (wadheader.numlumps);
    wadheader.start_lumps = LONG (wadheader.start_lumps);


    NUM_LUMPS = wadheader.numlumps;

    LUMPS = realloc (LUMPS, NUM_LUMPS * sizeof(LumpInfo));
    if (LUMPS == NULL)
    {   fatal_error ("LUMPS realloc failed!");
    }

    lseek (wad_fd, wadheader.start_lumps, SEEK_SET);


    /* stuff the newly-read lump headers into the LUMPS array */
    for (int i = startlumps; i < NUM_LUMPS; ++i)
    {
        int32_t lumpstart;
        read (wad_fd, &lumpstart, 4);
        LUMPS[i].position = LONG(lumpstart);

        int32_t size;
        read (wad_fd, &size, 4);
        LUMPS[i].size = LONG (size);

        read (wad_fd, LUMPS[i].name, 8);
        LUMPS[i].name[8] = '\0';

        LUMPS[i].fd = wad_fd;
    }
}

/* get_lump_index_from: return index of a lump,
 *                      starting from i = start */
int get_lump_from_level (char const *level, char const *lump)
{
    int start = get_lump_index (level) + 1;

    for (int i = start; i < NUM_LUMPS; ++i)
    {
        if (strcmp (LUMPS[i].name, lump) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* get_lump_index: return index of a lump */
int get_lump_index (char const *lump_name)
{
    for (int i = NUM_LUMPS-1; i >= 0; --i)
    {
        if (strcasecmp (LUMPS[i].name, lump_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* get_lump_size: return the size (in bytes) of the lump */
int get_lump_size (int lump)
{
    if (lump > NUM_LUMPS || lump < 0)
    {   fatal_error ("Invalid lump index '%i'", lump);
        return -1;
    }
    else
    {   return LUMPS[lump].size;
    }
}

/* read_lump_index: read the data from the lump into output
 *                  (addressed by the lump's index) */
void read_lump_index (int index, void *output)
{
    if (index > NUM_LUMPS || index < 0)
    {   fatal_error ("Invalid lump index '%i'", index);
    }

    LumpInfo lump = LUMPS[index];
    int bytes_read = 0;

    lseek (lump.fd, lump.position, SEEK_SET);
    bytes_read = read (lump.fd, output, lump.size);

    if (bytes_read < lump.size)
    {   fatal_error ("Only read %i bytes of lump '%s' (size %i)!",
                     bytes_read, lump.name, lump.size);
    }
}

/* read_lump: read data from the lump into output
 *            (addressed by the lump's name) */
void read_lump (const char *name, void *output)
{
    read_lump_index (get_lump_index (name), output);
}

