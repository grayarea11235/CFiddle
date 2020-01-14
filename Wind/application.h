#ifndef APPLICATION_H
#define APPLICATION_H

#include "wind.h"

application_t *create_application(HINSTANCE hInstance,
                                  HINSTANCE hPrevInstance,
                                  PSTR szCmdLine,
                                  int iCmdShow);

form_t *get_form_from_hwnd(HWND hwnd);
mouse_move_event_t *create_mouse_move_evt(WPARAM wParam, LPARAM lParam);

#endif
