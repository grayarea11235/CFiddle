// Scan a directory for audio files
//
// Build this with:
// gcc -I/usr/include/taglib ./file_scan.c -o file_scan -ltag_c -ltag -lsqlite3

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include <sqlite3.h>

#include <tag_c.h>

#define SQLITE_CALL(X)							\
  {									\
  int rc = sqlite3_##X;							\
  if (rc != SQLITE_OK)							\
  { \
fprintf(stderr, "Call to %s failed. Statis %d : %s\n", #X, rc, sqlite3_errmsg(db)); \
sqlite3_close(db); \
exit(1);								\
}									\
}									\


struct tag_info
{
  char *title;
  char *artist;
  char *album;
  int year;
  char *comment;
  int track;
  char *genre;
};

void dump_tag_info(struct tag_info *taginfo)
{
  printf("TAG INFO\n");
  printf("  title   = %s\n", taginfo->title);
  printf("  artist  = %s\n", taginfo->artist);
  printf("  album   = %s\n", taginfo->album);
  printf("  year    = %d\n", taginfo->year);
  printf("  comment = %s\n", taginfo->comment);
  printf("  track   = %d\n", taginfo->track);
  printf("  genre   = %s\n", taginfo->genre);  
}


/*
CREATE TABLE "tracks" (
	"id"	INTEGER NOT NULL UNIQUE,
	"title"	TEXT,
	"artist"	TEXT,
	"album"	TEXT,
	"year"	INTEGER,
	"comment"	TEXT,
	"track"	INTEGER,
	"genre"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
*/
void add_track_record(struct tag_info *taginfo)
{
  sqlite3 *db;
  sqlite3_stmt *res;

  SQLITE_CALL(open("/home/cpd/Devel/CFiddle/gst/tracks.db", &db));
  
  char *sql = "INSERT INTO tracks VALUES(?, ?, ?, ?, ?, ?, ?, ?);";
  sqlite3_stmt *stmt;
  
  SQLITE_CALL(prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL));
    
  SQLITE_CALL(bind_null(stmt, 1));
  SQLITE_CALL(bind_text(stmt, 2, taginfo->title, strlen(taginfo->title),
			0 /* The callback. */));
  SQLITE_CALL(bind_text(stmt, 3, taginfo->artist, strlen(taginfo->artist),
			0 /* The callback. */));
  SQLITE_CALL(bind_text(stmt, 4, taginfo->album, strlen(taginfo->album),
			0 /* The callback. */));
  SQLITE_CALL(bind_int(stmt, 5, taginfo->year));
  SQLITE_CALL(bind_text(stmt, 6, taginfo->comment, strlen(taginfo->comment),
			0 /* The callback. */));
  SQLITE_CALL(bind_int(stmt, 7, taginfo->track));
  SQLITE_CALL(bind_text(stmt, 8, taginfo->genre, strlen(taginfo->genre),
			0 /* The callback. */));


  int s = sqlite3_step(stmt);
}

void file_info(const char *full_name)
{
  TagLib_File *file;
  TagLib_Tag *tag;
  const TagLib_AudioProperties *properties;

//  printf("full_name=%s\n", full_name);
   
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
    struct tag_info taginfo;
    taginfo.title = strdup(taglib_tag_title(tag));
    taginfo.artist = strdup(taglib_tag_artist(tag));
    taginfo.album = strdup(taglib_tag_album(tag));
    taginfo.year = taglib_tag_year(tag);
    taginfo.comment = strdup(taglib_tag_comment(tag));
    taginfo.track = taglib_tag_track(tag);
    taginfo.genre = strdup(taglib_tag_genre(tag));

//    dump_tag_info(&taginfo);

    add_track_record(&taginfo);
    
    /* printf("-- TAG --\n"); */
    /* printf("title   - \"%s\"\n", taglib_tag_title(tag)); */
    /* printf("artist  - \"%s\"\n", taglib_tag_artist(tag)); */
    /* printf("album   - \"%s\"\n", taglib_tag_album(tag)); */
    /* printf("year    - \"%i\"\n", taglib_tag_year(tag)); */
    /* printf("comment - \"%s\"\n", taglib_tag_comment(tag)); */
    /* printf("track   - \"%i\"\n", taglib_tag_track(tag)); */
    /* printf("genre   - \"%s\"\n", taglib_tag_genre(tag)); */
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

  printf("%s\n", sqlite3_libversion()); 

  scan_dir(dir_name, 0);

  
  return 0;
}
