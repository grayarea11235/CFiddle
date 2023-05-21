## pkg-config --cflags --libs libavcodec
# avframe avutil <-- Important


all: libav_ex1

clean: 
	rm -f *.o rm -f libav_ex1

libav_ex1: libav_ex1.c
	gcc libav_ex1.c `pkg-config --cflags --libs libavcodec libavutil libavformat` -o libav_ex1
