// Scan a directory for audio files
//
// Build this with:
// gcc -I/usr/include/taglib ./file_scan.c -o file_scan -ltag_c -ltag

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include <sqlite3.h>

#include <tag_c.h>


void file_info(const char *full_name)
{
  TagLib_File *file;
  TagLib_Tag *tag;
  const TagLib_AudioProperties *properties;

  printf("full_name=%s\n", full_name);
  
  file = taglib_file_new(full_name);

  if (file == NULL)
  {
    printf("file is NULL\n");
    return;
  }

  tag = taglib_file_tag(file);
  properties = taglib_file_audioproperties(file);

  if(tag != NULL)
  {
    printf("-- TAG --\n");
    printf("title   - \"%s\"\n", taglib_tag_title(tag));
    printf("artist  - \"%s\"\n", taglib_tag_artist(tag));
    printf("album   - \"%s\"\n", taglib_tag_album(tag));
    printf("year    - \"%i\"\n", taglib_tag_year(tag));
    printf("comment - \"%s\"\n", taglib_tag_comment(tag));
    printf("track   - \"%i\"\n", taglib_tag_track(tag));
    printf("genre   - \"%s\"\n", taglib_tag_genre(tag));
  }

  // Store tag data in a database
}


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

//      printf("%*s%s/\n",depth,"",entry->d_name);

//      printf("Fule path = %s\n", buf);
      
      /* Recurse at a new indent level */
      scan_dir(entry->d_name, depth+4);
    }
    else
    {
//      printf("%*s%s\n",depth,"", entry->d_name);
      char full_path[PATH_MAX + 1];
      realpath(entry->d_name, full_path);

      file_info(full_path);
    }
  }

  chdir("..");

  closedir(dp);
}


int main(int argc, char *argv[])
{
  char *dir_name = "/media/cpd/1233-621A/Audiobooks/";
  //  scan_dir("/media/cpd/TOSHIBA/Audiobooks/Gregory Benford - Galactic Center - 01 - In The Ocean Of Night", 0);
//  scan_dir("/media/cpd/TOSHIBA/Audiobooks", 0);
  scan_dir(dir_name, 0);

  
  return 0;
}
