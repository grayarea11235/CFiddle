#ifndef DATASTORE_H
#define DATASTORE_H

#include <sqlite3.h>

typedef struct _datastore_t
{
  char *datastore_name;
  sqlite3 *db;
} datastore_t;


typedef struct _tracks_t
{
  int track_id;
  char *track_name;
  int track_number;
  char *directory;
  char *album;
} tracks_t;

extern datastore_t *open_datastore(const char *datastore_name);
extern void close_datastore(datastore_t *ds);

extern int scan_directory(const char *directory);

#endif
