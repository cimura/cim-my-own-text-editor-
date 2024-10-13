#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define BLUE \033[34m
#define END   \033[39m


typedef struct  s_config {
  int screen_rows;
  int screen_cols;
  struct	termios orig_termios;
} t_config;

t_config  g_E;

// *** setup.c ***
void	disable_raw_mode(void);
void	enable_raw_mode(void);
void  error_exit(const char *message);
void  init_editor(void);

// *** read_key.c ***
void  editor_process(void);
char  editor_read_key();

// *** output.c ***
void  clear_screen(void);

// *** utils.c ***
int get_window_size(int *rows, int *cols);
int get_cursor_position(void);