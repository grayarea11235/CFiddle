#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <assert.h>

void init_config()
{
  struct stat st = {0};

  g_print("In init_config\n");
  
  if (stat("~/.config", &st) == -1)
  {
    g_print("Creating config dir\n");
    assert(mkdir("~/.config", 0700) != -1);
  }

  if (stat("~/.config/mply", &st) == -1)
  {
    g_print("Creating mply dir\n");
    assert(mkdir("~/mply", 0700) != -1);
  }
}
