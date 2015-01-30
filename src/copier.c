#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "colorcon.h"

int do_cmd(char * command) 
{

    FILE* file = popen(command, "r");
	char buffer[256];
    while (fgets(buffer, 256, file))
		printf("%s", buffer);
    pclose(file);
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

   	out_cyan("\n[COPY]");   	
   	out_blue("\n\tFROM ");
   	out_white(s);
   	out_blue("\n\tTO ");
	out_white(d);
}

int main(int argc, char *argv[]) {

	char from[512], to[512], buffer[1024];
	while(fgets(buffer, 1024, stdin)) {
		sscanf(buffer, "%[^:]:%s", from, to);
		for_build(from, to);
	}
	out_white("\n");
	out_green("\n============================================================================\n                                BUILD READY\n============================================================================\n");
}