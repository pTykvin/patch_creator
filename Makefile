tmp_saver.o:
	gcc -c tmp_saver.c
colorcon.o: colorcon.c colorcon.h
	gcc -c colorcon.c
command.o: command.c command.h tmp_saver.c tmp_saver.h
	gcc -c command.c
hashtable.o: hashtable.c hashtable.h colorcon.c colorcon.h
	gcc -c hashtable.c
main.o: main.c command.c command.h colorcon.c colorcon.h hashtable.c hashtable.h tmp_saver.c tmp_saver.h
	gcc -c main.c 

hashtable: hashtable.o colorcon.o
	gcc hashtable.o colorcon.o -o hashtable
main: main.o command.o colorcon.o hashtable.o tmp_saver.o
	gcc main.o command.o colorcon.o hashtable.o tmp_saver.o -o main