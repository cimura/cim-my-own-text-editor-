#include "kilo.h"

static void padding(t_buf *buf, int welcome_len)
{
  int padding = (g_E.screen_cols - welcome_len) / 2;
  if (padding)
  {
    buf_append(buf, "~", 1);
    padding--;
  }
  while (padding--)
    buf_append(buf, " ", 1);  
}

static void scroll()
{
  if (g_E.cy < g_E.row_off)
    g_E.row_off = g_E.cy;
  if (g_E.cy >= g_E.row_off + g_E.screen_rows)
    g_E.row_off = g_E.cy - g_E.screen_rows + 1;
  if (g_E.cx < g_E.col_off)
    g_E.col_off = g_E.cx;
  if (g_E.cx >= g_E.col_off + g_E.screen_cols)
    g_E.col_off = g_E.cx - g_E.screen_cols + 1;
}

static void  draw_rows(t_buf *buf)
{
  int y;
  for (y = 0; y < g_E.screen_rows; ++y)
  {
    int file_row = y + g_E.row_off;
    if (file_row >= g_E.num_rows)
    {
      if (g_E.num_rows == 0 && y == g_E.screen_rows / 3)
      {
        char welcome[80];
        int welcome_len = snprintf(welcome, sizeof(welcome), 
                                  "my_vim editor -- version %s", MY_VIM_VERSION);
        if (welcome_len > g_E.screen_cols)
          welcome_len = g_E.screen_cols;
        
        padding(buf, welcome_len);
        buf_append(buf, welcome, welcome_len);
      }
      else
        buf_append(buf, "~", 1);
    }
    else
    {
      int len = g_E.row[file_row].size - g_E.col_off;
      if (len < 0)
        len = 0;
      if (len > g_E.screen_cols)
        len = g_E.screen_cols;
      buf_append(buf, &g_E.row[file_row].chars[g_E.col_off], len);
    }
    
    buf_append(buf, "\x1b[K", 3);
  
    if (y < g_E.screen_rows - 1)
      buf_append(buf, "\r\n", 2);
  }
}

void  clear_screen(void)
{
  scroll();
  t_buf buf = ABUF_INIT;

  buf_append(&buf, "\x1b[?25l", 6);
  buf_append(&buf, "\x1b[H", 3);

  draw_rows(&buf);

  char b[32];
  snprintf(b, sizeof(b), "\x1b[%d;%dH",
          (g_E.cy - g_E.row_off) + 1, (g_E.cx - g_E.col_off) + 1);
  buf_append(&buf, b, strlen(b));

  // buf_append(&buf, "\x1b[H", 3);
  buf_append(&buf, "\x1b[?25h", 6);

  write(STDOUT_FILENO, buf.data, buf.len);
  buf_free(&buf);
}
