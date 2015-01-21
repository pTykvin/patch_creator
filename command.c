#include <stdio.h>
#include "command.h"

typedef enum {
	CLONE,
	PULL,
	DIFF
} CMD;

int command(CMD cmd) 
{
	char *command;
	switch(cmd) {
		case CLONE:
			command = "git clone git@github.com:pTykvin/ConsumingFormatter.git repo";
		break;
		case PULL:
			command = "git pull";
		break;
		case DIFF:
			command = "git diff --raw 81ebdbadf415da1c77094a945095f888fff6f24f master";
	}

    FILE* file = popen(command, "r");
    if (cmd == DIFF) {
    	char buffer[5][32];
    	while(fgets(buffer[0], 256, file)) {
    		printf("%s ", buffer[0]);
    	}
    } else {
    	char buffer[256];
	    while (fscanf(file, "%255[^\n]", buffer)) {
			printf("%s\n", buffer);
	    }
	}
    pclose(file);
    return 0;
}

int pull() {
	printf("%s\n", "[PULL]");
	return command(PULL);
}

int clone() {
	printf("%s\n", "[CLONE]");
	return command(CLONE);
}

int diff() {
	printf("%s\n", "[DIFF]");
	return command(DIFF);
}