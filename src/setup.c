#include "kilo.h"

void  init_editor(void)
{
  g_E.cx = 0;
  g_E.cy = 0;
  
  if (get_window_size(&g_E.screen_rows, &g_E.screen_cols) == -1)
    error_exit("get_window_size");
}
void	disable_raw_mode(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_E.orig_termios) == -1)
    error_exit("tcsetattr");
}

void	enable_raw_mode(void)
{
  if (tcgetattr(STDIN_FILENO, &g_E.orig_termios) == -1)
    error_exit("tcgetattr");
  atexit(disable_raw_mode);

	struct	termios raw = g_E.orig_termios;
	// とりあえずフラグつけて <Ctrl> + <?> の設定をしている
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    error_exit("tcsetattr");
}

void  error_exit(const char *message)
{
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(message);
  exit(EXIT_FAILURE);
}