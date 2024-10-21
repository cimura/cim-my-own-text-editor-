#include "cim.h"

// ************************* filetypes *************************
#define HLDB_ENTRIES  (sizeof(HLDB) / sizeof(HLDB[0]))

char  *C_HL_extensions[] = {".c", ".h", ".cpp", NULL};
char  *C_HL_keywords[] = {
    "switch", "if", "while", "for", "break", "continue", "return", "else",
    "struct", "union", "typedef", "static", "enum", "class", "case", "include",

    "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
    "void|", NULL
};
// HLDB stands for "highlight database"
t_syntax HLDB[] = {
  {
    "c",
    C_HL_extensions,
    C_HL_keywords,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
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

  char  **keywords = g_E.syntax->keywords;
  char  *scs = g_E.syntax->singleline_comment_start;
  char  *mcs = g_E.syntax->multiline_comment_start;
  char  *mce = g_E.syntax->multiline_comment_end;

  int scs_len = scs ? strlen(scs) : 0;
  int mcs_len = mcs ? strlen(mcs) : 0;
  int mce_len = mce ? strlen(mce) : 0;

  int prev_sep = 1;
  int in_string = 0;
  int in_comment = (row->idx > 0 && g_E.row[row->idx - 1].hl_open_comment);

  int i = 0;
  while (i < row->rsize)
  {
    char  c = row->render[i];
    unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

    if (scs_len && !in_string && !in_comment)
    {
      if (!strncmp(&row->render[i], scs, scs_len))
      {
        memset(&row->hl[i], HL_COMMENT, row->rsize - i);
        break;
      }
    }

    if (mcs_len && mce_len && !in_string)
    {
      if (in_comment)
      {
        row->hl[i] = HL_MLCOMMENT;
        if (!strncmp(&row->render[i], mce, mce_len))
        {
          memset(&row->hl[i], HL_MLCOMMENT, mce_len);
          i += mce_len;
          in_comment = 0;
          prev_sep = 0;
        }
        else
          i++;
        continue;
      }
      else if (!strncmp(&row->render[i], mcs, mcs_len))
      {
        memset(&row->hl[i], HL_MLCOMMENT, mcs_len);
        i += mcs_len;
        in_comment = 1;
        continue;
      }
    }

    if (g_E.syntax->flags & HL_HIGHLIGHT_STRINGS)
    {
      if (in_string)
      {
        row->hl[i] = HL_STRING;
        if (c == '\\' && i + 1 < row->rsize)
        {
          row->hl[i + 1] = HL_STRING;
          i += 2;
          continue;
        }
        if (c == in_string)
          in_string = 0;
        i++;
        prev_sep = 1;
        continue;
      }
      else
      {
        if (c == '"' || c == '\'')
        {
          in_string = c;
          row->hl[i] = HL_STRING;
          i++;
          continue;
        }
      }
    }    
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
    if (prev_sep)
    {
      int j;
      for (j = 0; keywords[j]; ++j)
      {
        int klen = strlen(keywords[j]);
        int kw2 = keywords[j][klen - 1] == '|';
        if (kw2)
          klen--;
        
        if (!strncmp(&row->render[i], keywords[j], klen) &&
            is_separator(row->render[i + klen]))
        {
          memset(&row->hl[i], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
          i += klen;
          break;
        }
      }
      if (keywords[j] != NULL)
      {
        prev_sep = 0;
        continue;
      }
    }
    prev_sep += is_separator(c);
    i++;
  }

  int changed = (row->hl_open_comment != in_comment);
  row->hl_open_comment = in_comment;
  if (changed && row->idx + 1 < g_E.num_rows)
    update_syntax(&g_E.row[row->idx + 1]);
}

int syntax_to_color(int hl)
{
  switch (hl)
  {
    case HL_COMMENT:
    case HL_MLCOMMENT:
      return (36);
    case HL_KEYWORD1:
      return (93);
    case HL_KEYWORD2:
      return (92);
    case HL_STRING:
      return (35);
    case HL_NUMBER:
      return (95);
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
				int	filerow;
				for (filerow = 0; filerow < g_E.num_rows; ++filerow)
					update_row(&g_E.row[filerow]);
				return;
			}
			i++;
		}
	}
}
