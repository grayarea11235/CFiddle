#include <sqlite3.h>
#include <stdlib.h>
#include <glib.h>

int scan_audio_dir(const char *directory)
{
  int res = 0;

  GDir *dir;
  GError *error;
  const gchar *filename;

  dir = g_dir_open(".", 0, &error);
  while ((filename = g_dir_read_name(dir)))
    printf("%s\n", filename);


  return res;
}


int main(void)
{

  return EXIT_SUCCESS;
}
