/*
 * OpenGL Renderer
 *
 */

#include "../drawing.h"
#include "../error.h"
#include "../input.h"
#include "../DOOM.h"

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>


void handle_keypress    (unsigned char key, int x, int y);
void handle_keyrelease  (unsigned char key, int x, int y);
void handle_mousemotion (int x, int y);

void push_event (Event e);
void error_callback (GLenum source, GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length, GLchar *msg,
                     GLvoid *userParam);


Event *input_buffer     = NULL;
int input_buffer_length = 0,
    input_buffer_size   = 0,
    input_buffer_index  = 0;



/* render_ssector: draw an ssector */
void render_ssector (Level *lvl, uint16_t ssector_number)
{
    SSector s = lvl->ssectors[ssector_number];

    /* ceiling */
    glBegin (GL_POLYGON);
    for (uint16_t i = 0; i < s.num_segs; ++i)
    {
        Seg     seg = lvl->segs[s.start_seg + i];
        Linedef ld  = lvl->linedefs[seg.linedef];
        Sidedef r   = lvl->sidedefs[ld.right];
        Sector  sec = lvl->sectors[r.sector];

        Vertex start = lvl->vertices[seg.start];
        Vertex end   = lvl->vertices[seg.end];

        glColor3f (1,0,1);

        glVertex3f (  end.x,   end.y, sec.ceil_h );
        glVertex3f (start.x, start.y, sec.ceil_h );
    }
    glEnd();

    /* floor */
    glBegin (GL_POLYGON);
    for (uint16_t i = 0; i < s.num_segs; ++i)
    {
        Seg     seg = lvl->segs[s.start_seg + i];
        Linedef ld  = lvl->linedefs[seg.linedef];
        Sidedef r   = lvl->sidedefs[ld.right];
        Sector  sec = lvl->sectors[r.sector];

        Vertex start = lvl->vertices[seg.start];
        Vertex end   = lvl->vertices[seg.end];

        glColor3f (0,1,0);

        glVertex3f (  end.x,   end.y, sec.ceil_h );
        glVertex3f (start.x, start.y, sec.ceil_h );
    }
    glEnd();

    /* walls */
    glBegin (GL_QUADS);
    for (uint16_t i = 0; i < s.num_segs; ++i)
    {
        Seg     seg = lvl->segs[s.start_seg + i];
        Linedef ld  = lvl->linedefs[seg.linedef];
        Sidedef r   = lvl->sidedefs[ld.right];
        Sector  sec = lvl->sectors[r.sector];

        Vertex start = lvl->vertices[seg.start];
        Vertex end   = lvl->vertices[seg.end];

        if (strcmp ("-", r.mid_tex) != 0)
        {
            float c = 1 / (sec.brightness / 8.0);
            glColor3f (c, c, c);

            glVertex3f (start.x, start.y, sec.floor_h);
            glVertex3f (  end.x,   end.y, sec.floor_h);
            glVertex3f (  end.x,   end.y, sec.ceil_h );
            glVertex3f (start.x, start.y, sec.ceil_h );
        }
    }
    glEnd();
}

/* render_ssector_2D: draw a 2D ssector */
void render_ssector_2D (Level *lvl, uint16_t ssector_number)
{
    SSector s = lvl->ssectors[ssector_number];

    glBegin (GL_LINES);
    glColor3f (1,1,1);
    for (uint16_t i = 0; i < s.num_segs; ++i)
    {
        Seg seg = lvl->segs[s.start_seg + i];

        Vertex start = lvl->vertices[seg.start];
        Vertex end   = lvl->vertices[seg.end];

        glVertex2f (start.x, start.y);
        glVertex2f (  end.x,   end.y);
    }
    glEnd();
}

