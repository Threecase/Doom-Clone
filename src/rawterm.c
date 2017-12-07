/*
 * Terminal raw mode (ie no newline needed for input)
 *
 */

#include "rawterm.h"



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
        term_shutdown();
        break;
    }
    return -1;
}

/* term_raw: set the terminal to raw mode
    returns non-zero on failure, else 0*/
int term_raw() {
    
    extern struct termios def_term;
    struct termios raw_term;
    cfmakeraw (&raw_term);


    if (!isatty (STDIN_FILENO))
        return term_error ("Not in a tty!", 2);

    if (tcgetattr (STDIN_FILENO, &def_term) < 0)
        return term_error ("Couldn't get tty settings!", 2);

    if (atexit (term_shutdown) != 0)
        return term_error ("Couldn't register term_shutdown atexit!", 2);

    tcsetattr (STDIN_FILENO, TCSAFLUSH, &raw_term);

    return 0;
}

/* term_shutdown: reset the terminal to buffered mode */
void term_shutdown() {
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &def_term);
}

/* raw_input: get input from the terminal */
int raw_input() {

    int bytes_read, c;

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

/* raw_write: write character(s) to the terminal */
void raw_writes (char *str) {
    for (int i = 0; str+i != NULL && *(str+i) != '\0'; ++i)
        raw_writec (*(str+i));
}

