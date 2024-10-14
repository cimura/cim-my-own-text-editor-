#include "kilo.h"

void  editor_open(char *file_name)
{
  FILE  *fp = fopen(file_name, "r");
  if (!fp)
    error_exit("fopen");
  char  *line = NULL;
  size_t  line_cap = 0;
  ssize_t line_len;
  
  while ((line_len = getline(&line, &line_cap, fp)) != -1)
  {
    while (line_len > 0 && (line[line_len - 1] == '\n' || line[line_len - 1] == '\r'))
      line_len--;
    append_row(line, line_len);
  }
  free(line);
  fclose(fp);
}