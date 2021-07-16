#include <sqlite3.h>
#include <stdio.h>

void create_datastore(const char *database_name)
{
  sqlite3 *db;
  sqlite3_stmt *res;
    
  int rc = sqlite3_open(":memory:", &db);

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    
    return;
  }
}

#ifdef SIMP_TEST
int main(int argc, char *argv[])
{
  printf("%s\n", sqlite3_libversion());

  create_datastore("~/testdb.db");
  
}
#endif
