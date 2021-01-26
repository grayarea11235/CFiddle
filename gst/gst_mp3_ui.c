#include <gtk/gtk.h>
#include <gst/gst.h>

static gboolean bus_call (GstBus     *bus,
    GstMessage *msg,
    gpointer    data)
{
  GMainLoop *loop = (GMainLoop *) data;
  

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      //g_main_loop_quit(loop);
      break;

    case GST_MESSAGE_STREAM_START:
      g_print("Start of stream!\n");
      break;

    case GST_MESSAGE_ERROR: {
                              gchar  *debug;
                              GError *error;

                              gst_message_parse_error (msg, &error, &debug);
                              g_free (debug);

                              g_printerr ("Error: %s\n", error->message);
                              g_error_free (error);

                              //g_main_loop_quit (loop);
                              break;
                            }
    default:
                            break;
  }

  return TRUE;
}

  static void
on_pad_added (GstElement *element,
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

static void gst_cleanup()
{
  //gst_element_set_state(pipeline, GST_STATE_NULL);
  //g_print("Deleting pipeline\n");
  //gst_object_unref(GST_OBJECT (pipeline));
  //g_source_remove(bus_watch_id);
  //g_main_loop_unref(loop);
}

static void gst_start(char *filename)
{
  GMainLoop *loop;
  GstElement *pipeline, 
             *source, 
             *decoder, 
             *conv, 
             *sink;
  GstBus *bus;
  guint bus_watch_id;


  printf("1\n");

  /* Create gstreamer elements */
  pipeline = gst_pipeline_new("audio-player");
  source = gst_element_factory_make("filesrc", "file-source");
  decoder = gst_element_factory_make("flump3dec", "fluendo-decoder");
  conv = gst_element_factory_make("audioconvert", "converter");
  sink = gst_element_factory_make("autoaudiosink", "audio-output");

  if (!pipeline || !source || !decoder || !conv || !sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    printf("decoder = %p\n", decoder);
    return -1;
  }
  printf("2\n");

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set(G_OBJECT (source), "location", filename, NULL);

  /* we add a message handler */
  bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  gst_object_unref(bus);

  printf("3\n");

  /* we add all elements into the pipeline */
  /* file-source | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many(GST_BIN (pipeline),
      source, decoder, conv, sink, NULL);
  printf("3.1\n");

  /* we link the elements together */
  /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
  //gst_element_link (source, demuxer);
  gst_element_link_many(source, decoder, conv, sink, NULL);

  printf("3.2\n");

  //g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), decoder);

  printf("4\n");

  // Set the pipeline to "playing" state
  g_print ("Now playing: %s\n", filename);
  gst_element_set_state (pipeline, GST_STATE_PLAYING);


  /* Iterate */
  g_print ("Running...\n");
}

static void btn_clk(GtkWidget *widget,
    gpointer data)
{
  g_print ("Button push\n");
}

static void file_open_btn_click(GtkWidget *widget,
    gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  GtkWidget *parent = gtk_widget_get_toplevel(widget);
  dialog = gtk_file_chooser_dialog_new ("Open File",
      GTK_WIDGET(parent),
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

  gtk_widget_destroy (dialog);


  g_print("file_open_btn_click\n");
}

void add_list_item(GtkWidget *listbox, char *text)
{
  GtkWidget *item;
  GtkWidget *new_label;

  new_label = gtk_label_new(text);
  gtk_list_box_insert(GTK_LIST_BOX(listbox), new_label, -1);
}


static void activate (GtkApplication* app,
    gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *file_open_btn;
  GtkWidget *button_box;
  GtkWidget *list_box;

  window = gtk_application_window_new (app);
  gtk_window_set_title(GTK_WINDOW (window), "Window");
  gtk_window_set_default_size(GTK_WINDOW (window), 800, 600);

  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(window), button_box);

  button = gtk_button_new_with_label("Test Play");
  g_signal_connect(button, "clicked", G_CALLBACK(btn_clk), NULL);
  //g_signal_connect_swapped(button, "clicked", G_CALLBACK (gtk_widget_destroy), window);

  file_open_btn = gtk_button_new_with_label("Open...");
  g_signal_connect(file_open_btn, "clicked", G_CALLBACK(file_open_btn_click), NULL);
  //g_signal_connect(button, "clicked", G_CALLBACK(btn_clk), NULL);

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

  /* Initialisation */
  gst_init (&argc, &argv);

  gst_start("piano2-Audacity1.2.5.mp3");
//  gst_start("file_example_MP3_5MG.mp3");

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
