#include <gtk/gtk.h>
#include <gst/gst.h>

#include "gui.h"

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  g_print("%s started.\n", argv[0]);

  // Initialisation
  gst_init(&argc, &argv);

  g_print("GST is init\n");

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);

  g_print("Application created\n");

  g_signal_connect(app, "activate", G_CALLBACK(mainwindow_activate), NULL);
  
  g_print("activate signal connected... about to run\n");

  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
