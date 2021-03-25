#include "gst.h"


// gst.c
static void dump_info(stream_info *data)
{
  g_print("\n-----------------------------------------------------------------------------\n");
  g_print("pipline      = %p\n", data->pipeline);
  g_print("bus_watch_id = %d\n", data->bus_watch_id);
  g_print("-----------------------------------------------------------------------------\n");
}


// gst.c
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

void gst_stop()
{
  g_print("In gst_stop()\n");

  if (data != NULL)
  {
    gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
    gst_cleanup();
  }
}


void gst_start(char *filename)
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


void gst_cleanup(/*stream_info *data*/)
{
  g_print("In gst_cleanup()\n");

  if (data != NULL)
  {
    gst_element_set_state(data->pipeline, GST_STATE_NULL);
    g_print("Deleting pipeline\n");
    gst_object_unref(GST_OBJECT(data->pipeline));
    g_source_remove(data->bus_watch_id);
    
    g_print("Setting data to NULL\n");
    data = NULL;
  }
}

void gst_pause()
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

