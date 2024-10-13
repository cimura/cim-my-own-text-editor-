#include "kilo.h"

int get_window_size(int *rows, int *cols)
{
  struct winsize ws;
  
  if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1
      || ws.ws_col == 0)
  {
      if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
        return (-1);
      editor_read_key();
      return (get_cursor_position());
  }
  else
  {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return (0);
  }
}

int get_cursor_position(void)
{
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    return (-1);
  printf("\r\n");
  char c;

  while (read(STDIN_FILENO, &c, 1) == 1)
  {
    if (iscntrl(c))
      printf("%d\r\n", c);
    else
      printf("%d ('%c')\r\n", c, c);
  }
  editor_read_key();
  return (-1);
}