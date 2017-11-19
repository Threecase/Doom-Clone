/*
 * The Binary Space Partition Tree data structure
 * and related functions.
 *
 */

#include <stdio.h>

#include "BSP_Tree.h"



/* add_to_list: Adds E to list L, resizing it if needed */
int add_to_list (Polygon **L, int *len, Polygon *E) {
    int i;
    for (i = 0; i < *len; ++i)
        if (&L[i] == NULL) {
            L[i] = E;
            break;
        }
    if (i == *len) {
        printf ("resizing L to %i from %i\n", *(len)*2, *len);
        *len = *(len) * 2;
        L = realloc (L, *len);
        add_to_list (L, len, E);
    }
    return i;
}

/* split_poly_add: split the polygon,
    then add the pieces to the list */
void split_poly_add (Polygon *p, Polygon **list) {

    
}


/* create_BSP: Create a BSP tree */
void create_BSP (BSP_Tree *N, Polygon **polys, int num_polys) {

    N->pos = N->neg = NULL;

    // select random subsector as starting point
    Polygon *P = polys[rand() % num_polys];
    add_to_list (N->polygons, &N->num_polys, P);

    // apply the algorithm
    for (int i = 0; i < num_polys; ++i) {
        if (polys[i] == P)
            continue;

        // poly is in front of P
        else if (in_front (P, polys[i])) {
            if (N->pos == NULL)
                N->pos = calloc (1, sizeof(BSP_Tree));
            add_to_list (N->pos->polygons, &N->pos->num_polys, polys[i]);
        }
        // poly is behind P
        else if (behind (P, polys[i])) {
            if (N->neg == NULL)
                N->neg = calloc (1, sizeof(BSP_Tree));
            add_to_list (N->neg->polygons, &N->pos->num_polys, polys[i]);
        }
        // poly intersects P
        else if (intersect (P, polys[i])) {
            split_poly_add (polys[i], N->polygons);
            num_polys += 2;
        }
        // poly is in same plane as P
        else
            add_to_list (N->polygons, &N->num_polys, polys[i]);
    }
    create_BSP (N->pos, polys, num_polys);
    create_BSP (N->neg, polys, num_polys);
}


/* traverse: Traverse the BSP tree N */
void traverse (BSP_Tree N, Viewpoint V) {

    // node is a leaf, render it
    if (N.neg == NULL && N.pos == NULL)
        render (N);

    // V is in front of N
    else if (in_front (N, V)) {
        traverse (*N.neg, V);
        render (N);
        traverse (*N.pos, V);
    }
    // V is behind N
    else if (behind (N, V)) {
        traverse (*N.pos, V);
        render (N);
        traverse (*N.neg, V);
    }
    // V is on N
    else {
        traverse (*N.pos, V);
        traverse (*N.neg, V);
    }
}
