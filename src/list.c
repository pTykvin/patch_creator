#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "colorcon.h"

char * prepare_path(char **str) {
	char * pch;
	pch = strrchr(*str,'\t');
	*str += pch-*str+1;
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
		free(a);
	}
}

array * put(char * project, char * jar) {
	array * arr = malloc(sizeof(array));
	arr->project_path = (project != NULL) ? strdup(project) : NULL;
	arr->jar_path = (jar != NULL) ? strdup(jar) : NULL;
	arr->next = NULL;
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

	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0)  // All spaces?
	return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
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

int deploy_path(FILE * file) {
	printf("\t%s", "deploy_path...");	
	char buffer[256];
	char * deploy_path = NULL;
	int into_packaging = 0;
	while (fgets(buffer, 256, file)) {			
		if (strstr(buffer, "packaging"))
			into_packaging = 1;		
		if (into_packaging) {
			if (strstr(buffer, "}")) 
				into_packaging = 0;	
			if (strstr(buffer, "deployPath")) {
				deploy_path = get_value(strdup(buffer));
				break;
			}
		}
	}	
	if (deploy_path == NULL) {
		out_red("FAIL: CAN'T FIND DEPLOY PATH.\n");	
		return 0;
	} else {
		out_green("OK: ");
		out_green(deploy_path);
		out_green("\n");
	}
	return 1;
}

int jar_name(FILE * file, char * project_path) {
	printf("\t%s", "jar file name...");
	char buffer[256];
	char * jar_name = NULL;
	while (fgets(buffer, 256, file)) {			
		if (strstr(buffer, "jar.archiveName")) {
			jar_name = get_value(strdup(buffer));
			break;
		}
	}
	if (jar_name == NULL) 
		jar_name = jar_name_by_path(project_path);
	if (jar_name == NULL) {
		out_red("FAIL: CAN'T GET NAME.\n");	
		return 0;
	} else {
		out_green("OK: ");
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
	while (fgets(buffer, 256, file)) {			
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
		out_green("OK: ");	
		if (branch) 
			out_green(branch);
		else 
			out_green("NULL");
		out_green("\n");				
		return 1;
	} else {
		out_red("FAIL: CAN'T FIND BRANCH NAME\n");
		return 0;
	}
}

int create_build_path(array * project) {
	printf("\n%s\n", project->project_path);	
	FILE * file;
	char gradle_file[256];
	strcpy(gradle_file, project->project_path);
	strcat(gradle_file, "/build.gradle");
	printf("\t%s", "build.gradle file...");
	if(file = fopen(gradle_file, "r")) {
		out_green("OK.\n");
		if (check_branch(file)) {
			if (!jar_name(file, project->project_path) || !deploy_path(file)) {
				fclose(file);
				return 0;
			}
		} else {
			return 0;
		}
		fclose(file);		
	} else {
		out_red("NOT FOUND\n");
		return 0;
	}	
	return 1;
}

int prepare_build(array ** parent_link) {
	array * next = *parent_link;
	array * prev = NULL;
	while (next != NULL) {
		if (create_build_path(next)) {
			prev = next;
			next = next->next;
		} else {
			if (prev == NULL) {
				*parent_link = next->next;
			} else {
				prev->next = next->next;				
			}
			next = next->next;
		}
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
	prepare_build(&parent);
	//display(parent);
	fclose(file);	
	return parent;
}