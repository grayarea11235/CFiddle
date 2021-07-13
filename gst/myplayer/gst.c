#include "gst.h"

// gst.c
static void dump_info(stream_info *data)
{
  g_print("\n-----------------------------------------------------------------------------\n");
  g_print("pipline      = %p\n", data->pipeline);
  g_print("bus_watch_id = %d\n", data->bus_watch_id);
  g_print("-----------------------------------------------------------------------------\n");
}

// -----------------------------------------------------------------------------------------
// Function : cb_print_position
// -----------------------------------------------------------------------------------------
gboolean cb_print_position(GstElement *pipeline)
{
  gint64 pos, len;
  
  if (gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos)
      && gst_element_query_duration(pipeline, GST_FORMAT_TIME, &len))
  {
    g_print("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
	    GST_TIME_ARGS (pos), GST_TIME_ARGS (len));
  }

  // call me again
  return TRUE;
}
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
static gboolean bus_call(GstBus *bus,
			 GstMessage *msg,
			 gpointer data)
{
  stream_info *stream_data = (stream_info *) data;

  switch (GST_MESSAGE_TYPE(msg)) 
  {
    case GST_MESSAGE_EOS:
      g_print("End of stream\n");
      dump_info(data);
      gst_player_stop(data);
      
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
// -----------------------------------------------------------------------------------------

// gst_player --------------------------------------------------------------

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
static void gst_player_dump_info(gst_info_t *data)
{
  g_print("\n-----------------------------------------------------------------------------\n");
  g_print("pipline      = %p\n", data->pipeline);
  g_print("volume       = %p\n", data->volume);
  g_print("source       = %p\n", data->source);
  g_print("bus_watch_id = %d\n", data->bus_watch_id);
  g_print("-----------------------------------------------------------------------------\n");
}
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
void gst_player_stop(gst_info_t *info)
{
  // How do I reset?
  gst_element_set_state(info->pipeline, GST_STATE_PAUSED);
  gst_element_set_state(info->pipeline, GST_STATE_NULL);
}
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
void gst_player_pause(gst_info_t *info)
{
  g_print("In gst_player_pause\n");
  g_print("pipeline = %p\n", info->pipeline);
  gst_player_dump_info(info);
    
  gst_element_set_state(info->pipeline, GST_STATE_PAUSED);
}
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
void gst_player_play(gst_info_t *info, gchar *filename)
{
  // Set the filename - Should be in play
  g_object_set(G_OBJECT(info->source), "location", filename, NULL);

  g_print("Now playing: %s\n", filename);
  g_print("pipeline = %p\n", info->pipeline);
  gst_player_dump_info(info);

  gst_element_set_state(info->pipeline, GST_STATE_PLAYING);
  g_print("Started\n");
}

gst_info_t *gst_player_startup()
{
  GstElement *source, 
    *pipeline, 
    *decoder, 
    *conv, 
    *sink,
    *volume;
  GstBus *bus;
  guint bus_watch_id;

  gst_info_t *return_data = malloc(sizeof(stream_info));

  g_print("data = %p\n", data);

  g_print("In gst_startup()\n");
  
  // Look into autosource
  // https://gstreamer.freedesktop.org/documentation/autodetect/index.html?gi-language=c
  
  // Create elements
  pipeline = gst_pipeline_new("audio-player");
  source = gst_element_factory_make("filesrc", "file-source");
  decoder = gst_element_factory_make("flump3dec", "fluendo-decoder");
  conv = gst_element_factory_make("audioconvert", "converter");
  volume = gst_element_factory_make("volume", "volume-name");
  sink = gst_element_factory_make("autoaudiosink", "audio-output");

  if (!pipeline || !source || !decoder || !conv || !volume || !sink) 
  {
    g_printerr("One element could not be created. Exiting.\n");
    return NULL;
  }

  // Set the filename - Should be in play
//  g_object_set(G_OBJECT(source), "location", filename, NULL);
  
  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

//  bus_watch_id = gst_bus_add_watch(bus, bus_call, data);
  bus_watch_id = gst_bus_add_watch(bus, bus_call, return_data);
  gst_object_unref(bus);

  gst_bin_add_many(GST_BIN(pipeline),
		   source, decoder, volume, conv, sink, NULL);

  // Link em up
  gst_element_link_many(source, decoder, volume, conv, sink, NULL);

  return_data->pipeline = pipeline;
  return_data->volume = volume;
  return_data->source = source;
  return_data->bus_watch_id = bus_watch_id; // Why??

  gst_player_dump_info(return_data);

  g_print("About to set timeout\n");
  guint timeout_ret = g_timeout_add(200, (GSourceFunc) cb_print_position,
				    pipeline);
  g_print("timeout_ret = %d\n", timeout_ret);
  
  
  return return_data;
}
// gst_player --------------------------------------------------------------







