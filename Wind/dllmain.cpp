/*
 *
 */

#include <windows.h>
#include "wind.h"


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;
  
  switch (message)
    {
    case WM_CREATE:
      //      PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;

      return 0;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &rect);
      DrawText(hdc, TEXT ("Hello, Windows!"), -1, &rect,
               DT_SINGLELINE | DT_CENTER | DT_VCENTER);
      EndPaint(hwnd, &ps);

      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);

      return 0;
    }
  
  return DefWindowProc(hwnd, message, wParam, lParam);
}


Application *wCreateApp(char *name)
{
  //  Application *app = new Application

  return NULL;
}

void wCreateForm()
{
  
}


__stdcall void hello(const char *s)
{
  printf("Hello %s\n", s);
}
