#include <stdio.h>
#include <unistd.h>
#include <string.h>

void for_delete(char * path) {
	//printf("for_delete: %s\n", path);	
}

void for_copy(char * path, char * deploy, char * patch) {
	char buffer[1024];
	strcpy(buffer, deploy);
	strcat(buffer, strchr(path,'/'));
	strcat(buffer, ":");
	strcat(buffer, patch);
	char * c;
	c = strrchr(path,'/');
	*c = '\0';
	strcat(buffer, strchr(path,'/'));
	printf("%s\n", buffer);
}

int main(int argc, char *argv[]) {

	char ch;
	char * deploy = NULL;
	char * patch = NULL;
	while ((ch = getopt(argc, argv, "d:s:")) != EOF)
		switch (ch) {
			case 's':
				deploy = optarg;
			break;
			case 'd':
				patch = optarg;
			break;			
			default:
				printf("%s\n", "Неизвестный параметр: ");
				return 1;
		}
		if (deploy == NULL) {
			printf("%s\n", "Укажите обязательный ключ -s <deploy_path>");
			return 9;
		}
		if (patch == NULL) {
			printf("%s\n", "Укажите обязательный ключ -d <patch_path>");
			return 9;
		}		

	char flag[2], path[510], buffer[512];
	while(fgets(buffer, 512, stdin)) {
		sscanf(buffer, "%[^:]:%s", flag, path);
		switch (flag[0]) {
			case 'D':
				for_delete(path);
			break;			
			default:
				for_copy(path, deploy, patch);

		}
	}
	return 0;
}