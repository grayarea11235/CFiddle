wind.dll: dllmain.o
	gcc -shared -o wind.dll dllmain.o

dllmain.o: dllmain.cpp wind.h
	gcc -c dllmain.cpp 
