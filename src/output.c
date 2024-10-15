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
  g_E.rx = 0;
  if (g_E.cy < g_E.num_rows)
    g_E.rx = row_cx_to_rx(&g_E.row[g_E.cy], g_E.cx);

  if (g_E.cy < g_E.row_off)
    g_E.row_off = g_E.cy;
  if (g_E.cy >= g_E.row_off + g_E.screen_rows)
    g_E.row_off = g_E.cy - g_E.screen_rows + 1;
  if (g_E.cx < g_E.col_off)
    g_E.col_off = g_E.rx;
  if (g_E.cx >= g_E.col_off + g_E.screen_cols)
    g_E.col_off = g_E.rx - g_E.screen_cols + 1;
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
                                  "my_vim editor -- version %s", CIM_VERSION);
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
      int len = g_E.row[file_row].rsize - g_E.col_off;
      if (len < 0)
        len = 0;
      if (len > g_E.screen_cols)
        len = g_E.screen_cols;
      buf_append(buf, &g_E.row[file_row].render[g_E.col_off], len);
    }
    
    buf_append(buf, "\x1b[K", 3);
    buf_append(buf, "\r\n", 2);
  }
}

static void  draw_status_bar(t_buf *buf)
{
  buf_append(buf, "\x1b[7m", 4);
  
  char  status[80], rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines",
                      g_E.file_name ? g_E.file_name : "[No Name]", g_E.num_rows);
  int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d",
                      g_E.cy + 1, g_E.num_rows);
  if (len > g_E.screen_cols)
    len = g_E.screen_cols;
  buf_append(buf, status, len);
  while (len < g_E.screen_cols)
  {
    if (g_E.screen_cols - len == rlen)
    {
      buf_append(buf, rstatus, rlen);
      break ;
    }
    else
      buf_append(buf, " ", 1);
    len++;
  }
  buf_append(buf, "\x1b[m", 3);
  buf_append(buf, "\r\n", 2);
}

static void  draw_message_bar(t_buf *buf)
{
  buf_append(buf, "\x1b[K", 3);
  int msg_len = strlen(g_E.status_msg);
  if (msg_len > g_E.screen_cols)
    msg_len = g_E.screen_cols;
  
  // keyを押した10秒後にメッセージが消える
  if (msg_len && time(NULL) - g_E.status_msg_time < 5)
    buf_append(buf, g_E.status_msg, msg_len);
}

void  clear_screen(void)
{
  scroll();
  t_buf buf = ABUF_INIT;

  buf_append(&buf, "\x1b[?25l", 6);
  buf_append(&buf, "\x1b[H", 3);

  draw_rows(&buf);
  draw_status_bar(&buf);
  draw_message_bar(&buf);

  char b[32];
  snprintf(b, sizeof(b), "\x1b[%d;%dH",
          (g_E.cy - g_E.row_off) + 1, (g_E.rx - g_E.col_off) + 1);
  buf_append(&buf, b, strlen(b));

  // buf_append(&buf, "\x1b[H", 3);
  buf_append(&buf, "\x1b[?25h", 6);

  write(STDOUT_FILENO, buf.data, buf.len);
  buf_free(&buf);
}

void  set_status_message(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(g_E.status_msg, sizeof(g_E.status_msg), fmt, ap);
  va_end(ap);
  g_E.status_msg_time = time(NULL);
}