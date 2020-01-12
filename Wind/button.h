#ifndef BUTTON_H
#define BUTTON_H

#include "wind.h"

typedef struct _button_t
{
  CONTROL_COMMON
  
  // Win32 specific data
  WNDCLASS wndclass;
  HWND hwnd;
  
} button_t;


//button_t *create_button(HINSTANCE hInst, HWND hwnd, const char *name);
button_t *create_button(create_data_t *data);

#endif
