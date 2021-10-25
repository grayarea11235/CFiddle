#include <stdio.h>
#include <sys/statvfs.h>


long get_disk_space(const char *path)
{
  struct statvfs stat;

  int rc = statvfs(path, &stat);
  if (rc == 0)
  {
    printf("f_bsize  = %ld\n", stat.f_bsize);
    printf("f_favail = %ld\n", stat.f_favail);

    printf("Priv Space available   = %ld\n", stat.f_bsize * stat.f_bfree);
    printf("Unpriv Space available = %ld\n", stat.f_bsize * stat.f_bavail);
  }

  return stat.f_bsize * stat.f_bavail;
}

int main(int argc, char *argv[])
{
  get_disk_space("/");

  return 0;
}
