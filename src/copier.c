#include <stdio.h>
#include <unistd.h>
#include <string.h>

int do_cmd(char * command) 
{
	system(command);
    return 0;
}

char *trimStr(char *str) {
	char *end;
	while(isspace(*str)) 
		str++;
	if(*str == 0)
		return str;
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) 
		end--;
	*(end+1) = 0;
	return str;
}

void for_build(char * s, char * d) {

   	char buffer[1024], dir[1024];
   	char * c;

   	strcpy(buffer, "mkdir -p ");
   	strcpy(dir, d);
   	c = strrchr(dir, '/');
   	*c = '\0';
   	strcat(buffer, dir);
   	printf("%s\n", buffer);
   	do_cmd(buffer);

   	strcpy(buffer, "cp -rf ");
   	strcat(buffer, s);
   	strcat(buffer, " ");
   	strcat(buffer, dir);
   	printf("%s\n", buffer);
   	do_cmd(buffer);
   	/*
   	strcat(buffer, name);
   	char * c;
   	c = strrchr(buffer, '/');
   	*c = '\0';
   	do_cmd(buffer);
   	fprintf(stderr, "%s\n", buffer);

   	strcpy(buffer, "cp -rf ");
   	strcat(buffer, s);
   	strcat(buffer, "/");
   	strcat(buffer, name);
   	strcat(buffer, " ");
   	strcat(buffer, d);
   	strcat(buffer, "/");
   	strcat(buffer, name);
   	do_cmd(buffer);
   	fprintf(stderr, "%s\n", buffer);
*/
   	
}

void trim (char * buffer, char * a, char * b) {
	char * ch;
	a = buffer;
	ch = strchr(buffer, ':'); 
	*ch = '\0';
	b = ch + 1;
	//printf("%s - %s\n", a, b);
	for_build(trimStr(a), trimStr(b));
}

int main(int argc, char *argv[]) {

	char from[512], to[512], name[512], buffer[1024];
	while(fgets(buffer, 1024, stdin)) {
		trim(buffer, from, to);		
	}	
	return 0;
}