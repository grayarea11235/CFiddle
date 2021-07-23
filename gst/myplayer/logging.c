#include <glib.h>
#include <stdbool.h>
#include <stdio.h>

#include "logging.h"

void dirty_log(const char *msg)
{
  FILE *f = fopen("/home/cpd/dirty_log.txt", "wt");

  fprintf(f, "%s", msg);
   
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

  dirty_log("In log_handler_cb\n");
  printf("In Log handler\n");
  
  /* Ignore debug messages if disabled. */
  /* if (!debug_enabled && (log_level & G_LOG_LEVEL_DEBUG)) */
  /* { */
  /*   return; */
  /* } */
   
  log_level_str = log_level_to_string(log_level & G_LOG_LEVEL_MASK);
  g_print("%s: %s: %s\n", log_domain, log_level_str, message);
  
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
  
  
  g_log_set_handler("log-domain",
		    G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
		    log_handler_cb, NULL);
}
