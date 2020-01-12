#include <windows.h>

#include "wind.h"

#include "application.h"
#include "button.h"

#include "linked_list.h"
#include "log.h"



form_t *get_form_from_hwnd(HWND hwnd)
{
  form_t *ret = NULL;
  list_t *form_list = app->form_list;
  int c = get_count(form_list);
  list_item_t *il = form_list->head;

  for (int i = 0; i < c; i++)
  {
    form_t *f = (form_t *)il->data;
    if (f->hwnd == hwnd)
    {
      ret = f;
      break;
    }
    il = (list_item_t *)il->next;
  }

  return ret;
}


mouse_move_event_t *create_mouse_move_evt(WPARAM wParam, LPARAM lParam)
{
  mouse_move_event_t *mme = malloc(sizeof(mouse_move_event_t));
  
  mme->pnt.x = LOWORD(lParam); 
  mme->pnt.y = HIWORD(lParam); 
  
  if (wParam & MK_CONTROL)
  {
    mme->btn.ctrl = 1;
  }
  if (wParam & MK_LBUTTON)
  {
    mme->btn.lbtn = 1;
  }
  if (wParam & MK_MBUTTON)
  {
    mme->btn.mbtn = 1;
  }
  if (wParam & MK_RBUTTON)
  {
    mme->btn.rbtn = 1;
  }
  if (wParam & MK_SHIFT)
  {
    mme->btn.shft = 1;
  }
  if (wParam & MK_XBUTTON1)
  {
    mme->xbtn1 = 1;
  }
  if (wParam & MK_XBUTTON2)
  {
    mme->xbtn2 = 1;
  }
  
  return mme;
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

form_t *create_form(application_t *app)
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
                            app->hInstance,                  // program instance handle
                            NULL);                      // creation parameters
  
  ShowWindow(form->hwnd, app->iCmdShow);
  UpdateWindow(form->hwnd);

  return form;
}

DWORD wind_run()
{
  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}


// Example mouse move
void mouse_move(void *event)
{
  mouse_move_event_t *evt = (mouse_move_event_t *) event;

  c_logf(LOG_INFO, "In MouseMoveEvt! x=%d y=%d", evt->pnt.x, evt->pnt.y);

  
}


// TODO
//
// How does control_t work in C? Generic selectino in C11 - look at
// 0. Linked list
// 1. Create form list in application
// 1.5 Organise files
// 2. Imlement universal colors
// 3. Complete form
// 4. Create basic button
//

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine,
                   int iCmdShow)
{
  c_log(LOG_INFO, "In main!\n");

  app = create_application(hInstance, hPrevInstance, szCmdLine, iCmdShow);
  
  form_t *form = create_form(app);
  form->mouse_move_ptr = mouse_move;

  list_t *form_list = create_list();
  add_to_list(form_list, form, sizeof(form_t));

  
  list_item_t *t = get_list_index(form_list, 0);
  if (t)
  {
    form_t *f = (form_t *)t->data;
  }
  
  app->form_list = form_list;

//  create_button(hInstance, form->hwnd, "TestButton");
  
  c_logf(LOG_INFO, "list count = %d\n", get_count(form_list));

  create_data_t create_data;

  create_data.name = "Button 1";
  create_data.x = 10;
  create_data.y = 50;
  create_data.width = 100;
  create_data.height = 25;
  create_data.parent = form->hwnd;
  create_data.app = app;

  button_t *btn2 = create_button(&create_data);

  create_data.name = "Button 2";
  create_data.x = 10;
  create_data.y = 80;
  create_data.width = 100;
  create_data.height = 25;
  create_data.parent = form->hwnd;
  create_data.app = app;

  button_t *btn3 = create_button(&create_data);
  
  int ret = wind_run();
  
  c_log(LOG_INFO, "Exiting!\n");
  return ret;
}
