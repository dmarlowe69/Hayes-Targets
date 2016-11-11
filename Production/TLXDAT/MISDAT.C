#include <stdio.h>
#include <string.h>

void main(argc, argv)
int argc;
char *argv[];
{
static char misdat[] = "missle";
static char cp_in_0[] = "cp ";
static char cp_in_1[] = " for005.dat";
static char cp_out[] = "cp for006.dat ";
static char output_ex[] = ".dat";
static char input_ex[] = ".i";
static char buffer_1[81];
static char buffer_2[81];

	if(argc == 1) {
		printf("syntax: misdat filename\n");
		return;
		}

	strcat(buffer_1,cp_in_0);
	strcat(buffer_1,argv[1]);
	strcat(buffer_1,input_ex);
	strcat(buffer_1,cp_in_1);
	printf("%s\n",buffer_1);
	system(buffer_1);

	printf("%s\n",misdat);
	system(misdat);

	strcat(buffer_2,cp_out);
	strcat(buffer_2,argv[1]);
	strcat(buffer_2,output_ex);
	printf("%s\n",buffer_2);
	system(buffer_2);
}
