spath: main.o strings.o
	gcc -Wall main.o strings.o -o spath
	rm main.o strings.o
	
main.o: main.c strings.c strings.h
	gcc -c main.c
	
spath.o: strings.c strings.h
	gcc -c strings.c

	
