#include <stdio.h>

int tmp_save(char * filename, char * value) {
	FILE * file;
	if (file = fopen(filename, "w")) 
		fprintf(file, "%s", value);
	return 1;
}

char * tmp_load(char * filename) {
	FILE * file;
	char * value;
	if (file = fopen(filename, "r")) 
		fscanf(file, "%s", value);
	return value;
}