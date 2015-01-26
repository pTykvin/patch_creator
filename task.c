#include <stdio.h>
#include "hashtable.h"

typedef struct task
{
	char *project;
	char *dest;
	struct task *next;
} task;

struct task[256];

void display(task *t) {
	for (;t != NULL;t = t->next) {
		printf("%s %s\n", t->project, t->dest);
	}
}

int main(int argc, char const *argv[])
{
	task task1 = {"project1", "dest1", NULL};
	task task2 = {"project2", "dest2", NULL};
	task task3 = {"project3", "dest3", NULL};

	task1.next = &task2;
	task2.next = &task3;

	display(&task1);

	return 0;
}