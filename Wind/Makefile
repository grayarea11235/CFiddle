CC=gcc -c
LIBS=-lgdi32 -lcomctl32

# Makefile
# wind.dll: dllmain.o
# 	gcc -shared -o wind.dll dllmain.o

# dllmain.o: dllmain.cpp wind.h
# 	gcc -c dllmain.cpp 



# Wind
all: main.o log.o linked_list.o application.o button.o form.o dialog.o checkbox.o
	gcc main.o log.o linked_list.o application.o button.o form.o dialog.o checkbox.o $(LIBS) -W --machine-windows -o testwin.exe

clean:
	rm testwin.exe
	rm *.o

main.o: main.c
	$(CC) main.c

log.o: log.c log.h
	$(CC) log.c

linked_list.o: linked_list.c linked_list.h
	$(CC) linked_list.c

application.o: application.c application.h
	$(CC) application.c

button.o: button.c button.h
	$(CC) button.c

form.o: form.c form.h
	$(CC) form.c

dialog.o: dialog.c dialog.h
	gcc -c dialog.c

checkbox.o: checkbox.c checkbox.h
	$(CC) checkbox.c

# windbg
windbg: windbg.o
	gcc windbg.o -W --machine-windows -o windbg.exe

windbg.o: windbg.cpp
	g++ -c windbg.cpp

