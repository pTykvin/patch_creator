#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "colorcon.h"
#include "list.h"


char * prepare_path(char **str) {
	char * pch;
	pch = *str;
	if (pch = strstr(*str, "/src/"))
		*pch = '\0';
	return pch; 
}

int streq(char * str1, char * str2)  {
	if (strlen(str1) == strlen(str2)) {
		int i = 0;
		int len = strlen(str1);
		for (; i<len; i++) {
			if (*(str1 + i) != *(str2 + i))
				return 0;
		}
		return 1;
	}
	return 0;
}

array * gen_diff_inf(FILE * input) {
	array * parent = NULL;
	array * next = NULL;
	array * i = NULL;

	char buffer[256];
	while(fgets(buffer, 256, input)) {			
		char *c = buffer;
		if (prepare_path(&c)) {						
			next = put(c, NULL);				
			if (parent == NULL)
				parent = next;
			if (i != NULL) {
				if (!streq(i->project_path, next->project_path)) {
					i->next = next;
					i = next;
				}
			} else {
				i = next;
			}
		}
	}
	return parent;
}

int main(int argc, char const *argv[])
{  
	out_blue("[DIFFERENCER START]\n"); 
	array * arr = gen_diff_inf(stdin);
	out(arr);
	out_blue("[DIFFERENCER FINISH]\n"); 
	release(arr);
}