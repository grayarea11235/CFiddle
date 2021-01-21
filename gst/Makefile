# gst test makefile

COMPILE=gcc

all: basic-tutorial-1

clean:
	rm -f basic-tutorial-1
	rm -f *.o

basic-tutorial-1: basic-tutorial-1.c
	$(COMPILE) $< -o $@ `pkg-config --cflags --libs gstreamer-1.0`
 
