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

int check_type(FILE * file) {
	out_white("\tcheck type...");
	char buffer[256];
	char * type = NULL;
	int into_packaging = 0;
	FILE * f = file;
	while (fgets(buffer, 256, f)) {				
		if (strstr(buffer, "packaging"))
			into_packaging = 1;	
		if (into_packaging) {
			if (strstr(buffer, "}")) 
				into_packaging = 0;				
			if (strstr(buffer, "type")) {
				type = trim(get_value(strdup(buffer)));
				break;
			}
		}
	}
	if (type == NULL) {
		out_green("\t\tOK: unknown\n");
		return 1;
	} else if (strstr(type, "module")) {
		out_green("\t\tOK: module\n");			
		return 1;
	} else if (strstr(type, "plugin")) {
		out_green("\t\tOK: plugin\n");			
		return 1;
	} else if (strstr(type, "common")) {
		out_green("\t\tOK: common\n");			
		return 1;
	}  else if (strstr(type, "ejb")) {
		out_yellow("\t\tSKIP: ejb\n");			
		return 0;
	} else if (strstr(type, "updater")) {
		out_green("\t\tOK: updater\n");			
		return 1;
	} else if (strstr(type, "library")) {
		out_green("\t\tOK: library\n");			
		return 1;
	} else if (strstr(type, "serverLib")) {
		out_yellow("\t\tSKIP: server lib\n");			
		return 0;
	} else {
		out_red("\t\t\tFAIL: DON'T CORRECT TYPE: ");
		out_red(type);
		out_red("\n");
		return -1;
	}
}

int check_deploy(FILE * file) {
	out_white("\tcheck deploy...");
	char buffer[256];
	char * deploy = NULL;
	int into_packaging = 0;
	FILE * f = file;
	while (fgets(buffer, 256, f)) {			
		if (strstr(buffer, "packaging"))
			into_packaging = 1;		
		if (into_packaging) {
			if (strstr(buffer, "}")) 
				into_packaging = 0;	
			if (strstr(buffer, "deployPath")) {
				deploy = trim(get_value(strdup(buffer)));
				break;
			}
		}
	}

	if (deploy == NULL || !strstr(deploy, "...skipJar")) {
		out_green("\t\tOK: ");	
		if (deploy) 
			out_green(deploy);
		else 
			out_green("NULL");
		out_green("\n");			
		return 1;
	} else {
		out_yellow("\t\tSKIP: ");
		out_yellow(deploy);
		out_yellow("\n");
		return 0;
	}
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

int configure_deployPath(char * deploy, array * project) {
	out_white("\tconfigure_deployPath...");	
	char command[512];		
	strcpy(command, "find ");
	strcat(command, deploy);
	strcat(command, " -name ");
	strcat(command, project->jar_path);
	int count = 0;
	FILE * cmd = popen(command, "r");
	if (cmd) {
		while (fgets(command, 256, cmd))
			count++;
		if (count == 1) {		
			project->deploy_path = command + strlen(deploy) + 1;
			char * ch;
			ch = project->deploy_path + strlen(project->deploy_path) - 1;
			*ch = '\0';
			out_green("\tOK: ");
			out_green(project->deploy_path);
			out_green("\n");	
			return 1;		
		} else {
			out_red("\tFAIL: ");
			if (count == 0) 
				out_red("FILE NOT FOUND\n");
			else
				out_red("TO MANY FILES FOUND\n");
			return 0;
		}
		out_white("\n");
	}
	out_red("FAIL: CAN'T EXECUTE ");
	out_red(command);
	out_red("\n");
	return 0;
}

int create_build_path(array * project, char * deploy, char * patch) {
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

		rewind(file);
		if (project->need_to_build)
			project->need_to_build = check_deploy(file);
		rewind(file);
		if (project->need_to_build)
			project->need_to_build = check_type(file);
		rewind(file);
		if (project->need_to_build)
			project->need_to_build = check_branch(file);
		rewind(file);
		if (project->need_to_build)
			project->need_to_build = jar_name(file, project);			

		if (project->need_to_build)
			project->need_to_build  = configure_deployPath(deploy, project);

		if (project->need_to_build) {
			char from[1024], to[1024];

			strcpy(from, deploy);
			strcat(from, "/");
			strcat(from, project->deploy_path);

			strcpy(to, patch);
			strcat(to, "/");
			strcat(to, project->deploy_path);			

			printf("%s:%s\n", from, to);
		}
		
		fclose(file);
	} else {
		out_red("\t\tFAIL: NOT FOUND\n");
		project->need_to_build = 0;
		return 0;
	}	
	return 1;
}

void prepare_build (array * start, char * deploy, char * patch) {
	array * next = start;
	while (next != NULL) {
		create_build_path(next, deploy, patch);		
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
		prepare_build(arr, deploy, patch);
		out_blue("[ANALIZER FINISH]\n"); 
	return 0;
}
