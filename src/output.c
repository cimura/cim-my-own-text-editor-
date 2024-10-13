#include "kilo.h"

static void  draw_rows()
{
  int y;
  for (y = 0; y < 24; ++y)
    write(STDOUT_FILENO, "~\r\n", 3);
}

void  clear_screen(void)
{
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  draw_rows();

  write(STDOUT_FILENO, "\x1b[H", 3);
}
