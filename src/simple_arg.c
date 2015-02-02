#include <stdio.h>
#include <unistd.h>

extern char * deploy;

int main(int argc, char *argv[])
{
	char * delivery = "";
	int thick = 0;
	int count = 0;
	char ch;

	while ((ch = getopt(argc, argv, "d:t")) != EOF)
		switch (ch) {
			case 'deploy':
				deploy = optarg;
			break;
			default:
				fprintf(stderr, "Неизвестный параметр: '%s'\n", optarg);
				return 1;
		}
	argc -= optind;
	argv += optind;

	if (thick)
		puts("Пышное тесто.");

	if (delivery[0])
		printf("Будет доставлено %s\n", delivery);

	for (count = 0; count < argc; count++)
		puts(argv[count]);

	return 0;
}
