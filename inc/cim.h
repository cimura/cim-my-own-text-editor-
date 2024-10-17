#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define CIM_VERSION "0.0.1"
#define CIM_TAB_STOP 8
#define CIM_QUIT_TIMES 1

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#define BLUE \033[34m
#define END   \033[39m

enum  editor_key {
  BACKSPACE = 127,
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

// tabにはアドホックに対応する
typedef struct  e_row
{
  int size;
  int rsize;
  char* chars;
  char* render;
} t_row;

typedef struct  s_config {
  int cx;
  int cy;
  int rx;
  int row_off;
  int col_off;
  int screen_rows;
  int screen_cols;
  int num_rows;
  int dirty;
  t_row* row;
  char* file_name;
  char  status_msg[80];
  time_t  status_msg_time;
  struct	termios orig_termios;
} t_config;

typedef struct s_buf {
  char* data;
  int len;
} t_buf;

t_config  g_E;

// *** setup.c ***
void	disable_raw_mode(void);
void	enable_raw_mode(void);
void  error_exit(const char* message);
void  init_editor(void);

// *** read_key.c ***
void  editor_process(void);
int  editor_read_key(void);
void  editor_move_cursor(int key);
char *editor_prompt(char *prompt, void (*callback)(char *, int));

// *** output.c ***
void  clear_screen(void);
void  set_status_message(const char* fmt, ...);

// *** utils.c ***
int get_window_size(int* rows, int* cols);
int get_cursor_position(int* rows, int* cols);
void  buf_append(t_buf* buf, const char* data, int len);
void  buf_free(t_buf* buf);

// *** file_io.c ***
void  editor_open(char* file_name);
void  editor_save();

// *** row_operations.c ***
void  insert_row(int at, char* s, size_t len);
int   row_cx_to_rx(t_row* row, int cx);
int   row_rx_to_cx(t_row *row, int rx);
void  row_insert_char(t_row* row, int at, int c);
void  row_del_char(t_row* row, int at);
void  row_append_string(t_row* row, char* s, size_t len);
void  del_row(int at);
void  update_row(t_row* row);

// *** editor_operations.c ***
void  editor_insert_char(int c);
void  editor_del_char();
void  editor_insert_newline();

// *** find.c ***
void  find();