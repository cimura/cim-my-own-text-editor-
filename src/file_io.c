#include "cim.h"

char* rows_to_string(int* buflen)
{
	int ful_len = 0;
	int j;
	for (j = 0; j < g_E.num_rows; ++j)
		ful_len += g_E.row[j].size + 1;
	*buflen = ful_len;

	char* buf = malloc(ful_len);
	char* p = buf;
	for (j = 0; j < g_E.num_rows; ++j)
	{
		memcpy(p, g_E.row[j].chars, g_E.row[j].size);
		p += g_E.row[j].size;
		*p = '\n';
		p++;
	}
	return (buf);
}

void  editor_save()
{
	if (!g_E.file_name)
	{
    g_E.file_name = editor_prompt("Save as: %s (ESC to cancel)", NULL);
    if (!g_E.file_name)
    {
      set_status_message("Save aborted");
      return;
    }
    select_syntax_highlight();
  }

	int len;
	char *buf = rows_to_string(&len);

	// ftuncateではなくopenのオプションでもできるが，
	// writeが失敗するとデータが消滅してしまう
	int fd = open(g_E.file_name, O_RDWR | O_CREAT, 0644);
	if (fd != -1 && ftruncate(fd, len) != -1)
	{
		if (write(fd, buf, len) == len)
		{
			close(fd);
			free(buf);
			g_E.dirty = 0;
			set_status_message("%d bytes written to disk", len);
			return;
		}
		close(fd);
	}
	free(buf);
	set_status_message("Can't save! I/O error: %s", strerror(errno));
}
void  editor_open(char* file_name)
{
	free(g_E.file_name);
	g_E.file_name = strdup(file_name);

  select_syntax_highlight();
	FILE* fp = fopen(file_name, "r");
	if (!fp)
		error_exit("fopen");
	char* line = NULL;
	size_t  line_cap = 0;
	ssize_t line_len;

	while ((line_len = getline(&line, &line_cap, fp)) != -1)
	{
		while (line_len > 0 && (line[line_len - 1] == '\n' || line[line_len - 1] == '\r'))
			line_len--;
		insert_row(g_E.num_rows, line, line_len);
	}
	free(line);
	fclose(fp);
	g_E.dirty = 0;
}
