/*
 * Read a WAD file
 *
 */


#ifndef _READ_WAD_H
#define _READ_WAD_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

#include "error.h"



/* WadInfo:
 *  IWAD/PWAD header
 *
 *  Each WAD header is 12 bytes long and contains 3 fields:
 *   - the characters `IWAD' or `PWAD'
 *   - a the number of lumps in the WAD
 *   - a byte index into the WAD marking the
 *     beginning of the WAD data actual
 */
typedef struct WadInfo
{   char id[4 + 1];
    int32_t numlumps;
    int32_t start_lumps;
} WadInfo;

/* FileLump:
 *  Lump header
 *
 *  Each lump header is 16 bytes long and contains 3 fields:
 *   - an offset into the WAD pointing to the start of the lump
 *   - the size of the lump in bytes
 *   - an 8-byte ASCII string containing the name of the lump
 */
typedef struct FileLump
{   int32_t lumpstart;
    int32_t size;
    char name[8 + 1];
} FileLump;

/* LumpInfo:
 *  An internal data structure; contains metadata about a lump
 */
typedef struct LumpInfo
{   char name[8 + 1];
    int fd;
    int32_t position;
    int32_t size;
} LumpInfo;



extern int NUM_LUMPS;
extern LumpInfo *LUMPS;



void read_WAD (const char *wadname);

int get_lump_from_level (const char *level, const char *lump);
int get_lump_index (const char *lump_name);

int get_lump_size (int lump);

void read_lump_index (int lump_index, void *output);
void read_lump (const char *name, void *output);


#endif

