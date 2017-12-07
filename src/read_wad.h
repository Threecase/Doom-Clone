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
    uint32_t numlumps;
    uint32_t start_lumps;
} Wadinfo;

/* lump headers */
typedef struct {
    uint32_t lumpstart;
    uint32_t size;
    char name[8];
} Filelump;

/* info about a lump */
typedef struct {
    char name[8];
    int fd;
    uint32_t position;
    uint32_t size;
} Lumpinfo;


// VARS
extern int numlumps;
extern Lumpinfo *lumps;


// FUNCTIONS

void read_WAD (char *wadname);

int get_lump_index (char *lump_name);

int get_lump_size (int lump);

void read_lump (int lump_index, void *output);


#endif

