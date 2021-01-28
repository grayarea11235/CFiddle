#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


typedef struct _file_info
{
  char *file_name;
  long size;
} file_info;


void scan_dir(char *path)
{
  // /home/cpd/Downloads/Serhii\ Plokhy/Chernobyl\ -\ \ History\ of\ a\ Tragedy/
  // /home/cpd/Downloads/Serhii Plokhy/Chernobyl -  History of a Tragedy
  DIR *dp;
  struct dirent *ep;     
  struct stat statbuf;

  dp = opendir("/home/cpd/Downloads/Serhii Plokhy/Chernobyl -  History of a Tragedy");

  if (dp != NULL)
  {
    while((ep = readdir(dp)) != 0)
    {
      printf("FILE=%s\n", ep->d_name); 

      if (stat(ep->d_name, &statbuf) != -1)
      {
        printf("Filename = \"%s\" st_size = %ld\n", 
            ep->d_name, 
            statbuf.st_size);
      }
      else
      {
        perror("Error on stat!");
      }
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
