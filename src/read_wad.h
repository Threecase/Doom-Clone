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


// DATATYPES
/* IWAD/PWAD header */
typedef struct  {
    char id[4];
    int32_t numlumps;
    int32_t start_lumps;
} Wadinfo;

/* lump headers */
typedef struct {
    int32_t lumpstart;
    int32_t size;
    char name[8];
} Filelump;

/* info about a lump */
typedef struct {
    char name[8];
    int fd;
    int32_t position;
    int32_t size;
} Lumpinfo;


// VARS
extern int NUM_LUMPS;
extern Lumpinfo *LUMPS;


// FUNCTIONS

void read_WAD (const char *wadname);

int get_lump_index (const char *lump_name);

int get_lump_size (int lump);

void read_lump (int lump_index, void *output);


#endif

