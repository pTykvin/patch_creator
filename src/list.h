typedef struct array{
	char * project_path;
	char * jar_path;
	char * deploy_path;	
	struct array * next;
	unsigned int need_to_build:1;
} array;

void out (array * start);
array * put(char * project, char * jar);
void release(array * arr);
