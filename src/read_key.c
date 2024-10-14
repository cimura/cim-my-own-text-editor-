#include "kilo.h"

void  editor_move_cursor(int key)
{
  switch (key) {
    case ARROW_LEFT:
      if (g_E.cx != 0)
       g_E.cx--;
      break;
    case ARROW_RIGHT:
      g_E.cx++;
      break;
    case ARROW_UP:
      if (g_E.cy != 0)
        g_E.cy--;
      break;
    case ARROW_DOWN:
      if (g_E.cy < g_E.num_rows)
        g_E.cy++;
      break;
  }
}

int  editor_read_key()
{
  ssize_t nread;
  char  c;

  while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
  {
    if (nread == -1 && errno != EAGAIN)
      error_exit("read");
  }
  
  if (c == '\x1b')
  {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1)
      return ('\x1b');
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
      return ('\x1b');
    
    if (seq[0] == '[')
    {
      if (seq[1] >= '0' && seq[1] <= '9')
      {
        if (read(STDIN_FILENO, &seq[2], 1) != 1)
          return ('\x1b');
        if (seq[2] == '~')
        {
          switch (seq[1]) {
            case '1': return (HOME_KEY);
            case '3': return (DEL_KEY);
            case '4': return (END_KEY);
            case '5': return (PAGE_UP);
            case '6': return (PAGE_DOWN);
            case '7': return (HOME_KEY);
            case '8': return (END_KEY);
          }
        }
      }
      else
      {
          switch (seq[1]) {
            case 'A': return (ARROW_UP);
            case 'B': return (ARROW_DOWN);
            case 'C': return (ARROW_RIGHT);
            case 'D': return (ARROW_LEFT);
            case 'H': return (HOME_KEY);
            case 'F': return (END_KEY);
        }
      }
    }
    else if (seq[0] == 'O')
    {
      switch (seq[1]) {
        case 'H': return (HOME_KEY);
        case 'F': return (END_KEY);
      }
    }
    return ('\x1b'); 
  }
  else
    return (c);
}

void  editor_process(void)
{
  int  c = editor_read_key();
  switch (c) {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(EXIT_SUCCESS);
      break;
    
    case  HOME_KEY:
      g_E.cx = 0;
      break;
    
    case  END_KEY:
      g_E.cx = g_E.screen_cols - 1;
      break;
    
    case  PAGE_UP:
    case  PAGE_DOWN:
    {
      int times = g_E.screen_rows;
      while (times--)
        editor_move_cursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
    }
    break ;
    
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editor_move_cursor(c);
      break;
  }
}