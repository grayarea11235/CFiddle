#ifndef DIRTYLOG_H
#define DIRTYLOG_H

void dirty_log(const char *msg)
{
  FILE *f = fopen("dirty_log.txt", "at");
  time_t rawtime;
  struct tm * timeinfo;
  char time_str[32];
  
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  // Max size of time_str is 26
  asctime_r(timeinfo, time_str);
  if (time_str[strlen(time_str) - 1] == '\n')
  {
    time_str[strlen(time_str) - 1] = '\0';
  }
  
  fprintf(f, "%s : %s\n", time_str, msg);
   
  fclose(f);
}

#endif
