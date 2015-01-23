#include <stdio.h>
#include <string.h>
#include "hashtable.h"

char check;

unsigned char hash(unsigned char *str)
{
    unsigned char hash = 81;
    char c;
    while (c = *str++)
        hash = ((hash << sizeof(char)) + hash) + c;
    return hash;
}

int prepare_path(char *str)
{
	char c;
	int result;
    while (c = *str++) {
    	if (c == ' ') {
    		result = c;
    	}
    }
    return &result;
}

int main()
{
    FILE* file = fopen("1", "r");
	char buffer[256], result;
	unsigned char _hash;
	while(fgets(buffer, 256, file)) {	
		result = *(prepare_path(buffer));
		printf("%i\n", result);
	}

	//char msg[32];
	//while (scanf("%s", msg))
	//	printf("%u\n", hash(msg));	
	return 0;
}