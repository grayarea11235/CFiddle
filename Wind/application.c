#include <windows.h>

#include "application.h"

//void add_form_to_application(application_t *app, form_t *form)


application_t *create_application(HINSTANCE hInstance,
                                  HINSTANCE hPrevInstance,
                                  PSTR szCmdLine,
                                  int iCmdShow)
{
  application_t *new_app = malloc(sizeof(application_t));

  new_app->hInstance = hInstance;
  new_app->hPrevInstance = hPrevInstance;
  new_app->szCmdLine = szCmdLine;
  new_app->iCmdShow = iCmdShow;
  
  return new_app;
}

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



