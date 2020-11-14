#include <windows.h>

#include "checkbox.h"
#include "log.h"

checkbox_t *create_checkbox(create_data_t *data)
{
  checkbox_t *new_chkbox = malloc(sizeof(checkbox_t));
  HWND hwndChkbox;

  new_chkbox->type = CT_CHECKBOX;
  
  hwndChkbox = CreateWindow(TEXT("checkbox"),
                            TEXT(data->name),
                            WS_CHILD | WS_VISIBLE | BS_CHECKBOX /*| WS_TABSTOP*/,
                            data->x, // X
                            data->y, // Y
                            data->width, // Width
                            data->height, // Height
                            data->parent,
                            NULL, // Menu
                            data->app->hInstance,
                            NULL);

  c_logf(LOG_INFO, "Have created checkbox with handle 0x%04X\n", hwndChkbox);

  new_chkbox->hwnd = hwndChkbox;
  
  return new_chkbox;
}
