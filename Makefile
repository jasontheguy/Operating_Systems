hw5: main.o mem.o
	gcc -g main.o mem.o -o hw5
main.o: main.c mem.h 
	gcc -c main.c -Wall
mem.o: mem.c mem.h 
	gcc -c -Wall mem.c -g