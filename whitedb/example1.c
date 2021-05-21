#include <whitedb/dbapi.h>

int main(int argc, char **argv) 
{
  void *db;
  
  db = wg_attach_database("1000", 2000000);
  
  return 0;
}
