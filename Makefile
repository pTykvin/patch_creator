opt/tmp_saver.o: src/tmp_saver.c src/tmp_saver.h
	gcc -c src/tmp_saver.c -o opt/tmp_saver.o
opt/colorcon.o: src/colorcon.c src/colorcon.h
	gcc -c src/colorcon.c -o opt/colorcon.o
opt/global.o: src/global.c src/global.h
	gcc -c src/global.c -o opt/global.o
opt/command.o: src/command.c src/command.h src/tmp_saver.h src/global.h src/colorcon.h
	gcc -c src/command.c -o opt/command.o
opt/list.o: src/list.c src/list.h src/colorcon.h
	gcc -c src/list.c -o opt/list.o
opt/differencer.o: src/differencer.c src/colorcon.h
	gcc -c src/differencer.c -o opt/differencer.o 
opt/main.o: src/main.c src/command.h src/colorcon.h src/list.h src/global.h
	gcc -c src/main.c -o opt/main.o 


dir_tree:
	mkdir -p opt
	mkdir -p tmp
	mkdir -p build

list: opt/list.o opt/colorcon.o
	gcc opt/list.o opt/colorcon.o -o bin/list
main: dir_tree opt/main.o opt/command.o opt/colorcon.o opt/tmp_saver.o opt/global.o opt/list.o
	gcc opt/main.o opt/command.o opt/colorcon.o opt/tmp_saver.o opt/global.o opt/list.o -o main
differencer: dir_tree opt/differencer.o opt/colorcon.o opt/list.o 
	gcc opt/differencer.o opt/colorcon.o -o differencer