C:\MinGW\bin\gcc -g3 -std=c11 ./main.c ./log.c ./linked_list.c ./application.c -s -lgdi32 -lcomctl32 -W --machine-windows -o testwin.exe


#gcc ./main.c -s -lgdi32 -lcomctl32 -Wl,--subsystem,windows
