/*
 * Temp file for the softare version of render_ssector
 *
 */



/* render_ssector: draw a subsector to the screen */
/* TODO : IMPROVE !!! */
/* TODO: how does this work? */
void render_ssector (Level *level, uint16_t ssector_number)
{
    SSector ssec = level->ssectors[ssector_number];

    Point pnt = { 0 };
    int outx   = 0,
        outy   = 0,
        seg_n  = 0,
        corner = 0;
    Colour col[ssec.num_segs];

    /* what is this? */
    long pixels_to_draw[ssec.num_segs][4][2];
    memset (pixels_to_draw, 0, ssec.num_segs * 4 * 2);

    /* loop through lines in N, translate to 3D, and
        attempt to draw the polygon to the screen */
    seg_n = 0;
    for (int i = ssec.start_seg; i < ssec.start_seg + ssec.num_segs; ++i, ++seg_n)
    {
        corner = 0;
        /* gets the Sector corresponding with the passed SubSector */
        Seg     seg = level->segs[ssec.start_seg];
        Linedef ldf = level->linedefs[seg.line];
        Sidedef sdf = level->sidedefs[ldf.right];
        Sector  sec = level->sectors[sdf.sector_num];

        int y = sec.floor_h;
        for (int n = 0; n < 2; ++n)
        {
            /* start vertex */
            pnt.x = level->vertices[level->segs[i].start].x;
            pnt.y = y;
            pnt.z = level->vertices[level->segs[i].start].y;
            outx = outy = -1;
            if (coords_3D_to_2D (pnt, player_pos, angle, &outx, &outy))
            {
                /* add to drawbuffer */
                pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
            }
            else
            {
                memset (pixels_to_draw[seg_n], 0, sizeof(long) * 8);
                break;
            }

            /* end vertex */
            pnt.x = level->vertices[level->segs[i].end].x;
            pnt.y = y;
            pnt.z = level->vertices[level->segs[i].end].y;
            outx = outy = -1;
            if (coords_3D_to_2D (pnt, player_pos, angle, &outx, &outy))
            {
                /* add to drawbuffer */
                pixels_to_draw[seg_n][corner][0] = (SCREEN_WIDTH/2) + outx;
                pixels_to_draw[seg_n][corner++][1] = (SCREEN_HEIGHT/2) + outy;
            }
            else
            {
                memset (pixels_to_draw[seg_n], 0, sizeof(long) * 8);
                break;
            }
            col[seg_n].r = pnt.x % 255;
            col[seg_n].g = 127;
            col[seg_n].b = pnt.z % 255;

            y = sec.ceil_h;
        }
    }
    for (int j = 0, draw = 0; j < ssec.num_segs; ++j, draw = 0)
    {
        for (int i = 0; !draw && i < 4; ++i)
        {
            if (pixels_to_draw[j][i][0] != 0 || pixels_to_draw[j][i][1] != 0)
                draw = 1;
        }
        if (draw)
        {   fill_poly (pixels_to_draw[j], col[j]);
        }
    }
}