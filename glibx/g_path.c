// glib extension to make a path library
//
#include <string.h>
#include <stdlib.h>
#include <glib.h>


char *g_path_combine(const char *path1, const char *path2)
{
  int len1 = strlen(path1);
  int len2 = strlen(path2);
  int res = malloc(sizeof(char) * (len1 + len2 + 2));

  strcpy(res, path1);
  strcat(res, G_DIR_SEPARATOR);
  strcat(res, path2);

  return res;
}