/* init_video: intialize OpenGL, etc. */
void init_video(void)
{
    debug ("creating window, %dx%d", game.window.width, game.window.height);

    /* initialize GLUT */
    int argc    = 0;
    char **argv = NULL;
    glutInit (&argc, argv);

    glutInitContextVersion (2, 1);

    glutInitDisplayMode (GLUT_DOUBLE);

    glutInitWindowSize (game.window.width, game.window.height);
    glutCreateWindow ("DOOM");

    /* glut config */
    glutSetCursor (GLUT_CURSOR_NONE);

    /* input callbacks */
    glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
    glutKeyboardFunc (handle_keypress);
    glutKeyboardUpFunc (handle_keyrelease);
    glutPassiveMotionFunc (handle_mousemotion);


    /* initialize GLEW */
    GLenum e = glewInit();
    if (e != GLEW_OK)
    {   fatal_error ("failed to init GLEW: %s", glewGetErrorString (e));
    }
    if (!GLEW_VERSION_2_1)
    {   fatal_error ("OpenGL 2.1 not supported!");
    }

    /* setup the error logging callback */
    glEnable (GL_DEBUG_OUTPUT);
    glEnable (GL_COLOR_TABLE);
    glEnable (GL_TEXTURE_2D);
    glDebugMessageCallback ((GLDEBUGPROC)error_callback, 0);


    /* OpenGL setup */
    glViewport (0,0, game.window.width, game.window.height);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

#if 0
    glOrtho (0, game.window.width, game.window.height, 0, 1000, -100);

#else
    gluPerspective (game.FOV, game.window.width / game.window.height, 0.1f, 4000);
#endif


    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();


    /* enable + configure stuff */
    glEnable (GL_DEPTH_TEST);
    glClearColor (0,0,0, 1);


    /* set up input buffer */
    input_buffer_size   = 32;
    input_buffer_length =  0;
    input_buffer_index  =  0;
    input_buffer        = calloc (sizeof(*input_buffer), input_buffer_size);
}

/* shutdown_video:  */
void shutdown_video(void)
{ }

/* draw_frame: output the frame to the screen */
void draw_frame(void)
{
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    /* draw the cursor */
    if (game.drawmode == DRAWMODE_2D)
    {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef (game.window.width/2, game.window.height/2, 0);
        glScalef (game.map_scale, game.map_scale, 1);

        glBegin (GL_LINES);
          glColor3f (1,0,0);
          glVertex2f (  25 ,  25 );
          glVertex2f (  0  , -25 );

          glVertex2f ( -25 ,  25 );
          glVertex2f (  0  , -25 );
        glEnd();

        glPopMatrix();
    }


    /* draw the frame */
    glutSwapBuffers();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* transform everything */
    glMatrixMode (GL_MODELVIEW);

    if (game.drawmode == DRAWMODE_2D)
    {
        glTranslatef (game.window.width/2, game.window.height/2, 0);
        glScalef (game.map_scale, game.map_scale, 1);
        glRotatef (270 - player.angle, 0,0,1);

        GLfloat z_translation = 0;
        if (game.drawmode == DRAWMODE_3D)
        {
            z_translation = player.pos.z;
        }
        glTranslatef (-player.pos.x,
                      -player.pos.y,
                      -z_translation);
    }
    else
    {
        glScalef (game.map_scale, game.map_scale, 1);

        float x = player.pos.x,
              y = player.pos.y,
              z = player.pos.z;

        float a = 270 - player.angle;

        gluLookAt (x, y, z,
                    x - sin (RADS(a)),
                    y - cos (RADS(a)),
                    z,
                   0,0,-1);
    }

    input_buffer_length = 0;
    input_buffer_index  = 0;
}

/* init_input:  */
void init_input(void)
{ }

/* shutdown_input:  */
void shutdown_input(void)
{ }

/* print: output stuff to console */
void print (char *format, ...)
{
    va_list ap;
    va_start (ap, format);

    vprintf (format, ap);

    va_end (ap);
}

/* input: get user input */
Event input(void)
{
    if (input_buffer_index < input_buffer_length)
    {
        return input_buffer[input_buffer_index++];
    }
    else
    {   return NULL_EVENT;
    }
}


/* call_60_times_per_second:  */
void call_in_1_60th_of_a_second (void (*fn)(int))
{
    glutTimerFunc (1000.0 / 60.0, fn, 0);
}

/* start_mainloop:  */
void start_mainloop(void)
{
    glutMainLoop();
}



