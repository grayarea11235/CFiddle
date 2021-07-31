#include <gtk/gtk.h>
#include <gst/gst.h>
#include <assert.h>
#include <unistd.h>

#include <apr_general.h> 

#include "gui.h"
#include "config.h"
#include "datastore.h"
#include "logging.h"

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;
  datastore_t *ds;

  LOG_DEBUG("ENTER");
  
  init_logging();

  LOG_INFO("Starting main...");
  
  const gchar *home_dir = g_getenv("HOME");

  ds = open_datastore("/home/cpd/testdb.db");
  
  // Initialisation
  gst_init(&argc, &argv);
  apr_initialize();
  
  LOG_DEBUG("%s started.", argv[0]);
  assert(scan_directory("/media/cpd/3634-6261/Music/1988 - Green/") == 0);
  
  gst_meta_info("file:///media/cpd/3634-6261/Music/(2000) Kid A/01 Radiohead - Everything In Its Right Place.mp3");
    
  init_config();
  
  LOG_DEBUG("GST is init");

  app = gtk_application_new("org.gtk.myplayer", G_APPLICATION_FLAGS_NONE);

  LOG_DEBUG("Application created");

  g_signal_connect(app, "activate", G_CALLBACK(mainwindow_activate), NULL);
  
  LOG_DEBUG("activate signal connected... about to run\n");

  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  apr_terminate();

  close_datastore(ds);

  //gst_player_deinit(gst_info_t *info);
  
  LOG_DEBUG("EXIT");
  
  return status;
}
