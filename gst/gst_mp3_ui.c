#include <gtk/gtk.h>
#include <gst/gst.h>


typedef struct _stream_info
{
  GstElement *pipeline;
  guint bus_watch_id;
} stream_info;

static void gst_cleanup();

static void dump_info(stream_info *data)
{
  g_print("\n-----------------------------------------------------------------------------\n");
  g_print("pipline      = %p\n", data->pipeline);
  g_print("bus_watch_id = %d\n", data->bus_watch_id);
  g_print("-----------------------------------------------------------------------------\n");
}

static gboolean bus_call(GstBus *bus,
    GstMessage *msg,
    gpointer data)
{
  stream_info *stream_data = (stream_info *) data;

  switch (GST_MESSAGE_TYPE (msg)) 
  {
    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");

      dump_info(data);

      gst_cleanup(data);
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

static void on_pad_added(GstElement *element,
    GstPad     *pad,
    gpointer    data)
{
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the vorbis-decoder sink pad */
  g_print ("Dynamic pad created, linking demuxer/decoder\n");
  sinkpad = gst_element_get_static_pad (decoder, "sink");
  gst_pad_link (pad, sinkpad);
  gst_object_unref (sinkpad);
}

static void gst_cleanup(stream_info *data)
{
  g_print("In gst_cleanup()\n");


  gst_element_set_state(data->pipeline, GST_STATE_NULL);
  g_print("Deleting pipeline\n");
  gst_object_unref(GST_OBJECT(data->pipeline));
  g_source_remove(data->bus_watch_id);
}

static void gst_start(char *filename)
{
  GstElement *source, 
             *decoder, 
             *conv, 
             *sink;
  GstBus *bus;
  stream_info *data;

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

static void btn_clk(GtkWidget *widget,
    gpointer data)
{
  g_print ("Button push\n");
  gst_start("piano2-Audacity1.2.5.mp3");
}

static void file_open_btn_click(GtkWidget *widget,
    gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  GtkWindow *parent = gtk_widget_get_parent_window(widget);
  dialog = gtk_file_chooser_dialog_new ("Open File",
      parent,
      action,
      "_Cancel",
      GTK_RESPONSE_CANCEL,
      "_Open",
      GTK_RESPONSE_ACCEPT,
      NULL);

  res = gtk_dialog_run(GTK_DIALOG (dialog));
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

void add_list_item(GtkWidget *listbox, char *text)
{
  GtkWidget *item;
  GtkWidget *new_label;

  new_label = gtk_label_new(text);
  gtk_list_box_insert(GTK_LIST_BOX(listbox), new_label, -1);
}


static void activate(GtkApplication* app,
    gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *file_open_btn;
  GtkWidget *button_box;
  GtkWidget *list_box;
  GtkWidget *grid_box;
  GtkWidget *status_bar;

  g_print("Enter activate\n");
  
  grid_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window), "Window");
  gtk_window_set_default_size(GTK_WINDOW (window), 800, 600);

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
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
