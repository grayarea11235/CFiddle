CC=gcc -c
CFLAGS=`pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --cflags --libs gtk+-2.0`

all: hello_gtk_notify

clean:
	@echo "TODO"

hello_gtk_notify: hello_gtk_notify.o
	gcc hello_gtk_notify.o -ohello_gtk_notify $(LDFLAGS) -lnotify

hello_gtk_notify.o:
	gcc $(CFLAGS) -c hello_gtk_notify.c #-lnotify


#gtk_simple_app: gtk_simple_app.o
#	$(CC) 

#gtk_simple_app.o: gtk_simple_app.c
