#include <gtk/gtk.h>
#include <gst/gst.h>

typedef struct _stream_info
{
  GstElement *pipeline;
  guint bus_watch_id;
} stream_info;

typedef struct _ui_info
{
  GtkWidget *file_label;
  GtkWidget *list_box;
} ui_info;

static stream_info *data;
static char *file_name;

enum {
  LIST_ITEM = 0,
  N_COLUMNS
};

static void gst_cleanup();
static void dump_info(stream_info *data);
static void gst_pause();

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

static void dump_info(stream_info *data)
{
  g_print("\n-----------------------------------------------------------------------------\n");
  g_print("pipline      = %p\n", data->pipeline);
  g_print("bus_watch_id = %d\n", data->bus_watch_id);
  g_print("-----------------------------------------------------------------------------\n");
}

static gboolean bus_call(
    GstBus *bus,
    GstMessage *msg,
    gpointer data)
{
  stream_info *stream_data = (stream_info *) data;

  switch (GST_MESSAGE_TYPE (msg)) 
  {
    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");

      dump_info(data);

      gst_cleanup(/*data*/);
      g_free(data);
      break;

    case GST_MESSAGE_STREAM_START:
      g_print("Start of stream!\n");
      break;

    case GST_MESSAGE_ERROR: 
      {
        gchar  *debug;
        GError *error;

        gst_message_parse_error(msg, &error, &debug);
        g_free(debug);

        g_printerr("Error: %s\n", error->message);
        g_error_free(error);

        break;
      }
    default:
      break;
  }

  return TRUE;
}

static void on_pad_added(
    GstElement *element,
    GstPad     *pad,
    gpointer    data)
{
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the vorbis-decoder sink pad */
  g_print("Dynamic pad created, linking demuxer/decoder\n");
  sinkpad = gst_element_get_static_pad(decoder, "sink");
  gst_pad_link(pad, sinkpad);
  gst_object_unref(sinkpad);
}

static void gst_stop()
{
  g_print("In gst_stop()\n");

  if (data != NULL)
  {
    gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
    gst_cleanup();
  }
}


static void gst_cleanup(/*stream_info *data*/)
{
  g_print("In gst_cleanup()\n");

  gst_element_set_state(data->pipeline, GST_STATE_NULL);
  g_print("Deleting pipeline\n");
  gst_object_unref(GST_OBJECT(data->pipeline));
  g_source_remove(data->bus_watch_id);

//  g_free(data);
  g_print("Setting data to NULL\n");
  data = NULL;
}

static void gst_pause()
{
  g_print("In gst_pause()\n");
  GstState cur_state;

  if (data != NULL)
  {
    gst_element_get_state(data->pipeline, &cur_state, NULL, 0); 
    if (cur_state == GST_STATE_PAUSED)
    {
      g_print("PAUSED\n");
    }
    if (cur_state == GST_STATE_PLAYING)
    {
      g_print("PLAYING\n");
    }

    if (cur_state == GST_STATE_PLAYING)
    {
      gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
    }
    if (cur_state == GST_STATE_PAUSED)
    {
      gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
    }
  }
}

