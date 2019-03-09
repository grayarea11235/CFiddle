#include <stdio.h>
#include <libnotify/notify.h>


void print_usage(char *name) {
  printf("%s <title> <message>\n", name);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    print_usage(argv[0]);
    return 0;
  }

  notify_init("Hello world!");

  
  
  //  NotifyNotification * Hello = notify_notification_new("Hello world", "This is an example notification.", "dialog-information");
  NotifyNotification * Hello = notify_notification_new(argv[1], argv[2], "dialog-information");

  notify_notification_show (Hello, NULL);
  g_object_unref(G_OBJECT(Hello));

  notify_uninit();

  return 0;
}
