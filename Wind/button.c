#include <windows.h>

#include "wind.h"
#include "button.h"

button_t *create_button(create_data_t *data)
{
  button_t *new_btn = malloc(sizeof(button_t));
  HWND hwndButton;

//  new_btn->

  
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

  return new_btn;
}

/*
button_t *create_button(HINSTANCE hInst, HWND hwnd, const char *name)
{
  button_t *new_btn = malloc(sizeof(button_t));
  HWND hwndButton;
  
  hwndButton = CreateWindow(TEXT("button"),
                            TEXT("Some Button"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            10, // X
                            10, // Y
                            100, // Width
                            25, // Height
                            hwnd,
                            NULL, // Menu
                            hInst,
                            NULL);

  return new_btn;
}
*/
