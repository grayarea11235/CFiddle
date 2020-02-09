#ifndef BUTTON_H
#define BUTTON_H

#include "wind.h"

typedef struct _button_t
{
  CONTROL_COMMON
  
  // Win32 specific data
  WNDCLASS wndclass;
  HWND hwnd;

  // Events
  void (*mouse_move_ptr)(void *event);  
  void (*mouse_click_ptr)(void *event);  
  void (*mouse_dblclick_ptr)(void *event);  
  
} button_t;


//button_t *create_button(HINSTANCE hInst, HWND hwnd, const char *name);
button_t *create_button(create_data_t *data);

#endif
