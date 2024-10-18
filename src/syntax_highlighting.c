#include "cim.h"

// ************************* filetypes *************************
#define HLDB_ENTRIES  (sizeof(HLDB) / sizeof(HLDB[0]))

char  *C_HL_extensions[] = {".c", ".h", ".cpp", NULL};

// HLDB stands for "highlight database"
t_syntax HLDB[] = {
  {
    "c",
    C_HL_extensions,
    HL_HIGHLIGHT_NUMBERS
  },
};
// *************************************************************

static int  is_separator(int c)
{
  return (isspace(c) || c == '\0' || strchr(",.()+-/*=~%<>[];", c) != NULL);
}

void  update_syntax(t_row *row)
{
  row->hl = realloc(row->hl, row->rsize);
  memset(row->hl, HL_NORMAL, row->rsize);

  if (!g_E.syntax)
    return;

  int prev_sep = 1;

  int i = 0;
  while (i < row->rsize)
  {
    char  c = row->render[i];
    unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

    if (g_E.syntax->flags & HL_HIGHLIGHT_NUMBERS)
    {
      if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) ||
        (c == '.' && prev_hl == HL_NUMBER))
        {
          row->hl[i] = HL_NUMBER;
          i++;
          prev_sep = 0;
          continue;
        }
    }
    prev_sep += is_separator(c);
    i++;
  }
}

int syntax_to_color(int hl)
{
  switch (hl)
  {
    case HL_NUMBER:
      return (31);
    
    case HL_MATCH:
      return (34);
    default:
      return (37);
  }
}

void  select_syntax_highlight()
{
  g_E.syntax = NULL;
  if (g_E.file_name == NULL)
    return;
  
  char  *ext = strrchr(g_E.file_name, '.');

  for (unsigned int j = 0; j < HLDB_ENTRIES; ++j)
  {
    t_syntax  *s = &HLDB[j];
    unsigned int  i = 0;
    while (s->file_match[i])
    {
      int is_ext = (s->file_match[i][0] == '.');
      if ((is_ext && ext && !strcmp(ext, s->file_match[i])) ||
          (!is_ext && strstr(g_E.file_name, s->file_match[i])))
        {
          g_E.syntax = s;
          return;
        }
      i++;
    }
  }
}