#include "kilo.h"

int row_cx_to_rx(t_row *row, int cx)
{
  int rx = 0;
  
  for (int j = 0; j < cx; ++j)
  {
    if (row->chars[j] == '\t')
      rx += (CIM_TAB_STOP - 1) - (rx % CIM_TAB_STOP);
    rx++;
  }
  return (rx);
}

static void  update_row(t_row *row)
{
  int tabs = 0;
  int j;

  for (j = 0; j < row->size; ++j)
    if (row->chars[j] == '\t')
      tabs++;
  free(row->render);
  row->render = malloc(row->size + tabs*(CIM_TAB_STOP - 1) + 1);
  
  int idx = 0;
  for (j = 0; j < row->size; ++j)
  {
    if (row->chars[j] == '\t')
    {
      row->render[idx++] = ' ';
      while (idx % CIM_TAB_STOP != 0)
        row->render[idx++] = ' ';
    }
    else
      row->render[idx++] = row->chars[j];
  }
  row->render[idx] = '\0';
  row->rsize = idx;
}

void  append_row(char *s, size_t len)
{
  g_E.row = realloc(g_E.row, sizeof(t_row) * (g_E.num_rows + 1));

  int at = g_E.num_rows;
  g_E.row[at].size = len;
  g_E.row[at].chars = malloc(len + 1);
  memcpy(g_E.row[at].chars, s, len);
  g_E.row[at].chars[len] = '\0';

  g_E.row[at].rsize = 0;
  g_E.row[at].render = NULL;
  update_row(&g_E.row[at]);

  g_E.num_rows++;
}