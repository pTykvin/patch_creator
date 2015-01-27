#include <stdio.h>
#include <unistd.h>
#include "command.h"
#include "tmp_saver.h"
#include "global.h"

typedef enum {
	CLONE,
	FETCH,
	CHECKOUT,
	CLEAN,
	BUILD
} CMD;

int command(CMD cmd) 
{
	char *command;
	switch(cmd) {
		case CLONE:
			command = "git clone git@github.com:crystalservice/sco.git repo"; // git@github.com:pTykvin/ConsumingFormatter.git repo";
			tmp_save("repository", "git@github.com:crystalservice/sco.git");
		break;
		case FETCH:
			command = "git fetch";
		break;
		case CLEAN:
			command = "git clean -f";
		break;
		case BUILD:
			command = "gradle clean build -x test";
		break;		
		case CHECKOUT:
			command = "git checkout tag_v52_sco-sprint44";
			tmp_save("checkout", "tag_v52_sco-sprint44");
		break;

	}

    FILE* file = popen(command, "r");
	char buffer[256];
    while (fgets(buffer, 256, file)) {
		printf("%s", buffer);
    }
    printf("[%s READY]\n", command);
    pclose(file);
    return 0;
}

int fetch() {
	printf("%s\n", "[FETCH]");
	return command(FETCH);
}

int clone() {
	printf("%s\n", "[CLONE]");
	return command(CLONE);
}

int checkout() {
	printf("%s\n", "[CHECKOUT]");
	return command(CHECKOUT);
}

int gradle_build() {
	printf("%s\n", "[BUILD]");
	return command(BUILD);
}

int clean() {
	printf("%s\n", "[CLEAN]");
	return command(CLEAN);
}

FILE * diff() {
	printf("%s\n", "[DIFF]"); 
	return popen("git diff --raw tag_v52_sco-sprint43 tag_v52_sco-sprint44", "r");
}

int git_prepare() {
	char * repo = "repo";
	char * git = "repo/.git";
	if (access(git, F_OK) == -1) {
		clone();
	} else {
		printf("%s\n", "Repository find...");
	}
	printf("%s\n", "Change working directory to 'repo'...");
	chdir(repo);
	clean();
	fetch();	
}