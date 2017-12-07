/*
 * The Binary Space Partition Tree data structure
 * and related functions.
 *
 */

// FIXME: TEMP
#include <stdio.h>

#include "BSP_tree.h"
#include "drawing.h"


// return vals for direction returns
#define IN_FRONT    0
#define BEHIND      1
#define SAME_PLANE  2
#define INTERSECT   3


char test_line (Linedef L, int x, int y);


/* add_to_list: Adds E to list L, resizing it if needed */
int add_to_list (Linedef **L, int *len, Linedef *E) {
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

/* poly_test_loc: Tests whether the polygon
    is in front, on the same plane as, behind,
    or intersecting the other polygon */
char poly_test_loc (Linedef a, Linedef b) {

    char s1, s2;

    s1 = test_line (a, b.start->x, b.start->y);
    s2 = test_line (a, b.end->x, b.end->y);

    if (s1 != s2) 
        return INTERSECT;
    else
        return s1;

}

/* split_poly_add: split the polygon,
    then add the pieces to the list */
void split_poly_add (Linedef *p, Linedef **list) {
    // TODO
}


/* create_BSP: Create a BSP tree */
void create_BSP (BSP_Node *N, Linedef **polys, int num_polys) {

    N->pos = N->neg = NULL;

    // select random subsector as starting point
    Linedef *P = polys[rand() % num_polys];
    add_to_list (N->lines, &N->length, P);

    // apply the algorithm
    for (int i = 0; i < num_polys; ++i) {
        if (polys[i] == P)
            continue;

        switch (poly_test_loc (*P, *polys[i])) {
        // poly is in front of P
        case IN_FRONT:
            if (N->pos == NULL)
                N->pos = calloc (1, sizeof(BSP_Node));
            add_to_list (N->pos->lines, &N->pos->length, polys[i]);
            break;
        // poly is behind P
        case BEHIND:
            if (N->neg == NULL)
                N->neg = calloc (1, sizeof(BSP_Node));
            add_to_list (N->neg->lines, &N->pos->length, polys[i]);
        // poly intersects P
        case INTERSECT:
            split_poly_add (polys[i], N->lines);
            num_polys += 2;
        // poly is in same plane as P
        case SAME_PLANE:
            add_to_list (N->lines, &N->length, polys[i]);
        }
    }
    create_BSP (N->pos, polys, num_polys);
    create_BSP (N->neg, polys, num_polys);
}


/* traverse: Traverse the BSP tree N */
void traverse (BSP_Node N, Thing V) {

    // node is a leaf, render it
    if (N.neg == NULL && N.pos == NULL)
        render_node (N);

    for (int i = 0; N.lines[i] != NULL; ++i)
        switch (test_line (*N.lines[i], V.x, V.y)) {
        // V is in front of N
        case IN_FRONT:
            traverse (*N.neg, V);
            render_node (N);
            traverse (*N.pos, V);
            break;

        // V is behind N
        case BEHIND:
            traverse (*N.pos, V);
            render_node (N);
            traverse (*N.neg, V);
            break;

        // V is on N
        case INTERSECT:
            traverse (*N.pos, V);
            traverse (*N.neg, V);
            break;
        }
}

/* test_line: figure out where x/y are with respect to
    line L (eg in front, behind, etc.) */
char test_line (Linedef L, int x, int y) {

    int y0, m, b;
    char state = -1;

    m = (L.end->y - L.start->y) / (L.end->x - L.start->x);
    b = L.start->y / (m * L.start->x);

    for (int x0 = L.start->x; x0 < L.end->x; ++x0) {
        y0 = m * x0 + b;
        if (x > x0 || x > y0) {
            if (state == BEHIND || state == SAME_PLANE)
                return INTERSECT;
            state = IN_FRONT;
        }
        if (x < x0 || y < y0) {
            if (state == IN_FRONT || state == SAME_PLANE)
                return INTERSECT;
            state = BEHIND;
        }
        else
            state = SAME_PLANE;
    }
}

