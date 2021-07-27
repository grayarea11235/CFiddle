#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "logging.h"


// This has to be a macro stupid
void log_info(const char *format, ...)
{
  va_list argptr;
  va_start(argptr, format);

  // Call snprintf with NULL to get required string size
  size_t buff_size = snprintf(NULL, 0, format, argptr);
  char *msg_buff = malloc(buff_size + 1);
  snprintf(msg_buff, buff_size + 1, format, argptr);

  
  g_info("%s(%s) %d : %s", __func__, __FILE__,
	 __LINE__, msg_buff);

  va_end(argptr);
}

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

static const gchar *log_level_to_string(GLogLevelFlags level)
{
  switch (level)
  {
    case G_LOG_LEVEL_ERROR: return "ERROR";
    case G_LOG_LEVEL_CRITICAL: return "CRITICAL";
    case G_LOG_LEVEL_WARNING: return "WARNING";
    case G_LOG_LEVEL_MESSAGE: return "MESSAGE";
    case G_LOG_LEVEL_INFO: return "INFO";
    case G_LOG_LEVEL_DEBUG: return "DEBUG";

    default: return "UNKNOWN";
  }
}

static void log_handler_cb(const gchar *log_domain,
			   GLogLevelFlags log_level,
			   const gchar *message,
			   gpointer user_data)
{
  const gchar *log_level_str;
  bool debug_enabled = true;

  /* Ignore debug messages if disabled. */
  /* if (!debug_enabled && (log_level & G_LOG_LEVEL_DEBUG)) */
  /* { */
  /*   return; */
  /* } */
   
  log_level_str = log_level_to_string(log_level & G_LOG_LEVEL_MASK);
//  g_print("%s: %s: %s\n", log_domain, log_level_str, message);
  g_print("%s: %s\n", log_level_str, message);

  char temp[4096]; // Use snprintf to dynamically create buffer
  sprintf(temp, "%s: %s", log_level_str, message);
  dirty_log(temp);
  
  /* Use g_printerr() for warnings and g_print() otherwise. */
  /* if (flags <= G_LOG_LEVEL_WARNING) */
  /* { */
  /*   g_printerr("%s: %s: %s\n", log_domain, log_level_str, message); */
  /* } */
  /* else */
  /* { */
  /*   g_print("%s: %s: %s\n", log_domain, log_level_str, message); */
  /* } */
}

void init_logging()
{
  g_info("init_logging - ENTER");

  guint res = g_log_set_handler(G_LOG_DOMAIN,
				G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
				log_handler_cb, NULL);

  printf("\t ***res = %d\n", res);

  g_info("init_logging - EXIT");
}
