/*
 * Terminal raw mode (ie no newline needed for input)
 *
 */

#include "rawterm.h"

char term_israw = 0;



#ifndef __SDL
/* term_error: error logging
    severity:   0 = info
                1 = warning
                2 = error   */
int term_error (char *msg, int severity) {
    // severity: 0 = info, 1 = warning, 2 = error
    switch (severity) {
    case 0:     // info
        raw_writes (msg);
        break;
    case 1:     // warning
        raw_writes ("rawterm: Warning - ");
        raw_writes (msg);
        break;
    case 2:     // error
        raw_writes ("rawterm: Error - ");
        raw_writes (msg);
        term_noraw();
        break;
    }
    return -1;
}

/* term_raw: set the terminal to raw mode
    returns non-zero on failure, else 0*/
int term_raw() {

    extern struct termios def_term;
    struct termios raw_term;


    if (!isatty (STDIN_FILENO))
        return term_error ("Not in a tty!", 2);

    if (tcgetattr (STDIN_FILENO, &def_term) < 0)
        return term_error ("Couldn't get tty settings!", 2);

    if (atexit (term_noraw) != 0)
        return term_error ("Couldn't register term_noraw atexit!", 2);

    raw_term = def_term;
    cfmakeraw (&raw_term);
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &raw_term);

    term_israw = 1;

    return 0;
}

/* term_noraw: reset the terminal to buffered mode */
void term_noraw() {
    tcflush (STDIN_FILENO, TCIOFLUSH);
    tcsetattr (STDIN_FILENO, TCSANOW, &def_term);
    term_israw = 0;
}

/* raw_input: get input from the terminal */
int raw_input() {

    int bytes_read, c;
    c = 0;

    tcflush (STDIN_FILENO, TCIFLUSH);
    bytes_read = read (STDIN_FILENO, &c, 1);

    if (bytes_read <= 0)
        return bytes_read;  // input error
    else
        return c;           // read a character

}

/* raw_writec: write character to the terminal */
void raw_writec (char c) {
    write (STDOUT_FILENO, &c, 1);
}
#endif

/* raw_write: write character(s) to the terminal */
void raw_writes (char *format, ...) {

    va_list ap;
    va_start (ap, format);
#ifndef __SDL
    char str[1000];

    vsprintf (str, format, ap);

    for (int i = 0; str+i != NULL && *(str+i) != '\0'; ++i)
        raw_writec (*(str+i));
#else
    vprintf (format, ap);
    va_end (ap);
#endif
}

