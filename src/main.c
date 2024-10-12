#include "kilo.h"

int	main(void) {
  enable_raw_mode();
  while (1)
  {
    char  c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1)
      error_exit("read");
    if (c == 'q')
      break ;
    if (iscntrl(c))
      printf("%d\r\n", c);
    else
      printf("%d ('%c')\r\n", c, c);
  }
  return (0);
}
