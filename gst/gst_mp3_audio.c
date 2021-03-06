#include <gst/gst.h>
#include <glib.h>

static gboolean bus_call(
    GstBus     *bus,
    GstMessage *msg,
    gpointer    data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) 
  {
    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
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

      g_main_loop_quit (loop);
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

int main(
    int   argc,
    char *argv[])
{
  GMainLoop *loop;
  GstElement *pipeline, 
             *source, 
             *decoder, 
             *conv, 
             *sink;
  GstBus *bus;
  guint bus_watch_id;

  /* Initialisation */
  gst_init (&argc, &argv);

  loop = g_main_loop_new(NULL, FALSE);

  /* Check input arguments */
  if (argc != 2) 
  {
    g_printerr("Usage: %s <mp3 filename>\n", argv[0]);
    return -1;
  }

g_print("1\n");
  /* Create gstreamer elements */
  pipeline = gst_pipeline_new("audio-player");
  source = gst_element_factory_make("filesrc", "file-source");
  decoder = gst_element_factory_make("flump3dec", "fluendo-decoder");
  conv = gst_element_factory_make("audioconvert", "converter");
  sink = gst_element_factory_make("autoaudiosink", "audio-output");

  if (!pipeline || !source || !decoder || !conv || !sink) {
    g_printerr("One element could not be created. Exiting.\n");
    g_print("decoder = %p\n", decoder);

    return -1;
  }
g_print("2\n");

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set(G_OBJECT (source), "location", argv[1], NULL);

  /* we add a message handler */
  bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  gst_object_unref (bus);

g_print("3\n");

  /* we add all elements into the pipeline */
  /* file-source | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many(GST_BIN (pipeline),
      source, decoder, conv, sink, NULL);
g_print("3.1\n");

  /* we link the elements together */
  /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
  //gst_element_link (source, demuxer);
  gst_element_link_many(source, decoder, conv, sink, NULL);

g_print("3.2\n");

  //g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), decoder);

g_print("4\n");

  /* Set the pipeline to "playing" state*/
  g_print("Now playing: %s\n", argv[1]);
  gst_element_set_state(pipeline, GST_STATE_PLAYING);


  /* Iterate */
  g_print("Running...\n");
  g_main_loop_run(loop);


  /* Out of the main loop, clean up nicely */
  g_print("Returned, stopping playback\n");
  gst_element_set_state(pipeline, GST_STATE_NULL);

  g_print("Deleting pipeline\n");
  gst_object_unref(GST_OBJECT (pipeline));
  g_source_remove(bus_watch_id);
  g_main_loop_unref(loop);

  return 0;
}

