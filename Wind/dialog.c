#include <windows.h>

#include "dialog.h"

// Basic message dialog
int message_box(char *text, char *caption)
{
  return MessageBoxEx(NULL, caption, text, MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
}
