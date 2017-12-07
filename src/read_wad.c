/*
 * Read a WAD file
 *
 */

#include "read_wad.h"


int numlumps = 0;
Lumpinfo *lumps;


/* read_WAD: read a WAD file */
void read_WAD (char *wadname) {

    Wadinfo wadheader;
    Filelump *raw_lumps;
    int wad_fd;
    int startlumps, length;

    if ((wad_fd = open (wadname, O_RDONLY)) == -1)
        fatal_error ("Could not open file '%s'", wadname);


    read (wad_fd, &wadheader, 12);

    if (strncasecmp (wadheader.id, "iwad", 4))
        if (strncasecmp (wadheader.id, "pwad", 4))
            fatal_error ("'%s' is neither an IWAD nor PWAD!", wadname);


    numlumps = wadheader.numlumps;

    lumps = realloc (lumps, numlumps * sizeof(Lumpinfo));

    length = wadheader.numlumps * sizeof(Filelump);
    raw_lumps = alloca (length);
    lseek (wad_fd, wadheader.start_lumps, SEEK_SET);
    read (wad_fd, raw_lumps, length);

    for (int i = startlumps; i < numlumps; ++i, ++raw_lumps) {
        strncpy (lumps[i].name, raw_lumps->name, 8);
        lumps[i].position = raw_lumps->lumpstart;
        lumps[i].size = raw_lumps->size;
        lumps[i].fd = wad_fd;
    }
}

/* get_lump_index: return index of a lump */
int get_lump_index (char *lump_name) {

    for (int i = numlumps; i > 0; --i)
        if (!strncasecmp (lumps[i].name, lump_name, 8))
            return i;
    return -1;  // error: lump not found
}

/* get_lump_size: return the size
    (in bytes) of the lump*/
int get_lump_size (int lump) {

    if (lump > numlumps || lump < 0)
        fatal_error ("Lump %i out of range!", lump);

    return lumps[lump].size;
}

/*read_lump: read the data from the lump
    into output */
void read_lump (int lump_index, void *output) {

    if (lump_index > numlumps || lump_index < 0)
        fatal_error ("Lump %i is out of range!", lump_index);

    Lumpinfo lump;
    int read_b;

    lump = lumps[lump_index];

    lseek (lump.fd, lump.position, SEEK_SET);
    if ((read_b = read (lump.fd, output, lump.size) < lump.size))
        fatal_error ("Only read %i bytes of lump '%s' (size %i)!",
            read_b, lump.name, lump.size);
}

