#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <glib/gstdio.h>
#include <glib.h>

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
    GStatBuf buf;

    g_stat(filename, &buf);

<<<<<<< HEAD
    g_print("buf.st_size=%ld\n", buf.st_size);   
    
=======
    //g_print("%d\n", buf
>>>>>>> f87ceba26f29887706a692ad1afbd7c764b325ad
  }

  return res;
}


int main(int argc, char *argv[])
{
//  scan_audio_dir("/home/cpd/Downloads/Ursula K. Le Guin/Hainish Cycle/01 - The Dispossessed");
//  scan_audio_dir("/home/cpd/Downloads/Dennis E. Taylor - 2020 - Bobiverse, Book 4 - Heaven's River (Sci-Fi)/");

  g_print("argc = %d\n", argc);

  if (argc != 2)
  {
    g_print("Usage : %s <directory>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // /home/cpd/Downloads/earthsea/1.1\ -\ Shadow.mp3 
  scan_audio_dir(argv[1]);

  return EXIT_SUCCESS;
}
