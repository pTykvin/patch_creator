#include <stdio.h>
#include <unistd.h>
#include <string.h>

int do_cmd(char * command) 
{
	system(command);
    return 0;
}

void for_build(char * s, char * d) {

   	char buffer[1024], cmd[1024];

   	strcpy(buffer, "mkdir -p ");
   	strcpy(cmd, d);
	strcat(buffer, cmd);
   	do_cmd(buffer);

   	strcpy(buffer, "cp -rf ");
   	strcpy(cmd, s);
	strcat(buffer, cmd);
	strcat(buffer, " ");
	strcpy(cmd, d);
	strcat(buffer, cmd);
   	do_cmd(buffer);

   	fprintf(stderr, "\n[COPY]\n\t[FROM] %s\n\t[TO] %s\n", s, d);
}

int main(int argc, char *argv[]) {

	char from[512], to[512], buffer[1024];
	while(fgets(buffer, 1024, stdin)) {
		if (strstr(buffer, "[DEL]")) {
			char * b;
			b = strstr(buffer, "SCO/") + 4;
			strcpy(from, b);
			char * c;
			c = strstr(from,"..") - 1;
			*c = '\0';
			printf("%s", from);
			continue;
		}
		sscanf(buffer, "%[^:]:%s", from, to);
		for_build(from, to);
	}	
	return 0;
}
