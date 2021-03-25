#ifndef GUI_H
#define GUI_H

typedef struct _ui_info
{
  GtkWidget *file_label;
  GtkWidget *list_box;

  // Not im[plemented
  GtkWidget *window;
  GtkWidget *play_button;
  GtkWidget *stop_button;
  GtkWidget *pause_button;
  GtkWidget *file_open_btn;
  
//  stream_info stream_data;
} ui_info;


extern void mainwindow_activate(GtkApplication* app,
				gpointer user_data);


void gst_cleanup();


#endif
