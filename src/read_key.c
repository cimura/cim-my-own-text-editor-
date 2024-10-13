#include "kilo.h"

static char  editor_read_key()
{
  ssize_t nread;
  char  c;

  while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
  {
    if (nread == -1 && errno != EAGAIN)
      error_exit("read");
  }
  return (c);
}

void  editor_process(void)
{
  char  c = editor_read_key();
  switch (c)
  {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(EXIT_SUCCESS);
      break;
  }
}