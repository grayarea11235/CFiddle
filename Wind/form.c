#include <windows.h>

#include "form.h"

void on_button_click(void *event)
{

}


// Form callback TODO : Move this out
LRESULT CALLBACK form_wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;

  COLORREF bkgndcolor;
  HPEN pen;
  HBRUSH brush;

  int xPos;
  int yPos;

  form_t *form;
  
  switch (message)
  {
  case WM_ERASEBKGND:
    bkgndcolor = 0x00808080;
    
    pen = CreatePen(PS_SOLID, 1, bkgndcolor);
    brush = CreateSolidBrush(bkgndcolor);
    SelectObject((HDC)wParam, pen);
    SelectObject((HDC)wParam, brush);
    
    GetClientRect(hwnd, &rect);
    
    Rectangle((HDC)wParam, rect.left, rect.top, rect.right, rect.bottom);
    
    return 0;
    
  case WM_CREATE:
    //      PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;

    return 0;

  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:
      // Have a click command
      c_logf(LOG_INFO, "Button Clicked! Child ID = %d Child Window Handle = 0x%04X",
             LOWORD(wParam), lParam);

      break;

    default:
      break;
    }
    
  case WM_PAINT:
    hdc = BeginPaint(hwnd, &ps);

    GetClientRect(hwnd, &rect);
    DrawText(hdc, TEXT ("Hello, Windows!"), -1, &rect,
             DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    EndPaint(hwnd, &ps);

    return 0;

  case WM_LBUTTONDOWN:
    c_log(LOG_INFO, "WM_LBUTTONDOWN\n");
    return 0;
      
  case WM_LBUTTONUP:
    c_log(LOG_INFO, "WM_LBUTTONUP\n");
    return 0;
      
      
  case WM_MOUSEMOVE:
    form = get_form_from_hwnd(hwnd); 
//    c_logf(LOG_INFO, "form hwd = %d", form->hwnd);

    // We can now call the mouse move event if it is defined.
    if (form->mouse_move_ptr)
    {
      mouse_move_event_t *mme = create_mouse_move_evt(wParam, lParam);
      form->mouse_move_ptr(mme);
    }
    
    
//      xPos = GET_X_LPARAM(lParam); 
//      yPos = GET_Y_LPARAM(lParam); 
      
    //c_log(LOG_INFO, "In Mouse Move!\n");
    /* if (fBlocking) */
    /*   { */
    /*     SetCursor (LoadCursor (NULL, IDC_CROSS)) ; */
    /*     DrawBoxOutline (hwnd, ptBeg, ptEnd) ; */
    /*     ptEnd.x = LOWORD (lParam) ; */
    /*     ptEnd.y = HIWORD (lParam) ; */
    /*     DrawBoxOutline (hwnd, ptBeg, ptEnd) ; */
    /*   } */
    return 0;
      
      
  case WM_DESTROY:
    PostQuitMessage(0);

    return 0;
  }
  
  return DefWindowProc(hwnd, message, wParam, lParam);
}

void set_form_size(form_t *form)
{

}


form_t *create_form(application_t *app, HWND parent)
{
  c_log(LOG_INFO, "In create_form\n");
  
  form_t *form = malloc(sizeof(form_t));
  form->control_list = create_list();
  
  static TCHAR szAppName[] = TEXT ("HelloWin");

  form->wndclass.style = CS_HREDRAW | CS_VREDRAW;
  form->wndclass.lpfnWndProc = form_wnd_proc;  form->wndclass.cbClsExtra = 0;
  form->wndclass.cbWndExtra = 0;
  form->wndclass.hInstance = app->hInstance;
  form->wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  form->wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  form->wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  form->wndclass.lpszMenuName = NULL;
  form->wndclass.lpszClassName = szAppName;

  if (!RegisterClass(&form->wndclass))
  {
    MessageBox(NULL, TEXT("This program requires Windows NT!"),
               szAppName, MB_ICONERROR);
    return 0;
  }
  
  form->hwnd = CreateWindow(szAppName,                  // window class name
                            TEXT("Form1"),  // window caption
                            WS_OVERLAPPEDWINDOW,        // window style
                            CW_USEDEFAULT,              // initial x position
                            CW_USEDEFAULT,              // initial y position
                            CW_USEDEFAULT,              // initial x size
                            CW_USEDEFAULT,              // initial y size
                            NULL,                       // parent window handle
                            NULL,                       // window menu handle
                            app->hInstance,             // program instance handle
                            NULL);                      // creation parameters
  
  ShowWindow(form->hwnd, app->iCmdShow);
  UpdateWindow(form->hwnd);

  return form;
}

form_t *create_child_form(application_t *app, HWND parent)
{
  c_log(LOG_INFO, "In create_form\n");
  
  form_t *form = malloc(sizeof(form_t));
  
  static TCHAR szAppName[] = TEXT ("HelloWin");

  form->wndclass.style = CS_HREDRAW | CS_VREDRAW;
  form->wndclass.lpfnWndProc = form_wnd_proc;
  form->wndclass.cbClsExtra = 0;
  form->wndclass.cbWndExtra = 0;
  form->wndclass.hInstance = app->hInstance;
  form->wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  form->wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  form->wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  form->wndclass.lpszMenuName = NULL;
  form->wndclass.lpszClassName = szAppName;

  if (!RegisterClass(&form->wndclass))
  {
    MessageBox(NULL, TEXT ("This program requires Windows NT!"),
               szAppName, MB_ICONERROR);
    return 0;
  }
  
  form->hwnd = CreateWindow(szAppName,                  // window class name
                            TEXT("Form1"),  // window caption
                            WS_OVERLAPPEDWINDOW,        // window style

                            CW_USEDEFAULT,              // initial x position
                            CW_USEDEFAULT,              // initial y position
                            CW_USEDEFAULT,              // initial x size
                            CW_USEDEFAULT,              // initial y size

                            NULL,                       // parent window handle
                            NULL,                       // window menu handle
                            app->hInstance,             // program instance handle
                            NULL);                      // creation parameters
  
  ShowWindow(form->hwnd, app->iCmdShow);
  UpdateWindow(form->hwnd);

  return form;
}


// form code


