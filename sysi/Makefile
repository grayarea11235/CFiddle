# g++ ./process.cpp -lkernel32 -lpsapi -luser32 -o process.exe

all: process.o
	g++ process.o -lkernel32 -lpsapi -luser32 -o process.exe

process.o: process.cpp
	g++ -c process.cpp


clean:
	rm process.exe
	rm *.o
