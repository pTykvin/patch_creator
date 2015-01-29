#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "command.h"
#include "colorcon.h"
#include "global.h"
#include "list.h"
 
/*

typedef struct array{
	char * project_path;
	char * jar_path;
	struct array * next;
	unsigned int need_to_build:1;
} array;

*/

char * make_string(char * s1, char * s2, char * s3, char * s4) {

	char buffer[1024];
	char * result = buffer;
	int i;
	while (*s1 != '\0') {		
		*result = *s1;
		result++;
		s1++;
	}
	while (*s2 != '\0') {
		*result = *s2;
		result++;
		s2++;
	}
	while (*s3 != '\0') {
		*result = *s3;
		result++;
		s3++;
	}
	while (*s4 != '\0') {
		*result = *s4;
		result++;
		s4++;
	}
	*result = '\0';
	result = buffer;
	return result;
}

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

   	out_cyan("[COPY]");   	
   	out_blue("\n\tFROM ");
   	printf("%s", s);
   	out_blue("\n\tTO ");
	printf("%s\n", d);
	/*char * cmd;
	cmd = "mkdir -p ";
	strcat(cmd, dst);
	do_cmd(cmd);
	do_cmd(make_string("cp -rf ", src, " ", dst));*/
}

show_copy_from(array * start) {
	array * next = start;
	char * copy_path = "";
	char * src;
	char * dst;
	while(next != NULL) {
		if (next->need_to_build) {
			src = strdup(make_string(next->project_path, "/build/libs/", next->jar_path, "\0"));
			dst = strdup(make_string("../build/", next->deploy_path, "/", "\0"));	
			for_build(src, dst);
		}
		next = next->next;
	}
}

int main(void) {
	global_init();
	git_prepare();
	gradle_build();
	chdir("repo");
    FILE* file = diff();    
	array * arr = gen_diff_inf(file);
	out_blue("[DIFF READY]\n"); 
	show_copy_from(arr);
/*
	char buffer[256];
    while (fgets(buffer, 256, file)) {
		printf("%s", buffer);
	}
*/    
}