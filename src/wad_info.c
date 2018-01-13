/*
 * Interpret the info from the WAD.
 * (ie generate map, etc.)
 *
 */

#include "wad_info.h"

#include "rawterm.h" // FIXME : TEMP



// lists for WAD stuff
extern int NUM_STUFF[10];
extern void **DATA;


/* read_map_data: read the map data from the WAD */
void read_map_data (int lvl_num) {

    DATA = malloc (sizeof(void *) * 10);

    char lump_name[10][8] = {
        "THINGS", "LINEDEFS", "SIDEDEFS", "VERTEXES", "SEGS",
        "SSECTORS", "NODES", "SECTORS", "REJECT", "BLOCKMAP" };

    int typesize[10] = {
        sizeof(Thing), sizeof(Linedef), sizeof(Sidedef), sizeof(Vertex),
        sizeof(Seg), sizeof(SSector), sizeof(Node), sizeof(Sector), 1, 1 };

    char lvl_name[5];
    sprintf (lvl_name, "E1M%i", lvl_num);

    for (int n = 0, i; n < 8; ++n) {   // FIXME : read the REJECT and BLOCKMAP
        i = get_lump_index (lvl_name) + 1;
        Lumpinfo lump = LUMPS[i];

        for (; strncasecmp (lump_name[n], lump.name, 8);)
            lump = LUMPS[++i];
        NUM_STUFF[n] = get_lump_size (i);

        if (NUM_STUFF[n] / (float)typesize[n] != NUM_STUFF[n] / typesize[n])
            fatal_error("%.8s divides incorrectly! (%i/%i->%f)", lump_name[n],
                NUM_STUFF[n], typesize[n], NUM_STUFF[n] / (float)typesize[n]);

        DATA[n] = malloc (NUM_STUFF[n]);
        NUM_STUFF[n] /= typesize[n];
        if (DATA[n] == NULL)
            fatal_error ("Allocation of DATA list %i failed", n);
        read_lump (i, DATA[n]);
//        printf ("%8.8s : %4i\n", lump_name[n], NUM_STUFF[n]);
    }
}

