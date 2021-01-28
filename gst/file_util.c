#include <stdio.h>

typedef _file_info
{
  char *file_name;
  long size;
} file_info;


void scan_dir(char *path)
{
  DIR *dp;
  struct dirent *ep;     
  dp = opendir("./");

  if (dp != NULL)
  {
    while(ep = readdir (dp))
    {
      puts(ep->d_name);
    }

    closedir(dp);
  }
  else
  {
    perror("Couldn't open the directory");
  }
}

int main(void)
{
  scan_dir(NULL);
}
