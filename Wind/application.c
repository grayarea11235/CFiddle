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



