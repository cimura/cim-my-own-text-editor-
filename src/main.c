#include "kilo.h"

int	main(int argc, char *argv[]) {
  enable_raw_mode();
  init_editor();
  if (argc >= 2)
    editor_open(argv[1]);

  while (1)
  {
    clear_screen();
    editor_process();
  }
  return (0);
}
