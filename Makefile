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
opt/differencer.o: src/differencer.c src/colorcon.h src/list.h
	gcc -c src/differencer.c -o opt/differencer.o 
opt/analizer.o: src/analizer.c src/colorcon.h src/list.h
	gcc -c src/analizer.c -o opt/analizer.o 
opt/copier.o: src/copier.c src/colorcon.h
	gcc -c src/copier.c -o opt/copier.o 
opt/checkouter.o: src/checkouter.c src/colorcon.h
	gcc -c src/checkouter.c -o opt/checkouter.o 
opt/libs_differ.o: src/libs_differ.c
	gcc -c src/libs_differ.c -o opt/libs_differ.o 	
opt/main.o: src/main.c src/command.h src/colorcon.h src/list.h src/global.h
	gcc -c src/main.c -o opt/main.o 


dir_tree:
	mkdir -p opt
	mkdir -p tmp
	mkdir -p build
	mkdir -p art

list: opt/list.o opt/colorcon.o
	gcc opt/list.o opt/colorcon.o -o bin/list
main: dir_tree opt/main.o opt/command.o opt/colorcon.o opt/tmp_saver.o opt/global.o opt/list.o
	gcc opt/main.o opt/command.o opt/colorcon.o opt/tmp_saver.o opt/global.o opt/list.o -o main
differencer: opt/differencer.o opt/colorcon.o opt/list.o 
	gcc opt/differencer.o opt/colorcon.o opt/list.o -o differencer
analizer: opt/analizer.o opt/colorcon.o opt/list.o 
	gcc opt/analizer.o opt/colorcon.o opt/list.o -o analizer
checkouter: opt/checkouter.o opt/colorcon.o
	gcc opt/checkouter.o opt/colorcon.o -o checkouter
copier: opt/copier.o opt/colorcon.o
	gcc opt/copier.o opt/colorcon.o -o copier
libs_differ: opt/libs_differ.o
	gcc opt/libs_differ.o -o libs_differ	
install: dir_tree differencer analizer checkouter copier libs_differ