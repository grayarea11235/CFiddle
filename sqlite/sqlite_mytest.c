#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

void create_books()
{
}

int main(void) 
{
  sqlite3 *db;
  sqlite3_stmt *res;

  int rc = sqlite_open("mydb.db", &db);

  if (rc != SQLITE_OK) 
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return 1;
  }

  rc = sqlite3_prepare_v2(db, "CREATE TABLE books", -1, &res, 0);    

  if (rc != SQLITE_OK) 
  {

    fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return 1;
  }    

  sqlite3_close(db);

  return EXIT_SUCCESS;
}
