#include "cim.h"

// this func doesn't have to worry about the details of modifying an t_row
void  editor_insert_char(int c)
{
  // 最後の行?
  if (g_E.cy == g_E.num_rows)
    append_row("", 0);
  row_insert_char(&g_E.row[g_E.cy], g_E.cx, c);
  g_E.cx++;
}

void  editor_del_char()
{
  if (g_E.cy == g_E.num_rows
    || (g_E.cx == 0 && g_E.cy == 0))
    return;
  
  t_row * row = &g_E.row[g_E.cy];
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