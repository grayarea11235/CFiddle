#ifndef GUI_H
#define GUI_H

#include "gst.h"

typedef struct _ui_info
{
  GtkWidget *file_label;
  GtkWidget *list_box;

  GtkWidget *window;
  GtkWidget *play_button;
  GtkWidget *stop_button;
  GtkWidget *pause_button;
  GtkWidget *file_open_btn;

  gst_info_t *gst_info;

  char *current_file;
} ui_info;


extern void mainwindow_activate(GtkApplication* app,
				gpointer user_data);


void gst_cleanup();


#endif
