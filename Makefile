tmp_saver.o:
	gcc -c tmp_saver.c
colorcon.o: colorcon.c colorcon.h
	gcc -c colorcon.c
global.o: global.c global.h
	gcc -c global.c
command.o: command.c command.h tmp_saver.h global.h
	gcc -c command.c
list.o: list.c list.h colorcon.h
	gcc -c list.c
main.o: main.c command.h colorcon.h list.h global.h
	gcc -c main.c 

list: list.o colorcon.o
	gcc list.o colorcon.o -o list
main: main.o command.o colorcon.o tmp_saver.o global.o list.o
	gcc main.o command.o colorcon.o tmp_saver.o global.o list.o -o main