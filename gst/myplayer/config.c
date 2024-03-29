#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <assert.h>
#include <stdlib.h>
#include <apr_file_info.h>

#include "config.h"
#include "logging.h"

config_info_t *init_config()
{
  LOG_INFO("ENTER");

  struct stat st = { 0 };
  config_info_t *ret = malloc(sizeof(config_info_t));
  
  apr_pool_t *mp;
  apr_status_t rv;

  apr_pool_create(&mp, NULL);
			   
  const char *home_dir = getenv("HOME");
  
  LOG_INFO("In init_config... home dir is %s", home_dir);

  char *path_test;

  if ((rv = apr_filepath_merge(&path_test, home_dir, ".config", 0, mp)) == APR_SUCCESS)
  {
    LOG_INFO("path_test = %s\n", path_test);
  }
  
  if (stat(path_test, &st) == -1)
  {
    LOG_INFO("Creating config dir\n");
    assert(mkdir("~/.config", 0700) != -1);
  }

  apr_pool_destroy(mp);
  
  if (stat("/home/cpd/.config/mply", &st) == -1)
  {
    LOG_INFO("Creating mply dir\n");
    assert(mkdir("/home/cpd/.config/mply", 0700) != -1);
  }

  LOG_INFO("EXIT");
  return ret;
}
