#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/statfs.h>

void dump_statvfs(struct statvfs *stat)
{
  printf("f_bsize     = %ld\n", stat->f_bsize);
  printf("f_frsize    = %ld\n", stat->f_frsize);
  printf("f_blocks    = %ld\n", stat->f_blocks);
  printf("f_bfree     = %ld\n", stat->f_bfree);
  printf("f_bavail    = %ld\n", stat->f_bavail);
  printf("f_fsid      = %ld\n", stat->f_fsid);
  printf("f_flag      = 0x%0lX", stat->f_flag);
  printf(" (");
//  if (stat->f_flag && ST_MANDLOCK)
//  {
//    printf(" ST_MANDLOCK ");
//  }
  printf(")\n");
  printf("f_namemax   = %ld\n", stat->f_namemax);
}

long get_disk_space(const char *path)
{
  struct statvfs stat;

  int rc = statvfs(path, &stat);
  dump_statvfs(&stat);

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
