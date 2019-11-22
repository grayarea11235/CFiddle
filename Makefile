CC=gcc
CFLAGS=-c `pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --cflags --libs gtk+-3.0`

all: hello_gtk_notify gtk_simple_app gtk_simple_app_with_btn read_web

clean:
	@echo "Cleaning..."
	rm -f *.o
	rm -f ./hello_gtk_notify
	rm -f ./gtk_simple_app
	rm -f ./gtk_simple_app_with_btn
	rm -f ./read_web
	@echo "Cleaned"

hello_gtk_notify: hello_gtk_notify.o
	$(CC) hello_gtk_notify.o -o hello_gtk_notify $(LDFLAGS) -lnotify

hello_gtk_notify.o:
	$(CC) $(CFLAGS) hello_gtk_notify.c $(LDFLAGS)

gtk_simple_app: gtk_simple_app.o
	$(CC) gtk_simple_app.o -o gtk_simple_app $(LDFLAGS)

read_web: read_web.o
	$(CC) read_web.o -o read_web $(LDFLAGS)

gtk_simple_app.o: gtk_simple_app.c
	$(CC) $(CFLAGS) gtk_simple_app.c $(LDFLAGS) 

gtk_simple_app_with_btn: gtk_simple_app_with_btn.o
	$(CC) gtk_simple_app_with_btn.o -o gtk_simple_app_with_btn $(LDFLAGS)

gtk_simple_app_with_btn.o: gtk_simple_app_with_btn.c
	$(CC) $(CFLAGS) gtk_simple_app_with_btn.c $(LDFLAGS)

read_web.o: read_web.c
	$(CC) $(CFLAGS) read_web.c $(LDFLAGS)
