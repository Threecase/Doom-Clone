/*
 * Read a WAD file
 *
 */

#include "read_wad.h"


int NUM_LUMPS = 0;
Lumpinfo *LUMPS = NULL;


/* read_WAD: read a WAD file */
void read_WAD (char *wadname) {

    Wadinfo wadheader;
    Filelump *raw_lumps = NULL;
    int wad_fd;
    int startlumps, length;

    if ((wad_fd = open (wadname, O_RDONLY)) == -1)
        fatal_error ("Could not open file '%s'", wadname);


    startlumps = NUM_LUMPS;

    read (wad_fd, &wadheader, 12);

    if (strncasecmp (wadheader.id, "iwad", 4))
        if (strncasecmp (wadheader.id, "pwad", 4))
            fatal_error ("'%s' is neither an IWAD nor PWAD!", wadname);

    wadheader.numlumps = LONG (wadheader.numlumps);
    wadheader.start_lumps = LONG (wadheader.start_lumps);

    NUM_LUMPS = wadheader.numlumps;

    LUMPS = realloc (LUMPS, NUM_LUMPS * sizeof(Lumpinfo));
    if (LUMPS == NULL)
        fatal_error ("LUMPS realloc failed!");

    length = wadheader.numlumps * sizeof(Filelump);
    raw_lumps = alloca (length);
    if (raw_lumps == NULL)
        fatal_error ("raw_lumps alloc failed!");
    lseek (wad_fd, wadheader.start_lumps, SEEK_SET);
    read (wad_fd, raw_lumps, length);

    for (int i = startlumps; i < NUM_LUMPS; ++i, ++raw_lumps) {
        strncpy (LUMPS[i].name, raw_lumps->name, 8);
        LUMPS[i].position = LONG (raw_lumps->lumpstart);
        LUMPS[i].size = LONG (raw_lumps->size);
        LUMPS[i].fd = wad_fd;
    }
}

/* get_lump_index: return index of a lump */
int get_lump_index (char *lump_name) {

    for (int i = NUM_LUMPS-1; i > 0; --i) {
        if (!strncasecmp (LUMPS[i].name, lump_name, strnlen (lump_name, 8)))
            return i;
    }
    return -1;  // error: lump not found
}

/* get_lump_size: return the size
    (in bytes) of the lump*/
int get_lump_size (int lump) {

    if (lump > NUM_LUMPS || lump < 0)
        fatal_error ("Lump %i out of range!", lump);

    return LUMPS[lump].size;
}

/*read_lump: read the data from the lump
    into output */
void read_lump (int lump_index, void *output) {

    if (lump_index > NUM_LUMPS || lump_index < 0)
        fatal_error ("Lump %i is out of range!", lump_index);

    Lumpinfo lump;
    int bytes_read;

    lump = LUMPS[lump_index];
    lseek (lump.fd, lump.position, SEEK_SET);
    if ((bytes_read = read (lump.fd, output, lump.size) < lump.size))
        fatal_error ("Only read %i bytes of lump '%s' (size %i)!",
            bytes_read, lump.name, lump.size);
}

