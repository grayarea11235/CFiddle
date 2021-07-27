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

// Contains the main pipeline for the player
typedef struct _gst_info_t
{
  GstElement *decoder;
  GstElement *conv;
  GstElement *sink;
  GstElement *pipeline;
  GstElement *volume;
  GstElement *source;
  guint bus_watch_id;

  void (*progress_callback)(gint64 pos, gint64 length);
} gst_info_t;


extern void gst_cleanup();
extern void gst_pause();
extern void gst_start(char *filename);
extern void gst_stop();

// The new interface
extern gst_info_t *gst_player_init();
extern void gst_player_play(gst_info_t *info, gchar *filename);
extern void gst_player_pause(gst_info_t *info);
extern void gst_player_stop(gst_info_t *info);

extern void gst_meta_info();

#endif


