#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define MY_VIM_VERSION "0.0.1"

#define BLUE \033[34m
#define END   \033[39m

enum  editor_key {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

typedef struct  s_config {
  int cx;
  int cy;
  int screen_rows;
  int screen_cols;
  struct	termios orig_termios;
} t_config;

typedef struct s_buf {
  char *data;
  int len;
} t_buf;

t_config  g_E;

// *** setup.c ***
void	disable_raw_mode(void);
void	enable_raw_mode(void);
void  error_exit(const char *message);
void  init_editor(void);

// *** read_key.c ***
void  editor_process(void);
int  editor_read_key(void);
void  editor_move_cursor(int key);

// *** output.c ***
void  clear_screen(void);

// *** utils.c ***
int get_window_size(int *rows, int *cols);
int get_cursor_position(int *rows, int *cols);
void  buf_append(t_buf *buf, const char *data, int len);
void  buf_free(t_buf *buf);