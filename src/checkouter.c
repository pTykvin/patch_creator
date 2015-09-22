#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "colorcon.h"

void checkout(char * checkout) {
	char cmd[255];
	strcpy(cmd, "git fetch");
	system(cmd);
	strcpy(cmd, "git checkout ");
	strcat(cmd, checkout);
	system(cmd);
}

int main(int argc, char *argv[])
{	
	
	char ch;
	char * reqCheckout = NULL;

	while ((ch = getopt(argc, argv, "c:")) != EOF)
		switch (ch) {
			case 'c':
				reqCheckout = optarg;
			break;			
			default:
				out_red("Неизвестный параметр: ");
				out_red(optarg);
				out_red("\n");
				return 1;
		}
		out_blue("[CHECKOUTER START]\n"); 
		if (reqCheckout == NULL) {
			out_red("Укажите обязательный ключ -c <checkout>\n");
			return 9;
		}

		checkout(reqCheckout);
		out_blue("[CHECKOUTER FINISH]\n"); 
	return 0;
}