static void gst_start(
    char *filename)
{
  GstElement *source, 
             *decoder, 
             *conv, 
             *sink;
  GstBus *bus;
//  stream_info *data;

  g_print("data = %p\n", data);

  if (data != NULL)
  {
    return;
  }

  g_print("In gst_start()\n");

  data = g_new(stream_info, 1);
    
  /* Create gstreamer elements */
  data->pipeline = gst_pipeline_new("audio-player");
  source = gst_element_factory_make("filesrc", "file-source");
  decoder = gst_element_factory_make("flump3dec", "fluendo-decoder");
  conv = gst_element_factory_make("audioconvert", "converter");
  sink = gst_element_factory_make("autoaudiosink", "audio-output");

  if (!data->pipeline || !source || !decoder || !conv || !sink) 
  {
    g_printerr("One element could not be created. Exiting.\n");
    return;
  }

  /* Set up the pipeline */
  /* we set the input filename to the source element */
  g_object_set(G_OBJECT (source), "location", filename, NULL);

  /* we add a message handler */
  bus = gst_pipeline_get_bus(GST_PIPELINE(data->pipeline));
  data->bus_watch_id = gst_bus_add_watch(bus, bus_call, data);
  gst_object_unref(bus);

  /* we add all elements into the pipeline */
  /* file-source | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many(GST_BIN(data->pipeline),
      source, decoder, conv, sink, NULL);
  printf("3.1\n");

  gst_element_link_many(source, decoder, conv, sink, NULL);

  // Set the pipeline to "playing" state
  g_print("Now playing: %s\n", filename);
  gst_element_set_state(data->pipeline, GST_STATE_PLAYING);

  g_print("Running...\n");
}

static void btn_clk(
    GtkWidget *widget,
    gpointer data)
{
  g_print("Button push\n");
  gst_start("piano2-Audacity1.2.5.mp3");
}

static void file_open_btn_click(
    GtkWidget *widget,
    gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  
  ui_info *ui = (ui_info *) data;
  g_print("file_label = %p\n", ui->file_label);
  gtk_label_set_text(ui->file_label, "Chickens");
  
  
  GtkWindow *parent = gtk_widget_get_parent_window(widget);
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
    //open_file(filename);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);

  g_print("file_open_btn_click\n");
}

void add_list_item(
    GtkWidget *listbox, 
    char *text)
{
  GtkWidget *item;
  GtkWidget *new_label;

  new_label = gtk_label_new(text);
  gtk_list_box_insert(GTK_LIST_BOX(listbox), new_label, -1);
}

void configure_callback(
    GtkWindow *window, 
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
   
   gtk_widget_set_size_request(grid, 10, 10);

//   g_print("In config event. x=%d y=%d width=%d height=%d data=%p\n", x, y, width, height, data);
   
   buf = g_string_new(NULL);   
   g_string_printf(buf, "%d, %d", x, y);
   
   //gtk_window_set_title(window, buf->str);
   
   g_string_free(buf, TRUE);
}

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

static void activate(
    GtkApplication* app,
    gpointer user_data)
{
  g_print("In activate\n");
  
  GtkWidget *window;
  GtkWidget *play_button;
  GtkWidget *stop_button;
  GtkWidget *pause_button;
  GtkWidget *file_open_btn;
  GtkWidget *button_box;
  GtkWidget *list_box;
  GtkWidget *grid_box;
  GtkWidget *status_bar;
  GtkWidget *grid;
  GtkWidget *tree_view;
//  GtkWidget *file_label;
  GtkTreeSelection *selection;
  ui_info *ui_data = malloc(sizeof(ui_info));
  ui_data->file_label = NULL;
  
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
  gtk_layout_set_size(grid, 800, 600);

  tree_view = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);

  init_list(tree_view);
  add_to_list(tree_view, "Aliens");
  add_to_list(tree_view, "Leon");
  add_to_list(tree_view, "The Verdict");
  add_to_list(tree_view, "North Face");
  add_to_list(tree_view, "Der Untergang");

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  
  // Label for filename
  ui_data->file_label = gtk_label_new("Filename goes here"); 
  g_print("file_label = %p\n", ui_data->file_label);
//  gtk_widget_set_name(ui_data->file_label, "file_label");
  
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
  gtk_grid_attach(GTK_GRID(grid), tree_view, 0, 0, 1, 1);

  gtk_widget_set_halign(list_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(list_box, GTK_ALIGN_FILL);
  gtk_widget_set_hexpand(list_box, TRUE);
  gtk_widget_set_vexpand(list_box, TRUE);

  play_button = gtk_button_new_with_label("Play");
  g_signal_connect(play_button, "clicked", G_CALLBACK(btn_clk), NULL);
  //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

  stop_button = gtk_button_new_with_label("Stop");
  g_signal_connect(stop_button, "clicked", G_CALLBACK(gst_stop), NULL);
  
  pause_button = gtk_button_new_with_label("Pause");
  g_signal_connect(pause_button, "clicked", G_CALLBACK(gst_pause), NULL);
  
  file_open_btn = gtk_button_new_with_label("Open...");
  g_signal_connect(file_open_btn, "clicked", G_CALLBACK(file_open_btn_click), ui_data);

  gtk_container_add(GTK_CONTAINER(button_box), play_button);
  gtk_container_add(GTK_CONTAINER(button_box), pause_button);
  gtk_container_add(GTK_CONTAINER(button_box), stop_button);
  gtk_container_add(GTK_CONTAINER(button_box), file_open_btn);

  gtk_grid_attach(GTK_GRID(grid), file_label, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_box, 0, 2, 1, 1);




/*
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(window), button_box);

  button = gtk_button_new_with_label("Test Play");
  g_signal_connect(button, "clicked", G_CALLBACK(btn_clk), NULL);
  //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

  file_open_btn = gtk_button_new_with_label("Open...");
  g_signal_connect(file_open_btn, "clicked", G_CALLBACK(file_open_btn_click), NULL);
  //g_signal_connect(button, "clicked", G_CALLBACK(btn_clk), NULL);


  // Status bar
  status_bar = gtk_statusbar_new();

  list_box = gtk_list_box_new();
  add_list_item(list_box, "Chickens");
  add_list_item(list_box, "Cows");
  add_list_item(list_box, "Goats");


  gtk_container_add(GTK_CONTAINER (button_box), button);
  gtk_container_add(GTK_CONTAINER (button_box), file_open_btn);
  gtk_container_add(GTK_CONTAINER (button_box), list_box);
*/

  gtk_widget_show_all(window);
}

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

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  
  g_print("activate signal connected... about to run\n");

  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}

