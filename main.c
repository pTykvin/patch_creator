#include <stdio.h>
#include <unistd.h>
#include "command.h"

int main(int argc,char *argv[]){    
	const char *repo = "repo";
	const char *git = "repo/.git";

	if (access(git, F_OK) == -1) {
		clone();
	} else {
		printf("%s\n", "Repository find...");
	}
	printf("%s\n", "Change working directory to 'repo'...");
	chdir(repo);
	pull();
	diff();
}