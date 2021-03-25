#include <gtk/gtk.h>
#include <gst/gst.h>
#include <math.h>

#include "gui.h"
#include "gst.h"

enum {
  LIST_ITEM = 0,
  N_COLUMNS
};


//#ifndef M_LN10
//#define M_LN10 (log(10.0))
//#endif


// Should be in header or not here
static char *file_name;

void add_list_item(GtkWidget *listbox, 
		   char *text)
{
  GtkWidget *item;
  GtkWidget *new_label;

  new_label = gtk_label_new(text);
  gtk_list_box_insert(GTK_LIST_BOX(listbox), new_label, -1);
}


static void add_to_list(
  GtkWidget *list, 
  const gchar *str) 
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
			 (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

// Init the track list
void init_list(GtkWidget *list) 
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("List Items",
						    renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
			  GTK_TREE_MODEL(store));

  g_object_unref(store);
}

void play_btn_clk(GtkWidget *widget,
		  gpointer data)
{
  g_print("Button push\n");

  ui_info *ui = (ui_info *) data;
  //g_print("file_label = %p\n", ui->file_label);
  
  // Get the file name from the GtkLabel
  const gchar *name = gtk_label_get_text(GTK_LABEL(ui->file_label));  
  
  // TODO : Check if the file exists
  gst_start((char *)name);
}

void configure_callback(GtkWindow *window, 
			GdkEvent *event, 
			gpointer data) 
{
  int x, y;
  int width, height;
  GString *buf;
  GtkGrid *grid = GTK_GRID(data);
  
  x = event->configure.x;
  y = event->configure.y;
  width = event->configure.width;
  height = event->configure.height;
  
  gtk_widget_set_size_request(GTK_WIDGET(grid), 10, 10);
  
//   g_print("In config event. x=%d y=%d width=%d height=%d data=%p\n", x, y, width, height, data);
  
  buf = g_string_new(NULL);   
  g_string_printf(buf, "%d, %d", x, y);
  
  //gtk_window_set_title(window, buf->str);
  
  g_string_free(buf, TRUE);
}


static void file_open_btn_click(GtkWidget *widget,
				gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  
  ui_info *ui = (ui_info *) data;

  g_print("file_label = %p\n", ui->file_label);
  
  GtkWindow *parent = GTK_WINDOW(gtk_widget_get_parent_window(widget));
  dialog = gtk_file_chooser_dialog_new ("Open File",
					parent,
					action,
					"_Cancel",
					GTK_RESPONSE_CANCEL,
					"_Open",
					GTK_RESPONSE_ACCEPT,
					NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    g_print("filename = %s\n", filename);
    gtk_label_set_text(GTK_LABEL(ui->file_label), filename);
    //open_file(filename);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);

  g_print("file_open_btn_click\n");
}

static void volume_scale_changed(GtkWidget *widget, GstElement *volume)
{
  gdouble value;
  gdouble level;

  value = gtk_range_get_value(GTK_RANGE (widget));
  level = exp(value / 20.0 * M_LN10);
  g_print("Value: %f dB, level: %f\n", value, level);
  g_object_set(volume, "volume", level, NULL);
}


void mainwindow_activate(GtkApplication* app,
			 gpointer user_data)
{
  g_print("In activate\n");
  
  GtkWidget *window;
//  GtkWidget *file_open_btn;
  GtkWidget *button_box;
  GtkWidget *list_box;
  GtkWidget *grid_box;
  GtkWidget *status_bar;
  GtkWidget *grid;
  GtkWidget *tree_view;
  GtkTreeSelection *selection;
  ui_info *ui_info_cb = malloc(sizeof(ui_info));
  ui_info_cb->file_label = NULL;
  
  window = gtk_application_window_new(app);

  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  grid = gtk_grid_new();

  gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);
  g_signal_connect(G_OBJECT(window), "configure-event",
		   G_CALLBACK(configure_callback), grid);

  gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
  gtk_widget_set_valign(grid, GTK_ALIGN_FILL);
  gtk_widget_set_hexpand(grid, TRUE);
  gtk_widget_set_vexpand(grid, TRUE);
  //gtk_container_set_resize_mode(GTK_CONTAINER(grid), GTK_RESIZE_PARENT);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_layout_set_size(GTK_LAYOUT(grid), 800, 600);

  tree_view = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);

  init_list(tree_view);
  add_to_list(tree_view, "Aliens");
  add_to_list(tree_view, "Leon");
  add_to_list(tree_view, "The Verdict");
  add_to_list(tree_view, "North Face");
//  add_to_list(tree_view, "Der Untergang");

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  
  // Label for filename
  ui_info_cb->file_label = gtk_label_new("Filename goes here"); 
  g_print("file_label = %p\n", ui_info_cb->file_label);
//  gtk_widget_set_name(ui_info_cb->file_label, "file_label");

  // Set up the vol scale
  GtkWidget *volume_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -90.0, 10.0, 0.2);
  g_signal_connect(volume_scale, "value-changed",
		   G_CALLBACK(volume_scale_changed), volume_scale);
  
  // Make the button box
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

  gtk_widget_set_halign(button_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(button_box, GTK_ALIGN_FILL);
  gtk_widget_set_hexpand(button_box, TRUE);
  gtk_widget_set_vexpand(button_box, TRUE);

  list_box = gtk_list_box_new();
  add_list_item(list_box, "Track 1");
  add_list_item(list_box, "Track 2");
  add_list_item(list_box, "Track 3");

  gtk_widget_set_halign(list_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(list_box, GTK_ALIGN_FILL);
  gtk_widget_set_hexpand(list_box, TRUE);
  gtk_widget_set_vexpand(list_box, TRUE);

  ui_info_cb->play_button = gtk_button_new_with_label("Play");
  g_signal_connect(ui_info_cb->play_button, "clicked", G_CALLBACK(play_btn_clk), ui_info_cb);
  //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

  ui_info_cb->stop_button = gtk_button_new_with_label("Stop");
  g_signal_connect(ui_info_cb->stop_button, "clicked", G_CALLBACK(gst_stop), ui_info_cb);
  
  ui_info_cb->pause_button = gtk_button_new_with_label("Pause");
  g_signal_connect(ui_info_cb->pause_button, "clicked", G_CALLBACK(gst_pause), ui_info_cb);
  
  ui_info_cb->file_open_btn = gtk_button_new_with_label("Open...");
  g_signal_connect(ui_info_cb->file_open_btn, "clicked", G_CALLBACK(file_open_btn_click), ui_info_cb);

  gtk_container_add(GTK_CONTAINER(button_box), ui_info_cb->play_button);
  gtk_container_add(GTK_CONTAINER(button_box), ui_info_cb->pause_button);
  gtk_container_add(GTK_CONTAINER(button_box), ui_info_cb->stop_button);
  gtk_container_add(GTK_CONTAINER(button_box), ui_info_cb->file_open_btn);

  gtk_grid_attach(GTK_GRID(grid), tree_view, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), ui_info_cb->file_label, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), volume_scale, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_box, 0, 3, 1, 1);
  
  gtk_widget_show_all(window);
}
