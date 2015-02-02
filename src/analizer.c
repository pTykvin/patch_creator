#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"
#include "colorcon.h"

array * load(FILE * input) {
	array * parent = NULL;
	array * next = NULL;

	char buffer[256];
	while(fgets(buffer, 256, input)) {			
		if (next != NULL) {
			next->next = put(buffer, NULL);
			next = next->next;
		} else {
			next = put(buffer, NULL);
		}
		if (parent == NULL)
			parent = next;		
	}
	return parent;
}

char *trim(char *str) {
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

char * get_value(char * string) {	
	char * i = string;
	int cnt = 0;
	while (cnt < 2 && *i != 0) {
		if ((*i == '\"')||(*i == '\'')) {
			switch (++cnt) {
				case 1:
					string = i + 1;
				break;
				case 2:
					*i = 0;
				break;
			}
		}
		i++;
	}
	return string;
}

char * jar_name_by_path(char * path) {
	char * result;
	while (*path != 0) {
		if (*path == '/') 
			result = path + 1;
		path++;
	}
	strcat(result, ".jar");
	return result;
}

int deploy_path(FILE * file, array * project) {
	out_white("\tdeploy_path...");		
	char buffer[256];
	char * path = NULL;
	int into_packaging = 0;
	FILE * f = file;
	while (fgets(buffer, 256, f)) {		
		if (strstr(buffer, "packaging"))
			into_packaging = 1;		
		if (into_packaging) {
			if (strstr(buffer, "}")) 
				into_packaging = 0;	
			if (strstr(buffer, "deployPath")) {				
				path = get_value(strdup(buffer));
				break;
			}
		}
	}	
	if (path == NULL) {
		out_red("\t\tFAIL: CAN'T FIND DEPLOY PATH.\n");	
		return 0;
	} else if (strstr("...skipJar", path)) {
		out_yellow("\t\t\tSKIP_JAR: DON'T NEED TO BUILD.\n");	
		return 0;
	} else {
		project->deploy_path = path;
		out_green("\t\t\tOK: ");
		out_green(path);
		out_green("\n");
	}
	return 1;
}

int jar_name(FILE * file, array * project) {
	out_white("\tjar file name...");	
	char buffer[256];
	char * jar_name = NULL;
	FILE * f = file;
	while (fgets(buffer, 256, f)) {		
		if (strstr(buffer, "jar.archiveName")) {
			jar_name = get_value(strdup(buffer));
			break;
		}
	}
	if (jar_name == NULL) 
		jar_name = jar_name_by_path(project->project_path);
	if (jar_name == NULL) {
		out_red("FAIL: CAN'T GET NAME.\n");	
		return 0;
	} else {
		project->jar_path = jar_name;
		out_green("\tOK: ");
		out_green(jar_name);
		out_green("\n");
	}
	return 1;
}

int check_branch(FILE * file) {
	out_white("\tcheck branch...");
	char buffer[256];
	char * branch = NULL;
	int into_packaging = 0;
	FILE * f = file;
	while (fgets(buffer, 256, f)) {			
		if (strstr(buffer, "packaging"))
			into_packaging = 1;		
		if (into_packaging) {
			if (strstr(buffer, "}")) 
				into_packaging = 0;	
			if (strstr(buffer, "branch")) {
				branch = trim(get_value(strdup(buffer)));
				break;
			}
		}
	}

	if (branch == NULL || strstr(branch, "SCO")) {
		out_green("\t\tOK: ");	
		if (branch) 
			out_green(branch);
		else 
			out_green("NULL");
		out_green("\n");			
		return 1;
	} else {
		out_red("\t\tFAIL: DON'T CORRECT BRANCH NAME:");
		out_red(branch);
		out_red("\n");
		return 0;
	}
}

void deleteEnter(char ** str) {
	if (*(*str + strlen(*str) - 1) == '\n')
		*(*str + strlen(*str) - 1) = '\0';	
}

int is_build_exists (array * project, char * deploy) {
	out_white("\tJar file exists...");
	char * result = malloc(strlen(deploy)+strlen("/")+strlen(project->deploy_path)+strlen("/")+strlen(project->jar_path)+1);
	strcpy(result, deploy);
	strcat(result, "/");
	strcat(result, project->deploy_path);
	strcat(result, "/");
	strcat(result, project->jar_path);
	if (access(result, F_OK) == -1) {
		out_red("\tNO.\n");
		return 0;	
	} else {
		out_green("\tYES.\n");
		return 1;	
	}
}

int create_build_path(array * project, char * deploy) {
	deleteEnter(&project->project_path);
	out_white("\n");
	out_white(project->project_path);
	out_white(":\n");
	FILE * file;
	char gradle_file[256];	
	strcpy(gradle_file, project->project_path);	
	strcat(gradle_file, "/build.gradle");
	out_white("\t");
	out_white("build.gradle...");
	if(file = fopen(gradle_file, "r")) {
		out_green("\t\tOK.\n");
		if (project->need_to_build)
			project->need_to_build = check_branch(file);
		rewind(file);
		if (project->need_to_build)
			project->need_to_build = deploy_path(file, project);
		rewind(file);
		if (project->need_to_build)
			project->need_to_build = jar_name(file, project);
		if (project->need_to_build)
			project->need_to_build = is_build_exists(project, deploy);		

		fclose(file);
	} else {
		out_red("\t\tFAIL: NOT FOUND\n");
		project->need_to_build = 0;
		return 0;
	}	
	return 1;
}

int prepare_build (array * start, char * deploy) {
	array * next = start;
	while (next != NULL) {
		create_build_path(next, deploy);
		next = next->next;
	}
}

char * get_path(array * project, char  * deploy, int need_name) {
	int len_name = (need_name) ? strlen(project->jar_path) : 0;
	char * d = malloc(strlen(deploy)+strlen("/")+strlen(project->deploy_path)+strlen("/")+len_name+1);
	strcpy(d, deploy);
	strcat(d, "/");
	strcat(d, project->deploy_path);
	strcat(d, "/");
	if (need_name)
		strcat(d, project->jar_path);
	return d;	
}

int copy_from_to(array * start, char  * deploy, char  * patch) {

	array * next = start;
	while (next != NULL) {
		if (next->need_to_build) {
			char * d = get_path(next, deploy, 1);
			char * p = get_path(next, patch, 0);
			printf("%s:%s\n", d, p);
		}
		next = next->next;
	}
}

int main(int argc, char *argv[])
{	
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
				out_red("Неизвестный параметр: ");
				out_red(optarg);
				out_red("\n");
				return 1;
		}
		array * arr = load(stdin);
		out_blue("[ANALIZER START]\n"); 
		if (deploy == NULL) {
			out_red("Укажите обязательный ключ -s <deploy_path>\n");
			return 9;
		}
		if (patch == NULL) {
			out_red("Укажите обязательный ключ -d <patch_path>\n");
			return 9;
		}		
		prepare_build(arr, deploy);
		copy_from_to(arr, deploy, patch);
		out_blue("[ANALIZER FINISH]\n"); 
	return 0;
}
