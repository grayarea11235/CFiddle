#include <stdio.h>
#include <libnotify/notify.h>
#include "argtable3.h"

// API docs are here https://developer.gnome.org/libnotify/unstable/NotifyNotification.html

/* global arg_xxx structs */
struct arg_lit *verb, *help, *version;
struct arg_int *level;
struct arg_int *timeout;
struct arg_file *o, *file;
struct arg_str *title;
struct arg_str *message;
struct arg_str *hint;
struct arg_end *end;

void print_usage(char *name)
{
  printf("Usage : %s <title> <message>\n", name);
}

void callback(NotifyNotification *notification,
    char *action,
    gpointer user_data)
{

}

void print_server_caps()
{
  GList *res = notify_get_server_caps(); 
  printf("List length = %d\n", g_list_length(res));
  
  GList *l;
  int i = 0;
  for (l = res; l != NULL; l = l->next)
  {
    printf("%s\n", (char *)l->data);
  }

  g_list_foreach(res, (GFunc)g_free, NULL);
  g_list_free(res);
}

int main(int argc, char *argv[])
{
  print_server_caps();

  void *argtable[] = {
    help    = arg_litn(NULL, "help", 0, 1, "Display this help and exit"),
    version = arg_litn(NULL, "version", 0, 1, "Display version info and exit"),
    timeout = arg_intn("o", "timeout", "<n>", 0, 1, "Timeout of the notification in millisecs. If ommited use system default. If expire never set to 0"),
    //verb    = arg_litn("v", "verbose", 0, 1, "verbose output"),
    title   = arg_strn("t", "title", "string", 1, 1, "The title of the notification."),
    message = arg_strn("m", "message", "string", 1, 1, "The message of the notification"),
    hint    = arg_strn("h", "hint", "string", 0, 1, "The hint of the notification"),
    end     = arg_end(20),
  };

  int exitcode = 0;
  char progname[] = "notify";

  int nerrors;
  nerrors = arg_parse(argc, argv, argtable);

  /* special case: '--help' takes precedence over error reporting */
  if (help->count > 0)
  {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
    printf("Demonstrate command-line parsing in argtable3.\n\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    exitcode = 0;
    goto exit;
  }

  /* If the parser returned any errors then display them and exit */
  if (nerrors > 0)
  {
    /* Display the error details contained in the arg_end struct.*/
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }

  notify_init("gtknotifycmd");

  // Stuff to add:
  //
  // Timeout if -1 display forevber, if omitted then the default
  // Hint
  // Urgency?
  // Icon
  // Custom icon
  // Actions

  //  NotifyNotification * Hello = notify_notification_new("Hello world", "This is an example notification.", "dialog-information");
  NotifyNotification *gtk_notify_cmd = notify_notification_new(title->sval[0], message->sval[0], "dialog-information");

  printf("timeout->count = %d\n", timeout->count);
  if (timeout->ival != NULL)
  {
    printf("timeout->ival = %d\n", timeout->ival[0]);
  }

  // Add a test action
  notify_notification_add_action(gtk_notify_cmd, "action-name", "Test", NULL, NULL, NULL);

  // Do the timeout
  int actual_timeout = NOTIFY_EXPIRES_DEFAULT;
  if (timeout->count > 0)
  {
    if (timeout->ival[0] == 0)
    {
      actual_timeout = NOTIFY_EXPIRES_NEVER;
    }
    if (timeout->ival[0] > 0)
    {
      actual_timeout = timeout->ival[0];
    }
  }

  printf("actual_timeout=%d\n", actual_timeout);

  notify_notification_set_timeout(gtk_notify_cmd, actual_timeout); 

  // NOTIFY_EXPIRES_NEVER 
  notify_notification_show(gtk_notify_cmd, NULL);
  g_object_unref(G_OBJECT(gtk_notify_cmd));

  notify_uninit();

exit:
  /* deallocate each non-null entry in argtable[] */
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
  return exitcode;
}
