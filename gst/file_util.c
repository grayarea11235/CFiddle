#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <apr.h>

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

  dp = opendir(path);

  if (dp != NULL)
  {
    while((ep = readdir(dp)) != 0)
    {
      char *full_path = malloc(strlen(path) + strlen(ep->d_name) + 2);
      strcpy(full_path, path);
      strcat(full_path, "/");
      strcat(full_path, ep->d_name);
      printf("full_path = %s ", full_path);


      apr_status_t res;
      res = apr_filepath_merge(char **  	newpath,
          path,
          ep->d_name,
          apr_int32_t  	flags,
          apr_pool_t *  	p);

      if (stat(full_path, &statbuf) != -1)
      {
        printf("st_size = %ld\n", 
            statbuf.st_size);
      }
      else
      {
        //perror("Error on stat!");
        printf("Error on stat!\n");
      }

      free(full_path);
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
  scan_dir("/home/cpd/Downloads/Serhii Plokhy/Chernobyl -  History of a Tragedy/");
}
