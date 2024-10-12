#include "kilo.h"

void	disable_raw_mode(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == 1)
    error_exit("tcsetattr");
}

void	enable_raw_mode(void)
{
  if (tcgetattr(STDIN_FILENO, &orig_termios) == 1)
    error_exit("tcgetattr");
  atexit(disable_raw_mode);

	struct	termios raw = orig_termios;

	// とりあえずフラグつけて <Ctrl> + <?> の設定をしている
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 1)
    error_exit("tcsetattr");
}

void  error_exit(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}