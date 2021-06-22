#include <glib.h>
#include <stdio.h>

int main (void)
{
  GList *test_list = NULL;

  test_list = g_list_append(test_list, "Hello world!");

  for (int i = 0; i < 1000; ++i)
  {
    g_list_append(test_list, "Chickens");
  }

  printf("The first item is \"%s\"\n", (char *)g_list_first(test_list)->data);

  return EXIT_SUCCESS;
}
