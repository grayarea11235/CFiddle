#include <windows.h>
#include <stdio.h>
#include <ctype.h>

#include "log.h"


// ---------------------------------------------------------
// Simple logging with no va_args
// ---------------------------------------------------------
void c_log(int level, char *msg)
{
  OutputDebugString(msg);
}

void c_logf(int level, char *format, ...)
{
  char    buf[4096], *p = buf;
  va_list args;
  int     n;
  
  va_start(args, format);
  n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
  va_end(args);
  
  p += (n < 0) ? sizeof buf - 3 : n;
  
  while ( p > buf  &&  isspace(p[-1]) )
    *--p = '\0';
  
  *p++ = '\r';
  *p++ = '\n';
  *p   = '\0';
  
  OutputDebugString(buf);
}
