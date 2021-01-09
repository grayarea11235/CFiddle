#include <stdio.h>
#include <libnotify/notify.h>
#include "argtable3.h"

// API docs are here https://developer.gnome.org/libnotify/unstable/NotifyNotification.html

/* global arg_xxx structs */
struct arg_lit *verb, *help, *version;
struct arg_int *level;
struct arg_file *o, *file;
struct arg_end *end;

void print_usage(char *name)
{
  printf("Usage : %s <title> <message>\n", name);
}

int main(int argc, char *argv[])
{
  // the global arg_xxx structs are initialised within the argtable
  void *argtable[] = {
    help    = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
    version = arg_litn(NULL, "version", 0, 1, "display version info and exit"),
    level   = arg_intn(NULL, "level", "<n>", 0, 1, "foo value"),
    verb    = arg_litn("v", "verbose", 0, 1, "verbose output"),
    o       = arg_filen("o", NULL, "myfile", 0, 1, "output file"),
    file    = arg_filen(NULL, NULL, "<file>", 1, 100, "input files"),
    end     = arg_end(20),
  };

    if (argc != 3)
    {
      print_usage(argv[0]);
      return 0;
    }

    notify_init("gtknotifycmd");

    // Stuff to add:
    //
    // Timeout
    // Hint
    // Urgency?
    // Icon
    // Custom icon
    // Actions


    //  NotifyNotification * Hello = notify_notification_new("Hello world", "This is an example notification.", "dialog-information");
    NotifyNotification *gtk_notify_cmd = notify_notification_new(argv[1], argv[2], "dialog-information");

    notify_notification_set_timeout(gtk_notify_cmd, 1000); 
    //  notify_notification_set_timeout(gtk_notify_cmd, NOTIFY_EXPIRES_NEVER); 
    //  notify_notification_set_timeout(gtk_notify_cmd, NOTIFY_EXPIRES_DEFAULT); 

    // NOTIFY_EXPIRES_NEVER 
    notify_notification_show(gtk_notify_cmd, NULL);
    g_object_unref(G_OBJECT(gtk_notify_cmd));

    notify_uninit();

    return 0;
  }
