#include <Elementary.h>
 
EAPI_MAIN int
elm_main(int argc, char **argv)
{
  Evas_Object *win;
 
  elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
 
  win = elm_win_util_standard_add("Main", "Hello, World!");
  elm_win_autodel_set(win, EINA_TRUE);
  //win 400x400
  evas_object_resize(win, 400, 400);

  /*basic tutorial code*/
 
  evas_object_show(win);
 
  elm_run();
  return 0;
}

ELM_MAIN()
