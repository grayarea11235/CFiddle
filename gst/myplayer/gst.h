// gst header
#ifndef GST_H
#define GST_H

#include <gtk/gtk.h>
#include <gst/gst.h>


// Deprecated
typedef struct _stream_info
{
  GstElement *pipeline;
  guint bus_watch_id;
} stream_info;

// Contrains the main pipeline for the player
typedef struct _gst_info
{
  GstElement *pipeline;
  GstElement *volume;
  GstElement *source;
  guint bus_watch_id;
} gst_info;

// TODO : This is BAD
static stream_info *data;


extern void gst_cleanup();
extern void gst_pause();
extern void gst_start(char *filename);
extern void gst_stop();

#endif


