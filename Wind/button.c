#include <windows.h>

#include "wind.h"
#include "button.h"
#include "log.h"

button_t *create_button(create_data_t *data)
{
  button_t *new_btn = malloc(sizeof(button_t));
  HWND hwndButton;

  new_btn->type = CT_BUTTON;
  
  hwndButton = CreateWindow(TEXT("button"),
                            TEXT(data->name),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            data->x, // X
                            data->y, // Y
                            data->width, // Width
                            data->height, // Height
                            data->parent,
                            NULL, // Menu
                            data->app->hInstance,
                            NULL);

  c_logf(LOG_INFO, "Have created button with handle 0x%04X\n", hwndButton);

  new_btn->hwnd = hwndButton;
  
  return new_btn;
}
