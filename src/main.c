#include "kilo.h"

int	main(void) {
  enable_raw_mode();
  init_editor();

  while (1)
  {
    clear_screen();
    editor_process();
  }
  return (0);
}
