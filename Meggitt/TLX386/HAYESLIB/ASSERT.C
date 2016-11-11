/*
 * Assert for The ROM environment.  
 */

void _assert(char *e, char *file, unsigned line)
{
	printf("Assertion Failure: '%s' on line %d, in file %s\n", 
				e, line, file);
	exit(99);
}
