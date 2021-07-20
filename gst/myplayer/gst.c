#include "gst.h"

// gst.c
static void dump_info(stream_info *data)
{
  g_print("\n-----------------------------------------------------------------------------\n");
  g_print("pipline      = %p\n", data->pipeline);
  g_print("bus_watch_id = %d\n", data->bus_watch_id);
  g_print("-----------------------------------------------------------------------------\n");
}

static void print_one_tag(const GstTagList * list,
			  const gchar * tag,
			  gpointer user_data)
{
  int i, num;

  num = gst_tag_list_get_tag_size(list, tag);
  for (i = 0; i < num; ++i)
  {
    const GValue *val;

    /* Note: when looking for specific tags, use the gst_tag_list_get_xyz() API,
     * we only use the GValue approach here because it is more generic */
    val = gst_tag_list_get_value_index(list, tag, i);
    if (G_VALUE_HOLDS_STRING(val))
    {
      g_print("\t%20s : %s\n", tag, g_value_get_string(val));
    }
    else if (G_VALUE_HOLDS_UINT (val))
    {
      g_print("\t%20s : %u\n", tag, g_value_get_uint(val));
    }
    else if (G_VALUE_HOLDS_DOUBLE(val))
    {
      g_print("\t%20s : %g\n", tag, g_value_get_double(val));
    }
    else if(G_VALUE_HOLDS_BOOLEAN(val))
    {
      g_print("\t%20s : %s\n", tag,
	      (g_value_get_boolean (val)) ? "true" : "false");
    }
    else if(GST_VALUE_HOLDS_BUFFER (val))
    {
      GstBuffer *buf = gst_value_get_buffer (val);
      guint buffer_size = gst_buffer_get_size (buf);

      g_print("\t%20s : buffer of size %u\n", tag, buffer_size);
    }
    else if(GST_VALUE_HOLDS_DATE_TIME(val)) {
      GstDateTime *dt = g_value_get_boxed(val);
      gchar *dt_str = gst_date_time_to_iso8601_string(dt);

      g_print("\t%20s : %s\n", tag, dt_str);
      g_free(dt_str);
    }
    else
    {
      g_print("\t%20s : tag of type '%s'\n", tag, G_VALUE_TYPE_NAME(val));
    }
  }
}

static void on_new_pad(GstElement *dec,
		       GstPad *pad,
		       GstElement *fakesink)
{
  GstPad *sinkpad;

  sinkpad = gst_element_get_static_pad(fakesink, "sink");
  if (!gst_pad_is_linked(sinkpad))
  {
    if (gst_pad_link(pad, sinkpad) != GST_PAD_LINK_OK)
    {
      g_error("Failed to link pads!");
    }
  }
}

// -----------------------------------------------------------------------------------------
//
// Function : gst_meta_info
//
// Description : 
//
// -----------------------------------------------------------------------------------------
void gst_meta_info(const gchar *uri)
{
  GstElement *pipe;
  GstElement *dec;
  GstElement *sink;
  GstMessage *msg;
//  gchar *uri = "file:///home/cpd/Devel/CFiddle/gst/myplayer/piano2-Audacity1.2.5.mp3";

//  gchar *uri = "file:///media/cpd/3634-6261/Music/(2000) Kid A/01 Radiohead - Everything In Its Right Place.mp3";

  
  pipe = gst_pipeline_new("pipeline");
  
  dec = gst_element_factory_make("uridecodebin", NULL);
  g_object_set(dec, "uri", uri, NULL);
  gst_bin_add(GST_BIN (pipe), dec);
  
  sink = gst_element_factory_make("fakesink", NULL);
  gst_bin_add(GST_BIN (pipe), sink);
  
  g_signal_connect(dec, "pad-added", G_CALLBACK (on_new_pad), sink);
  
  gst_element_set_state(pipe, GST_STATE_PAUSED);

  g_print("About to loop\n");
  while (TRUE)
  {
    GstTagList *tags = NULL;
    
    msg = gst_bus_timed_pop_filtered(GST_ELEMENT_BUS (pipe),
				     GST_CLOCK_TIME_NONE,
				     GST_MESSAGE_ASYNC_DONE | GST_MESSAGE_TAG | GST_MESSAGE_ERROR);
    
    if (GST_MESSAGE_TYPE (msg) != GST_MESSAGE_TAG) /* error or async_done */
    {
      break;
    }

    gst_message_parse_tag(msg, &tags);

    g_print("Got tags from element %s:\n", GST_OBJECT_NAME (msg->src));
    gst_tag_list_foreach(tags, print_one_tag, NULL);
    g_print("\n");
    gst_tag_list_unref(tags);

    gst_message_unref(msg);
  }

  if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR)
  {
    GError *err = NULL;

    gst_message_parse_error(msg, &err, NULL);
    g_printerr("Got error: %s\n", err->message);
    g_error_free(err);
  }

  gst_message_unref(msg);
  gst_element_set_state(pipe, GST_STATE_NULL);
  gst_object_unref(pipe);
//  g_free(uri);
}
// -----------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------
//
// Function : cb_print_position
//
// Description : 
//
// -----------------------------------------------------------------------------------------
gboolean cb_print_position(GstElement *pipeline)
{
  gint64 pos, len;
  
  if (gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos)
      && gst_element_query_duration(pipeline, GST_FORMAT_TIME, &len))
  {
    gint64 percent = (pos * 100) / len;
    
    g_print("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT " %ld\r",
	    GST_TIME_ARGS(pos), GST_TIME_ARGS(len), percent);
  }

  // call me again
  return TRUE;
}
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
//
// Function : bus_call
//
// Description :
//
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
//
// Function : gst_player_stop
//
// Description : Stop playing the pipeline
//
// -----------------------------------------------------------------------------------------
void gst_player_stop(gst_info_t *info)
{
  // How do I reset?
  gst_element_set_state(info->pipeline, GST_STATE_PAUSED);
  gst_element_set_state(info->pipeline, GST_STATE_NULL);
}
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
//
// Function : gst_player_pause
//
// Description : Pause the gst stream
//
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
  g_print("About to set timeout\n");
  guint timeout_ret = g_timeout_add(200, (GSourceFunc) cb_print_position,
				    info->pipeline);
  g_print("timeout_ret = %d\n", timeout_ret);
  
  // Set the filename - Should be in play
  g_object_set(G_OBJECT(info->source), "location", filename, NULL);

  g_print("Now playing: %s\n", filename);
  g_print("pipeline = %p\n", info->pipeline);
  gst_player_dump_info(info);

  gst_element_set_state(info->pipeline, GST_STATE_PLAYING);
  g_print("Started\n");
}
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
//
// Function : gst_player_startup
//
// Description :
//
// -----------------------------------------------------------------------------------------
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

  g_print("return_data = %p\n", return_data);

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

//  g_print("About to set timeout\n");
//  guint timeout_ret = g_timeout_add(200, (GSourceFunc) cb_print_position,
//				    pipeline);
//  g_print("timeout_ret = %d\n", timeout_ret);
  
  
  return return_data;
}
// -----------------------------------------------------------------------------------------

// gst_player --------------------------------------------------------------







