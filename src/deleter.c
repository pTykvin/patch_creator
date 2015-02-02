#include <stdio.h>
#include <unistd.h>
#include <string.h>

FILE * open_file(char * path) {

	FILE * file;
	char buffer[1024];
	strcpy(buffer, path);
	strcat(buffer, "restore/");	
	char cmd[1024];
	strcpy(cmd, "mkdir -p ");
	strcat(cmd, buffer);
	system(cmd);
	strcat(buffer, "delete_files.xml");
	printf("buffer: %s\n", buffer);
	file = fopen(buffer, "w");
	return file;

}

int main(int argc, char *argv[]) {

	char ch;
	char * patch = NULL;	

		while ((ch = getopt(argc, argv, "d:")) != EOF)
		switch (ch) {
			case 'd':
				patch = optarg;
			break;			
			default:
				printf("%s\n", "Неизвестный параметр: ");
				return 1;
		}
		if (patch == NULL) {
			printf("%s\n", "Укажите обязательный ключ -d <patch_path>");
			return 9;
		}	

	FILE * delete_files = open_file(patch);
	fputs("<delete>\n", delete_files);
	char path[512];
	while(fgets(path, 1024, stdin)) {
		fprintf(delete_files, "<file>%s</file>\n", path);
		printf("DELETE: %s\n", path);
	}
	fputs("</delete>", delete_files);
	fprintf(stderr, "%s", "\n\n============================================================================\n                                BUILD READY\n============================================================================\n");
	return 0;
}
