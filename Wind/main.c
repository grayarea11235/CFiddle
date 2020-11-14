#include <windows.h>

#include "wind.h"
#include "form.h"

#include "application.h"
#include "button.h"
#include "checkbox.h"

#include "linked_list.h"
#include "log.h"


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

void mouse_ldown(void *event)
{

}

void mouse_click(void *event)
{
}


// TODO
//
// How does control_t work in C? Generic selectino in C11 - look at
// 0. Linked list
// 1. Create form list in application
// 1.5 Organise files
// 2. Implement universal colors
// 3. Complete form
// 4. Create basic button
// 5. Button events
//

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine,
                   int iCmdShow)
{
  c_log(LOG_INFO, "In main!\n");

  app = create_application(hInstance, hPrevInstance, szCmdLine, iCmdShow);

  form_t *form = create_form(app, NULL);
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

  create_data.name = "Test 1";
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

  create_data.name = "Checkbox 1";
  create_data.x = 70;
  create_data.y = 110;
  create_data.width = 100;
  create_data.height = 25;
  create_data.parent = form->hwnd;
  create_data.app = app;

  checkbox_t *chkbox1 = create_checkbox(&create_data);

  
  int ret = wind_run();
  
  c_log(LOG_INFO, "Exiting!\n");
  return ret;
}
