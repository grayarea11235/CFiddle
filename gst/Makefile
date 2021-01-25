# gst test makefile
# gcc -Wall gst_simple_audio.c -o gst_simple_audio $(pkg-config --cflags --libs gstreamer-1.0)
GTKFLAGS=`pkg-config --libs --cflags gtk+-3.0`
GSTFLAGS=`pkg-config --cflags --libs gstreamer-1.0` 

COMPILE=gcc

all: basic-tutorial-1 gst_simple_audio gst_mp3_audio gst_mp3_ui

clean:
	rm -f basic-tutorial-1
	rm -f gst_simple_audio
	rm -f gst_mp3_audio
	rm -f gst_mp3_ui
	rm -f *.o


basic-tutorial-1: basic-tutorial-1.c
	$(COMPILE) $< -o $@ `pkg-config --cflags --libs gstreamer-1.0`
 
gst_simple_audio: gst_simple_audio.c
	$(COMPILE) $< -o $@ `pkg-config --cflags --libs gstreamer-1.0`

gst_mp3_audio: gst_mp3_audio.c
	$(COMPILE) $< -o $@ `pkg-config --cflags --libs gstreamer-1.0`

gst_ui_audio: gst_ui_audio.c
	$(COMPILE) $(GTKFLAGS)  $< -o $@
