typedef struct array{
	char * project_path;
	char * jar_path;
	struct array * next;
} array;

array * gen_diff_inf(FILE * file);