#include "cim.h"

int get_window_size(int* rows, int* cols)
{
	struct winsize ws;

	// ioctlはターミナルに関する情報を取得するシステムコントロール
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1
		|| ws.ws_col == 0)
	{
		// 999C->far right, 999B->far down
		// 右下のカーソルポジションをゲットすることでwindowの大きさを得る
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
			return (-1);
		// editor_read_key();
		return (get_cursor_position(rows, cols));
	}
	else
	{
		*rows = ws.ws_row;
		*cols = ws.ws_col;
		return (0);
	}
}

int get_cursor_position(int* rows, int* cols)
{
	char  buf[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return (-1);
	while (i < sizeof(buf) - 1)
	{
		if (read(STDIN_FILENO, &buf[i], 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';
	// &buf[1]: '[41;111'  col, rowこれを解釈してゲット

	if (buf[0] != '\x1b' || buf[1] != '[')
		return (-1);
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
		return (-1);
	return (0);
}

void  buf_append(t_buf* buf, const char* data, int len)
{
	if (!buf || !data || len <= 0)
		return;

	char* new = realloc(buf->data, buf->len + len);
	if (!new)
		return;
	memcpy(&new[buf->len], data, len);
	buf->data = new;
	buf->len += len;
}

void  buf_free(t_buf* buf)
{
	free(buf->data);
}
