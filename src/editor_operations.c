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