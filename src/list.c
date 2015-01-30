#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "colorcon.h"

char * prepare_path(char **str) {
	char * pch;
	pch = *str;
	if (pch = strstr(*str, "/src/"))
		*pch = '\0';
	return pch; 
}

void release(array * arr) {	
	array * a = arr;
	array * next = NULL;
	for (; a != NULL; a = next) {
		next = a->next;
		free(a->project_path);
		free(a->jar_path);
		free(a->deploy_path);
		free(a);
	}
}

array * put(char * project, char * jar) {
	array * arr = malloc(sizeof(array));
	arr->project_path = (project != NULL) ? strdup(project) : NULL;
	arr->jar_path = (jar != NULL) ? strdup(jar) : NULL;
	arr->next = NULL;
	arr->need_to_build = 1;
	return arr;
}

void display (array * start) {
	array * next = start;
	while (next != NULL) {
		printf("%s\n", next->project_path);
		next = next->next;
	}
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
	printf("\t%s", "deploy_path...");	
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
		out_yellow("\t\tSKIP_JAR: DON'T NEED TO BUILD.\n");	
		return 0;
	} else {
		project->deploy_path = path;
		out_green("\t\tOK: ");
		out_green(path);
		out_green("\n");
	}
	return 1;
}

int jar_name(FILE * file, array * project) {
	printf("\t%s", "jar file name...");
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
	printf("\t%s", "check branch...");	
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

int create_build_path(array * project) {
	printf("\n%s:\n", project->project_path);	
	FILE * file;
	char gradle_file[256];
	strcpy(gradle_file, project->project_path);
	strcat(gradle_file, "/build.gradle");
	printf("\t%s", "build.gradle file...");
	if(file = fopen(gradle_file, "r")) {
		out_green("\tOK.\n");
		if (project->need_to_build)
			project->need_to_build = check_branch(file);
		rewind(file);
		if (project->need_to_build)
			project->need_to_build = deploy_path(file, project);
		rewind(file);
		if (project->need_to_build)
			project->need_to_build = jar_name(file, project);

		fclose(file);
	} else {
		out_red("\tFAIL: NOT FOUND\n");
		project->need_to_build = 0;
		return 0;
	}	
	return 1;
}

int prepare_build (array * start) {
	array * next = start;
	while (next != NULL) {
		create_build_path(next);
		next = next->next;
	}
}

array * gen_diff_inf(FILE * file) {
	array * parent = NULL;
	array * next = NULL;
	array * i = NULL;

	char buffer[256];
	while(fgets(buffer, 256, file)) {	
		char *c = buffer;
		if (prepare_path(&c)) {			
			next = put(c, NULL);				
			if (parent == NULL)
				parent = next;
			if (i != NULL) {				
				if (strcmp(i->project_path, next->project_path) != 0) {
					i->next = next;
					i = next;
				}
			} else {
				i = next;
			}
		}
	}
	prepare_build(parent);
	//display(parent);
	fclose(file);	
	return parent;
}