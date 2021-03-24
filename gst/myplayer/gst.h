// gst header
#ifndef GST_H
#define GST_H

typedef struct _stream_info
{
  GstElement *pipeline;
  guint bus_watch_id;
} stream_info;


static stream_info *data;

#endif


