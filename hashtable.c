#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"

typedef struct array {
	char * project_path;
	char * jar_path;
	struct array * next;
} array;

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

int main()
{
	array * parent = NULL;
	array * next = NULL;
	array * i = NULL;

    FILE * file = fopen("1", "r");
	char buffer[256];
	while(fgets(buffer, 256, file)) {	
		char *c = buffer;
		if (prepare_path(&c)) {
			next = put(c, NULL);
			if (parent == NULL) {
				parent = next;
			}
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
	display(parent);
	fclose(file);
}