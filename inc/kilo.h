#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct	termios orig_termios;

// *** setup.c ***
void	disable_raw_mode(void);
void	enable_raw_mode(void);
void  error_exit(const char *message);