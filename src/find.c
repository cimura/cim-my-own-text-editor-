#include "cim.h"

void  find_callback(char *query, int key)
{
  static int  last_match = -1;
  static int  direction = 1;

  static int saved_hl_line;
  static char *saved_hl = NULL;

  if (saved_hl)
  {
    memcpy(g_E.row[saved_hl_line].hl, saved_hl, g_E.row[saved_hl_line].rsize);
    free(saved_hl);
    saved_hl = NULL;
  }
  if (key == '\r' || key == '\x1b')
  {
    last_match = -1;
    direction = 1;
    return;
  }
  else if (key == ARROW_RIGHT || key == ARROW_DOWN)
    direction = 1;
  else if (key == ARROW_LEFT || key == ARROW_UP)
    direction = -1;
  else
  {
    last_match = -1;
    direction = 1;
  }

  if (last_match == -1)
    direction = 1;
  
  int current = last_match;
  int  i;
  for (i = 0; i < g_E.num_rows; ++i)
  {
    current += direction;
    if (current == -1)
      current = g_E.num_rows - 1;
    else if (current == g_E.num_rows)
      current = 0;

    t_row *row = &g_E.row[current];
    char  *match = strstr(row->render, query);
    if (match)
    {
      last_match = current;
      g_E.cy = current;
      g_E.cx = row_rx_to_cx(row, match - row->render);
      g_E.row_off = g_E.num_rows;

      saved_hl_line = current;
      saved_hl = malloc(row->rsize);
      memcpy(saved_hl, row->hl, row->rsize);
      memset(&row->hl[match - row->render], HL_MATCH, strlen(query));
      break;
    }
  }
}

void  find()
{
  int saved_cx = g_E.cx;
  int saved_cy = g_E.cy;
  int saved_coloff  = g_E.col_off;
  int saved_rowoff = g_E.row_off;

  char  *query = editor_prompt("Search: %s (Use ESC/Arrows/Enter)", find_callback);
  if (query)
    free(query);
  else
  {
    g_E.cx = saved_cx;
    g_E.cy = saved_cy;
    g_E.col_off = saved_coloff;
    g_E.row_off = saved_rowoff;
  }
}