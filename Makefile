command.o: command.c command.h
	gcc -c command.c
main.o: main.c command.c command.h
	gcc -c main.c 
main: main.o command.o
	gcc main.o command.o -o main