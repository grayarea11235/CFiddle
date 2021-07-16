#include <sqlite3.h>
#include <stdio.h>

void create_datastore()
{

}



#ifdef SIMP_TEST
int main(int argc, char *argv[])
{
  printf("%s\n", sqlite3_libversion());
}
#endif
