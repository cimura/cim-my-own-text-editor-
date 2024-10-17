#include "cim.h"

// this func doesn't have to worry about the details of modifying an t_row
void  editor_insert_char(int c)
{
  // 最後の行,空文字を入れることで改行しているようにしている
  if (g_E.cy == g_E.num_rows)
    insert_row(g_E.num_rows, "", 0);
  row_insert_char(&g_E.row[g_E.cy], g_E.cx, c);
  g_E.cx++;
}

// if->行頭での改行，else->改行して文を二つに分ける
void  editor_insert_newline()
{
  if (g_E.cx == 0)
    insert_row(g_E.cy, "", 0);
  else
  {
    t_row *row = &g_E.row[g_E.cy];
    insert_row(g_E.cy + 1, &row->chars[g_E.cx], row->size - g_E.cx);
    row = &g_E.row[g_E.cy];
    row->size = g_E.cx;
    row->chars[row->size] = '\0';
    update_row(row);
  }
  g_E.cy++;
  g_E.cx = 0;
}

void  editor_del_char()
{
  if (g_E.cy == g_E.num_rows
    || (g_E.cx == 0 && g_E.cy == 0))
    return;

  t_row* row = &g_E.row[g_E.cy];
  if (g_E.cx > 0)
  {
    row_del_char(row, g_E.cx - 1);
    g_E.cx--;
  }
  else
  {
    // 行頭でdelete-keyを押すとその行が消されるように
    g_E.cx = g_E.row[g_E.cy - 1].size;
    row_append_string(&g_E.row[g_E.cy - 1], row->chars, row->size);
    del_row(g_E.cy);
    g_E.cy--;
  }
}
