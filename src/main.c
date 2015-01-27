#include <stdio.h>
#include <unistd.h>
#include "command.h"
#include "colorcon.h"
#include "global.h"
#include "list.h"
 
int main(void) {
	//global_init();
	//git_prepare();
	//gradle_build();
	chdir("repo");
    FILE* file = diff();    
	array * arr = gen_diff_inf(file);
/*
	char buffer[256];
    while (fgets(buffer, 256, file)) {
		printf("%s", buffer);
	}
*/
    printf("%s", "[DIFF READY]\n");    
	checkout();
}