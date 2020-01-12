

wind.dll: dllmain.o
	gcc -shared -o wind.dll dllmain.o

dllmain.o: dllmain.cpp wind.h
	gcc -c dllmain.cpp 


# Wind
wind: main.o log.o linked_list.o application.o button.o
	gcc main.o log.o linked_list.o application.o button.o -lgdi32 -lcomctl32 -W --machine-windows -o testwin.exe

cleanwind:
	rm testwin.exe
	rm *.o


main.o: main.c
	gcc -c main.c

log.o: log.c log.h
	gcc -c log.c

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c

application.o: application.c application.h
	gcc -c application.c

button.o: button.c
	gcc -c button.c


# windbg
windbg: windbg.o
	gcc win_dbg.o --machine-windows -o windbg.exe

windbg.o: windbg.c
	gcc -c windbg.c

