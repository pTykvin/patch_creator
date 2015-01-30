#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void out_red(char * string) {
	fprintf(stderr, "%s%s%s",ANSI_COLOR_RED, string, ANSI_COLOR_RESET);
}

void out_green(char * string) {
	fprintf(stderr, "%s%s%s",ANSI_COLOR_GREEN, string, ANSI_COLOR_RESET);
}

void out_yellow(char * string) {
	fprintf(stderr, "%s%s%s",ANSI_COLOR_YELLOW, string, ANSI_COLOR_RESET);
}

void out_blue(char * string) {
	fprintf(stderr, "%s%s%s",ANSI_COLOR_BLUE, string, ANSI_COLOR_RESET);
}

void out_magenta(char * string) {
	fprintf(stderr, "%s%s%s",ANSI_COLOR_MAGENTA, string, ANSI_COLOR_RESET);
}

void out_cyan(char * string) {
	fprintf(stderr, "%s%s%s",ANSI_COLOR_CYAN, string, ANSI_COLOR_RESET);
}