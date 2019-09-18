CC=gcc
CFLAGS=-c `pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --cflags --libs gtk+-3.0`

all: hello_gtk_notify gtk_simple_app gtk_simple_app_with_btn simp_client

clean:
	@echo "Cleaning..."
	rm -f *.o
	rm -f ./hello_gtk_notify
	rm -f ./gtk_simple_app
	rm -f ./gtk_simple_app_with_btn
	@echo "Cleaned"

hello_gtk_notify: hello_gtk_notify.o
	$(CC) hello_gtk_notify.o -o hello_gtk_notify $(LDFLAGS) -lnotify

hello_gtk_notify.o: hello_gtk_notify.c
	$(CC) $(CFLAGS) hello_gtk_notify.c $(LDFLAGS)

gtk_simple_app: gtk_simple_app.o
	$(CC) gtk_simple_app.o -o gtk_simple_app $(LDFLAGS)

gtk_simple_app.o: gtk_simple_app.c
	$(CC) $(CFLAGS) gtk_simple_app.c $(LDFLAGS) 

gtk_simple_app_with_btn: gtk_simple_app_with_btn.o
	$(CC) gtk_simple_app_with_btn.o -o gtk_simple_app_with_btn $(LDFLAGS)

gtk_simple_app_with_btn.o: gtk_simple_app_with_btn.c
	$(CC) $(CFLAGS) gtk_simple_app_with_btn.c $(LDFLAGS)

simp_client: simp_client.o
	$(CC) simp_client.o -o simp_client $(LDFLAGS)

simp_client.o: simp_client.c
	$(CC) $(CFLAGS) simp_client.c
