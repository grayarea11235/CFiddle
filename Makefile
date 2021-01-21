CC=gcc
CFLAGS=-c `pkg-config --cflags gtk+-3.0` -Iinclude
LDFLAGS=`pkg-config --cflags --libs gtk+-3.0`
DBUSCFLAGS=`pkg-config --cflags dbus-1`
DBUSLFLAGS=`pkg-config --libs dbus-1` 

# gcc ./dbus_example.c  -o dbus_example -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -ldbus-1

all: hello_gtk_notify notify gtk_simple_app gtk_simple_app_with_btn simp_client read_web simp_server simp_client dbus_example


clean:
	@echo "Cleaning..."
	rm -f *.o
	rm -f ./hello_gtk_notify
	rm -f ./gtk_simple_app
	rm -f ./gtk_simple_app_with_btn
	rm -f ./read_web
	rm -f ./notify
	rm -f ./dbus_example
	rm -f ./simp_server
	rm -f ./simp_client
	@echo "Cleaned"


simp_server: simp_server.o
	$(CC) simp_server.o -o simp_server $(LDFLAGS)

simp_server.o: simp_server.c
	$(CC) -c simp_server.c

simp_client: simp_client.o
	$(CC) simp_client.o -o simp_client $(LDFLAGS)

simp_client.o: simp_client.c
	$(CC) -c simp_client.c

dbus_example: dbus_example.o
	$(CC) dbus_example.o -o dbus_example $(LDFLAGS) $(DBUSLFLAGS)

dbus_example.o: dbus_example.c
<<<<<<< HEAD
	$(CC) $(CFLAGS) $(DBUSCFLAGS) dbus_example.c $(DBUSLFLAGS)
=======
	$(CC) -c $(DBUSCFLAGS) dbus_example.c $(DBUSLFLAGS)
>>>>>>> e0f90f89a7f5a683753007a5dc1d4eba40b815b0

hello_gtk_notify: hello_gtk_notify.o
	$(CC) hello_gtk_notify.o -o hello_gtk_notify $(LDFLAGS) -lnotify

hello_gtk_notify.o: hello_gtk_notify.c
	$(CC) $(CFLAGS) hello_gtk_notify.c $(LDFLAGS)

notify: notify.o argtable3.o
	$(CC) notify.o argtable3.o -o notify $(LDFLAGS) -lnotify -lm

argtable3.o: libs/argtable3/argtable3.c
	$(CC) $(CFLAGS) libs/argtable3/argtable3.c $(LDFLAGS)

notify.o: notify.c
	$(CC) $(CFLAGS) notify.c $(LDFLAGS)

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
