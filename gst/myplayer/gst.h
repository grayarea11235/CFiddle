// gst header
#ifndef GST_H
#define GST_H

#include <gtk/gtk.h>
#include <gst/gst.h>


typedef struct _stream_info
{
  GstElement *pipeline;
  guint bus_watch_id;
} stream_info;


// TODO : This is BAD
static stream_info *data;


extern void gst_cleanup();
extern void gst_pause();
extern void gst_start(char *filename);
extern void gst_stop();

#endif


