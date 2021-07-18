#include <sqlite3.h>
#include <stdio.h>
#include <glib.h>
#include <assert.h>
#include <dirent.h>
#include <glib/gstdio.h>
 
#include "datastore.h"

// DB Structure

// CREATE TABLE tracks(
//     track_id INTEGER PRIMARY KEY,
//     track_name TEXT NOT NULL,
//     track_number INTEGER,
//     directory TEXT NOT NULL,
//     album TEXT NOT NULL)

// CREATE TABLE directories
// CREATE TABLE track_state
// CREATE TABLE albums


int add_track(const datastore_t *ds,
	      sqlite3 *db,
	      const tracks_t *track)
{
  sqlite3_stmt *stmt;
  char *err_msg = 0;
  
  g_info("Adding new track to store\n");
  g_info("track_name   = %s\n", track->track_name);
  g_info("track_number = %d\n", track->track_number);
  g_info("directory    = %s\n", track->directory);
  g_info("album        = %s\n", track->album);

  const char *sql = "INSERT INTO tracks(track_name, track_number, directory, album) VALUES (?, ?, ?, ?)";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);    
  
  if (rc != SQLITE_OK)
  {
    //fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    //sqlite3_close(db);
    
    return 1;
  }    

  int c = 1;
  rc = sqlite3_bind_text(stmt, c++, track->track_name, strlen(track->track_name), NULL);
  rc = sqlite3_bind_int(stmt, c++, track->track_number);
  rc = sqlite3_bind_text(stmt, c++, track->track_name, strlen(track->track_name), NULL);
  rc = sqlite3_bind_text(stmt, c++, track->track_name, strlen(track->track_name), NULL);

  
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    
    sqlite3_free(err_msg);        
    sqlite3_close(db);
    
    return 1;
  } 
  
  return 0;
}

int scan_directory(const char *directory)
{
  DIR *dir = opendir(directory);

  if (dir == NULL)
  {
    g_error("Unable to read directory");

    return 1;
  }
  
  struct dirent *entry;
  int files = 0;

  while ((entry = readdir(dir)))
  {
    files++;
    g_print("File %3d: %s %s\n", files,
	    entry->d_name, directory);
    g_print("Full name = %s\n",
     	    g_build_filename(directory, entry->d_name, NULL));
  }
  
  closedir(dir);

  return 0;
}

int exec_sql(sqlite3 *db,
	     const char *sql)
{
  char *err_msg = 0;

  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    
    sqlite3_free(err_msg);        
//    sqlite3_close(db);

    return 1;
  }
}

int create_tracks_table(sqlite3 *db)
{
  const char *sql =
    "DROP TABLE IF EXISTS tracks;"
    "CREATE TABLE tracks(track_id INTEGER PRIMARY KEY,"
    "track_name TEXT NOT NULL,"
    "track_number INTEGER,"
    "directory TEXT NOT NULL,"
    "album TEXT NOT NULL);";

  exec_sql(db, sql);
}

int create_datastore(const char *database_name)
{
  sqlite3 *db;
  sqlite3_stmt *res;
	
  int rc = sqlite3_open(database_name, &db);
  
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    
    return 1;
  }
  
  rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);    

  printf("rc = %d\n", rc);
  
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
        
    return 1;
  }

  /* char *sql = "DROP TABLE IF EXISTS Cars;"  */
  /*   "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"  */
  /*   "INSERT INTO Cars VALUES(1, 'Audi', 52642);"  */
  /*   "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"  */
  /*   "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"  */
  /*   "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"  */
  /*   "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"  */
  /*   "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"  */
  /*   "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"  */
  /*   "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);"; */
  

  /* exec_sql(db, sql); */

  create_tracks_table(db);
  
  sqlite3_close(db);
  
  return 0;
}

datastore_t *open_datastore(const char *datastore_name)
{
  datastore_t *res = malloc(sizeof(datastore_t));
  res->datastore_name = strdup(datastore_name);
			    
  return res;
}

void close_datastore(datastore_t *ds)
{
  free(ds->datastore_name);
  free(ds);
}

#ifdef SIMP_TEST
int main(int argc, char *argv[])
{
  printf("%s\n", sqlite3_libversion());

  create_datastore("./testdb.db");
}
#endif
