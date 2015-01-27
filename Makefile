tmp_saver.o: src/tmp_saver.c src/tmp_saver.h
	gcc -c src/tmp_saver.c -o opt/tmp_saver.o
colorcon.o: src/colorcon.c src/colorcon.h
	gcc -c src/colorcon.c -o opt/colorcon.o
global.o: src/global.c src/global.h
	gcc -c src/global.c -o opt/global.o
command.o: src/command.c src/command.h src/tmp_saver.h src/global.h
	gcc -c src/command.c -o opt/command.o
list.o: src/list.c src/list.h src/colorcon.h
	gcc -c src/list.c -o opt/list.o
main.o: src/main.c src/command.h src/colorcon.h src/list.h src/global.h
	gcc -c src/main.c -o opt/main.o 

dir_tree:
	mkdir opt
	mkdir bin

list: list.o colorcon.o
	gcc opt/list.o opt/colorcon.o -o bin/list
main: main.o command.o colorcon.o tmp_saver.o global.o list.o
	gcc opt/main.o opt/command.o opt/colorcon.o opt/tmp_saver.o opt/global.o opt/list.o -o bin/main