/*
=======
#include <gtk/gtk.h>
#include <gst/gst.h>

typedef struct _stream_info
{
  GstElement *pipeline;
  guint bus_watch_id;
} stream_info;

typedef struct _ui_info
{
  GtkWidget *file_label;
  GtkWidget *list_box;
} ui_info;

static stream_info *data;
static char *file_name;

enum {
  LIST_ITEM = 0,
  N_COLUMNS
};

static void gst_cleanup();
static void dump_info(stream_info *data);
static void gst_pause();

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

static void dump_info(stream_info *data)
{
  g_print("\n-----------------------------------------------------------------------------\n");
  g_print("pipline      = %p\n", data->pipeline);
  g_print("bus_watch_id = %d\n", data->bus_watch_id);
  g_print("-----------------------------------------------------------------------------\n");
}

static gboolean bus_call(
    GstBus *bus,
    GstMessage *msg,
    gpointer data)
{
  stream_info *stream_data = (stream_info *) data;

  switch (GST_MESSAGE_TYPE (msg)) 
  {
    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");

      dump_info(data);

      gst_cleanup(/*data*/);
      g_free(data);
      break;

    case GST_MESSAGE_STREAM_START:
      g_print("Start of stream!\n");
      break;

    case GST_MESSAGE_ERROR: 
      {
        gchar  *debug;
        GError *error;

        gst_message_parse_error(msg, &error, &debug);
        g_free(debug);

        g_printerr("Error: %s\n", error->message);
        g_error_free(error);

        break;
      }
    default:
      break;
  }

  return TRUE;
}

static void on_pad_added(
    GstElement *element,
    GstPad     *pad,
    gpointer    data)
{
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the vorbis-decoder sink pad */
  g_print("Dynamic pad created, linking demuxer/decoder\n");
  sinkpad = gst_element_get_static_pad(decoder, "sink");
  gst_pad_link(pad, sinkpad);
  gst_object_unref(sinkpad);
}

static void gst_stop()
{
  g_print("In gst_stop()\n");

  if (data != NULL)
  {
    gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
    gst_cleanup();
  }
}


