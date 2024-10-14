#include "kilo.h"

void  append_row(char *s, size_t len)
{
  g_E.row = realloc(g_E.row, sizeof(t_row) * (g_E.num_rows + 1));

  int at = g_E.num_rows;
  g_E.row[at].size = len;
  g_E.row[at].chars = malloc(len + 1);
  memcpy(g_E.row[at].chars, s, len);
  g_E.row[at].chars[len] = '\0';
  g_E.num_rows++;
}