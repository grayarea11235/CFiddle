#include <glib.h>

int main (void)
{
  GList *test_list = NULL;

  test_list = g_list_append(list, "Hello world!");
  printf("The first item is '%s'\n", g_list_first(list)‑>data);

  return EXIT_SUCCESS;
}
