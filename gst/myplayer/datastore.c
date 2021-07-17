#include <sqlite3.h>
#include <stdio.h>
#include <glib.h>

// DB Structure


// CREATE TABLE tracks(
//     track_id INTEGER PRIMARY KEY,
//     track_name TEXT NOT NULL,
//     track_number INTEGER,
//     directory TEXT NOT NULL,
//     album TEXT NOT NULL)

// CREATE TABLE directories
// CREATE TABLE track_state


int add_track(const char *track_name,
	      int track_number,
	      const char *directory,
	      const char *album)
{
  g_info("Adding new track to store\n");
  g_info("track_name   = %s\n", track_name);
  g_info("track_number = %d\n", track_number);
  g_info("directory    = %s\n", directory);
  g_info("album        = %s\n", album);

  
  return 0;
}

int scan_directory(const char *dir)
{
  return 0;
}

int exec_sql(sqlite3 *db,
	     const char *sql)
{
  char *err_msg = 0;

  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if(rc != SQLITE_OK )
  {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    
    sqlite3_free(err_msg);        
//    sqlite3_close(db);

    return 1;
  }
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

  char *sql = "DROP TABLE IF EXISTS Cars;" 
    "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);" 
    "INSERT INTO Cars VALUES(1, 'Audi', 52642);" 
    "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);" 
    "INSERT INTO Cars VALUES(3, 'Skoda', 9000);" 
    "INSERT INTO Cars VALUES(4, 'Volvo', 29000);" 
    "INSERT INTO Cars VALUES(5, 'Bentley', 350000);" 
    "INSERT INTO Cars VALUES(6, 'Citroen', 21000);" 
    "INSERT INTO Cars VALUES(7, 'Hummer', 41400);" 
    "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";
  

  exec_sql(db, sql);
  
  sqlite3_close(db);
  
  return 0;
}

void open_datastore()
{

}

void close_datastore()
{

}

#ifdef SIMP_TEST
int main(int argc, char *argv[])
{
  printf("%s\n", sqlite3_libversion());

  create_datastore("./testdb.db");
}
#endif
