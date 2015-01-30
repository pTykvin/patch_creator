#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "colorcon.h"

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

void out (array * start) {
	array * next = start;
	while (next != NULL) {
		printf("%s\n", next->project_path);
		out_white("\t");
		out_white(next->project_path);
		out_white("\n");
		next = next->next;
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