static void gst_cleanup(/*stream_info *data*/)
{
  g_print("In gst_cleanup()\n");

  gst_element_set_state(data->pipeline, GST_STATE_NULL);
  g_print("Deleting pipeline\n");
  gst_object_unref(GST_OBJECT(data->pipeline));
  g_source_remove(data->bus_watch_id);

//  g_free(data);
  g_print("Setting data to NULL\n");
  data = NULL;
}

static void gst_pause()
{
  g_print("In gst_pause()\n");
  GstState cur_state;

  if (data != NULL)
  {
    gst_element_get_state(data->pipeline, &cur_state, NULL, 0); 
    if (cur_state == GST_STATE_PAUSED)
    {
      g_print("PAUSED\n");
    }
    if (cur_state == GST_STATE_PLAYING)
    {
      g_print("PLAYING\n");
    }

    if (cur_state == GST_STATE_PLAYING)
    {
      gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
    }
    if (cur_state == GST_STATE_PAUSED)
    {
      gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
    }
  }
}

static void gst_start(
    char *filename)
{
  GstElement *source, 
             *decoder, 
             *conv, 
             *sink;
  GstBus *bus;
//  stream_info *data;

  g_print("data = %p\n", data);

  if (data != NULL)
  {
    return;
  }

  g_print("In gst_start()\n");

  data = g_new(stream_info, 1);
    
  /* Create gstreamer elements */
  data->pipeline = gst_pipeline_new("audio-player");
  source = gst_element_factory_make("filesrc", "file-source");
  decoder = gst_element_factory_make("flump3dec", "fluendo-decoder");
  conv = gst_element_factory_make("audioconvert", "converter");
  sink = gst_element_factory_make("autoaudiosink", "audio-output");

  if (!data->pipeline || !source || !decoder || !conv || !sink) 
  {
    g_printerr("One element could not be created. Exiting.\n");
    return;
  }

  /* Set up the pipeline */
  /* we set the input filename to the source element */
  g_object_set(G_OBJECT (source), "location", filename, NULL);

  /* we add a message handler */
  bus = gst_pipeline_get_bus(GST_PIPELINE(data->pipeline));
  data->bus_watch_id = gst_bus_add_watch(bus, bus_call, data);
  gst_object_unref(bus);

  /* we add all elements into the pipeline */
  /* file-source | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many(GST_BIN(data->pipeline),
      source, decoder, conv, sink, NULL);
  printf("3.1\n");

  gst_element_link_many(source, decoder, conv, sink, NULL);

  // Set the pipeline to "playing" state
  g_print("Now playing: %s\n", filename);
  gst_element_set_state(data->pipeline, GST_STATE_PLAYING);

  g_print("Running...\n");
}

static void btn_clk(
    GtkWidget *widget,
    gpointer data)
{
  g_print("Button push\n");
  gst_start("piano2-Audacity1.2.5.mp3");
}

static void file_open_btn_click(
    GtkWidget *widget,
    gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  
  ui_info *ui = (ui_info *) data;
  g_print("file_label = %p\n", ui->file_label);
  gtk_label_set_text(ui->file_label, "Chickens");
  
  
  GtkWindow *parent = gtk_widget_get_parent_window(widget);
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
    //open_file(filename);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);

  g_print("file_open_btn_click\n");
}

void add_list_item(
    GtkWidget *listbox, 
    char *text)
{
  GtkWidget *item;
  GtkWidget *new_label;

  new_label = gtk_label_new(text);
  gtk_list_box_insert(GTK_LIST_BOX(listbox), new_label, -1);
}

void configure_callback(
    GtkWindow *window, 
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
   
   gtk_widget_set_size_request(grid, 10, 10);

//   g_print("In config event. x=%d y=%d width=%d height=%d data=%p\n", x, y, width, height, data);
   
   buf = g_string_new(NULL);   
   g_string_printf(buf, "%d, %d", x, y);
   
   //gtk_window_set_title(window, buf->str);
   
   g_string_free(buf, TRUE);
}

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

static void activate(
    GtkApplication* app,
    gpointer user_data)
{
  g_print("In activate\n");
  
  GtkWidget *window;
  GtkWidget *play_button;
  GtkWidget *stop_button;
  GtkWidget *pause_button;
  GtkWidget *file_open_btn;
  GtkWidget *button_box;
  GtkWidget *list_box;
  GtkWidget *grid_box;
  GtkWidget *status_bar;
  GtkWidget *grid;
  GtkWidget *tree_view;
//  GtkWidget *file_label;
  GtkTreeSelection *selection;
  ui_info *ui_data = malloc(sizeof(ui_info));
  ui_data->file_label = NULL;
  
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
  gtk_layout_set_size(grid, 800, 600);

  tree_view = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);

  init_list(tree_view);
  add_to_list(tree_view, "Aliens");
  add_to_list(tree_view, "Leon");
  add_to_list(tree_view, "The Verdict");
  add_to_list(tree_view, "North Face");
  add_to_list(tree_view, "Der Untergang");

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  
  // Label for filename
  ui_data->file_label = gtk_label_new("Filename goes here"); 
  g_print("file_label = %p\n", ui_data->file_label);
//  gtk_widget_set_name(ui_data->file_label, "file_label");
  
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
  gtk_grid_attach(GTK_GRID(grid), tree_view, 0, 0, 1, 1);

  gtk_widget_set_halign(list_box, GTK_ALIGN_FILL);
  gtk_widget_set_valign(list_box, GTK_ALIGN_FILL);
  gtk_widget_set_hexpand(list_box, TRUE);
  gtk_widget_set_vexpand(list_box, TRUE);

  play_button = gtk_button_new_with_label("Play");
  g_signal_connect(play_button, "clicked", G_CALLBACK(btn_clk), NULL);
  //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

  stop_button = gtk_button_new_with_label("Stop");
  g_signal_connect(stop_button, "clicked", G_CALLBACK(gst_stop), NULL);
  
  pause_button = gtk_button_new_with_label("Pause");
  g_signal_connect(pause_button, "clicked", G_CALLBACK(gst_pause), NULL);
  
  file_open_btn = gtk_button_new_with_label("Open...");
  g_signal_connect(file_open_btn, "clicked", G_CALLBACK(file_open_btn_click), ui_data);

  gtk_container_add(GTK_CONTAINER(button_box), play_button);
  gtk_container_add(GTK_CONTAINER(button_box), pause_button);
  gtk_container_add(GTK_CONTAINER(button_box), stop_button);
  gtk_container_add(GTK_CONTAINER(button_box), file_open_btn);

  //gtk_grid_attach(GTK_GRID(grid), file_label, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_box, 0, 2, 1, 1);




/*
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(window), button_box);

  button = gtk_button_new_with_label("Test Play");
  g_signal_connect(button, "clicked", G_CALLBACK(btn_clk), NULL);
  //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

  file_open_btn = gtk_button_new_with_label("Open...");
  g_signal_connect(file_open_btn, "clicked", G_CALLBACK(file_open_btn_click), NULL);
  //g_signal_connect(button, "clicked", G_CALLBACK(btn_clk), NULL);


  // Status bar
  status_bar = gtk_statusbar_new();

  list_box = gtk_list_box_new();
  add_list_item(list_box, "Chickens");
  add_list_item(list_box, "Cows");
  add_list_item(list_box, "Goats");


  gtk_container_add(GTK_CONTAINER (button_box), button);
  gtk_container_add(GTK_CONTAINER (button_box), file_open_btn);
  gtk_container_add(GTK_CONTAINER (button_box), list_box);
*/

  gtk_widget_show_all(window);
}

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

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  
  g_print("activate signal connected... about to run\n");

  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
>>>>>>> 870799c24881d846999a8ca619e3324ad052adcd
*/
