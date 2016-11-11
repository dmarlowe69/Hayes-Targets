#include <stdio.h>
#include <string.h>

void main(argc, argv)
int argc;
char *argv[];
{
static char uplot_0[] = "uplot -D ";
static char uplot_1[] = " -T hp -M 7475 -P A ";
static char pgl[] = ".pgl";
static char plt[] = ".plt";
static char buffer[81];

	if(argc == 1) {
		printf("syntax: hplot filename\n");
		return;
		}

	strcat(buffer,uplot_0);

	strcat(buffer,argv[1]);

	strcat(buffer,pgl);

	strcat(buffer,uplot_1);

	strcat(buffer,argv[1]);

	strcat(buffer,plt);

	printf("%s\n",buffer);

	system(buffer);
}