/* handle_keypress:  */
void handle_keypress (unsigned char key, int x, int y)
{
    Event e;
    e.type = EVENT_KEYPRESS;
    e.key  = key;
    push_event (e);

    /* NOTE: to shut GCC up (see error_callback) */
    x = x;
    y = y;
}

/* handle_keyrelease:  */
void handle_keyrelease (unsigned char key, int x, int y)
{
    Event e;
    e.type = EVENT_KEYRELEASE;
    e.key  = key;
    push_event (e);

    /* NOTE: to shut GCC up (see error_callback) */
    x = x;
    y = y;
}

/* handle_mousemotion:  */
void handle_mousemotion (int x, int y)
{
    static int previous_x = 400,
               previous_y = 300;

    int const mouse_limit = game.window.width/3;

    Event e;
    e.type = EVENT_MOUSEMOTION;
    e.motion.dx = x - previous_x;
    e.motion.dy = y - previous_y;

    previous_x = x;
    previous_y = y;

    if (x >= game.window.width - mouse_limit || x <= mouse_limit)
    {
        previous_x = game.window.width/2;
        previous_y = game.window.height/2;
        glutWarpPointer (game.window.width/2, game.window.height/2);
    }

    push_event (e);
}

/* push_event: push an event onto the event queue */
void push_event (Event e)
{
    if (input_buffer_length > input_buffer_size)
    {
        input_buffer_size *= 2;
        input_buffer = realloc (input_buffer, input_buffer_size);
    }
    input_buffer[input_buffer_length++] = e;
}

/* error_callback: OpenGL error/logging callback */
void error_callback (GLenum source, GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length, GLchar *msg,
                     GLvoid *userParam)
{
    fprintf (stderr, "%s: %s type = 0x%x, severity = 0x%x, "
                     "message = %s\n",
        __func__,
        (type == GL_DEBUG_TYPE_ERROR)? "** GL_ERROR **" : "",
        type, severity, msg);

    if (severity == 0x9146)
    {
        exit (1);
    }

    /* NOTE: this is so GCC will shut up about unused variables.
     *       (can't use '#pragma GCC diagnostic ignored' because
     *        of a bug in GCC, apparently.) */
    source = source;
    type = type;
    id = id;
    severity = severity;
    length = length;
    msg = msg;
    userParam = userParam;
}







/* set_palette: set the colour palette */
void set_palette (Colour *palette)
{
#if 0
    glColorTable (GL_COLOR_TABLE,
                  GL_RGBA8,
                  256,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  palette);
#endif
}


#include "../texture.h"
/* FIXME: temp */
void draw_sidedef_texture_as_quad (Sidedef sd)
{
    /* FIXME: note that in OpenGL, the 1st byte will
     *        be the bottom left texel */
    debug ("get_middle_texture");
    Colour *texture_data = get_middle_texture (sd);
    debug ("get_middle_texture post");


    GLvoid *dat = malloc (64 * 72 * 4);
    memset (dat, 0, 64 * 72 * 4);

    glDrawPixels (64, 72,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  dat/*texture_data*/);

    free (texture_data);


#if 0
    debug ("glGenTextures");
    GLuint texname = 0;
    glGenTextures (1, &texname);
    glBindTexture (GL_TEXTURE_2D, texname);
    debug ("glGenTextures post");


    uint8_t *tmp = calloc (4, 64 * 72);
    debug ("tmp = %p", tmp);

    debug ("glTexImage2D");
    glTexImage2D (GL_TEXTURE_2D,
                  0,
                  GL_RGBA8,
                  64, 72,    /* TODO: get width/height */
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  tmp/*texture_data*/);
    debug ("glTexImage2D post");


    debug ("rendering quad");
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    glScalef (5, 5, 1);

    glBegin (GL_QUADS);

      glTexCoord2f (-1, -1);
      glVertex2f   (-1, -1);

      glTexCoord2f (-1,  1);
      glVertex2f   (-1,  1);

      glTexCoord2f ( 1,  1);
      glVertex2f   ( 1,  1);

      glTexCoord2f ( 1, -1);
      glVertex2f   ( 1, -1);

    glEnd();
#endif

}

