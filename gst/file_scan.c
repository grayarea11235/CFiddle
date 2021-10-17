// Scan a directory for audio files
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

void scan_dir(const char *directory, int depth)
{
  printf("Directory is %s\n", directory);
  DIR *dp;
  struct dirent *entry;
  struct stat statbuf;

  if((dp = opendir(directory)) == NULL)
  {
    fprintf(stderr,"cannot open directory: %s\n", directory);
    return;
  }

  chdir(directory);

  while((entry = readdir(dp)) != NULL)
  {
    lstat(entry->d_name,&statbuf);

    if(S_ISDIR(statbuf.st_mode))
    {
      /* Found a directory, but ignore . and .. */
      if(strcmp(".",entry->d_name) == 0 ||
	 strcmp("..",entry->d_name) == 0)
	continue;

      printf("%*s%s/\n",depth,"",entry->d_name);

      /* Recurse at a new indent level */
      scan_dir(entry->d_name, depth+4);
    }
    else
    {
      printf("%*s%s\n",depth,"", entry->d_name);
    }
  }

  chdir("..");

  closedir(dp);
}

int main(int argc, char *argv[])
{
  scan_dir("/media/cpd/TOSHIBA/Audiobooks/Gregory Benford - Galactic Center - 01 - In The Ocean Of Night", 0);

  
  return 0;
}
