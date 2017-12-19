/*
 * The Binary Space Partition Tree data structure
 * and related functions.
 *
 */

// FIXME: TEMP INCLUDE
#include <stdio.h>

#include "BSP_tree.h"


// return vals for direction returns
#define IN_FRONT    0
#define BEHIND      1
#define SAME_PLANE  2
#define INTERSECT   3


char test_line (Node n, int x, int y);


/* add_to_list: Adds E to list L, resizing it if needed */
/*int add_to_list (Linedef **L, int *len, Linedef *E) {
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
*/

/* poly_test_loc: Tests whether the polygon
    is in front, on the same plane as, behind,
    or intersecting the other polygon */
/*char poly_test_loc (Linedef a, Linedef b) {

    char s1, s2;

    s1 = test_line (a, b.start->x, b.start->y);
    s2 = test_line (a, b.end->x, b.end->y);

    if (s1 != s2) 
        return INTERSECT;
    else
        return s1;

}
*/

/* split_poly_add: split the polygon,
    then add the pieces to the list */
/*void split_poly_add (Linedef *p, Linedef **list) {
    // TODO
}*/


/* create_BSP: Create a BSP tree */
/* NOT NEEDED
void create_BSP (Node *N, Linedef **polys, int num_polys) {

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
                N->pos = calloc (1, sizeof(Node));
            add_to_list (N->pos->lines, &N->pos->length, polys[i]);
            break;
        // poly is behind P
        case BEHIND:
            if (N->neg == NULL)
                N->neg = calloc (1, sizeof(Node));
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
*/


/* render_tree: Traverse the BSP tree */
void render_tree (uint16_t n_num, Point view) {

    // N is a subsectot
    if (n_num & IS_SSECTOR) {
        if (n_num == -1)
            n_num = 0;
//        printf ("render %hi\n", n_num & (~IS_SSECTOR));
        render_ssector (SSECTOR_LIST[n_num & (~IS_SSECTOR)], view);
        return;
    }

    Node bsp_node = (Node)NODE_LIST[n_num];
//    printf ("%hi : %hi | %hi\n", n_num, bsp_node.child[1], bsp_node.child[0]);

    char i = test_line (bsp_node, view.x, view.z);
    render_tree (bsp_node.child[i], view);
    render_tree (bsp_node.child[i ^ 1], view);
}

/* test_line: figure out where x/y are with respect to
    line L (eg in front, behind, etc.) */
char test_line (Node n, int x, int y) {

    if (n.dx == 0)          // node line is straight vertical
        return x > n.x;     // note: right side is front

    if (n.dz == 0)          // node line is straight horizontal
        return y <= n.z;

    // more advanced calculations
    if (x - n.x < 0) {      // x is to the left
        if (y - n.z < 0)    // but y is below --> front
            return 0;
        else
            return 1;
    }
    else {                  // x is to the right
        if (y - n.z <0)     // and y is below --> front
            return 0;
        else
            return 1;
    }
}

