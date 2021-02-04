#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include </usr/include/glib-2.0/glib.h>

int scan_audio_dir(const char *directory)
{
  int res = 0;

  GDir *dir;
  GError *error;
  const gchar *filename;

  dir = g_dir_open(directory, 0, &error);
  while ((filename = g_dir_read_name(dir)))
  {
    printf("%s\n", filename);
    g_stat(filename);
  }

  return res;
}


int main(int argc, char *argv[])
{
//  scan_audio_dir("/home/cpd/Downloads/Ursula K. Le Guin/Hainish Cycle/01 - The Dispossessed");
  scan_audio_dir("/home/cpd/Downloads/Dennis E. Taylor - 2020 - Bobiverse, Book 4 - Heaven's River (Sci-Fi)/");
  return EXIT_SUCCESS;
}
