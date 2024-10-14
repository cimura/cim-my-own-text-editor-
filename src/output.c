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

static void  draw_rows(t_buf *buf)
{
  int y;
  for (y = 0; y < g_E.screen_rows; ++y)
  {
    if (y == g_E.screen_rows / 3)
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
    
    buf_append(buf, "\x1b[K", 3);
  
    if (y < g_E.screen_rows - 1)
      buf_append(buf, "\r\n", 2);
  }
}

void  clear_screen(void)
{
  t_buf buf = ABUF_INIT;

  buf_append(&buf, "\x1b[?25l", 6);
  buf_append(&buf, "\x1b[H", 3);

  draw_rows(&buf);

  char b[32];
  snprintf(b, sizeof(b), "\x1b[%d;%dH", g_E.cy + 1, g_E.cx + 1);
  buf_append(&buf, b, strlen(b));

  // buf_append(&buf, "\x1b[H", 3);
  buf_append(&buf, "\x1b[?25h", 6);

  write(STDOUT_FILENO, buf.data, buf.len);
  buf_free(&buf);
}
