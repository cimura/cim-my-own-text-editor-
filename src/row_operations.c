#include "cim.h"

int row_cx_to_rx(t_row* row, int cx)
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

void  update_row(t_row* row)
{
  int tabs = 0;
  int j;

  for (j = 0; j < row->size; ++j)
    if (row->chars[j] == '\t')
      tabs++;
  free(row->render);
  row->render = malloc(row->size + tabs * (CIM_TAB_STOP - 1) + 1);

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

void  insert_row(int at, char *s, size_t len)
{
  if (at < 0 || at > g_E.num_rows)
    return;

  g_E.row = realloc(g_E.row, sizeof(t_row) * (g_E.num_rows + 1));
  memmove(&g_E.row[at + 1], &g_E.row[at], sizeof(t_row) * (g_E.num_rows - at));

  g_E.row[at].size = len;
  g_E.row[at].chars = malloc(len + 1);
  memcpy(g_E.row[at].chars, s, len);
  g_E.row[at].chars[len] = '\0';

  g_E.row[at].rsize = 0;
  g_E.row[at].render = NULL;
  update_row(&g_E.row[at]);

  g_E.num_rows++;
  g_E.dirty++;
}

void  free_row(t_row* row)
{
  free(row->render);
  free(row->chars);
}

void  del_row(int at)
{
  if (at < 0 || at >= g_E.num_rows)
    return;
  free_row(&g_E.row[at]);
  memmove(&g_E.row[at], &g_E.row[at + 1], sizeof(t_row) * (g_E.num_rows - at - 1));
  g_E.num_rows--;
  g_E.dirty++;
}

// ユーザーのinputに対応
void  row_insert_char(t_row* row, int at, int c)
{
  if (at < 0 || at > row->size)
    at = row->size;

  // c(input), `\0' => +2
  row->chars = realloc(row->chars, row->size + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
  row->size++;
  row->chars[at] = c;
  update_row(row);

  // どれだけソースに変更を加えたかを表示できる（＋α）
  g_E.dirty++;
}

void  row_append_string(t_row* row, char* s, size_t len)
{
  row->chars = realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  update_row(row);
  g_E.dirty++;
}

void  row_del_char(t_row* row, int at)
{
  if (at < 0 || at >= row->size)
    return;
  memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
  row->size--;
  update_row(row);
  g_E.dirty++;
}
