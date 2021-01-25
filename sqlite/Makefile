all: sqlite_getversion

clean:
	rm -f *.o
	rm -f sqlite_getversion

sqlite_getversion: sqlite_getversion.c
	gcc $< -o $@ `pkg-config --cflags --libs sqlite3`	

