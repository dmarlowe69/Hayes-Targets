/* file: menu_utl.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Ground Test Functions")
 #pragma subtitle("(c)1991-1993 Hayes Targets")

/****************************************************************************
 *
 *	Ground test functions
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include BIRD definitions
 *
 ***************************************************************************/

 #include	<hayes.h>

 #include	"ground.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	General Serial utilites function prototypes
 *
 ***************************************************************************/

 void int_to_binary(int num,char *str); 	/* 8 bit integer to binary */
 void int_to_binary12(int num,char *str);	/* 12 bit integer to binary */
 void new_line(void);						/* CRLF */
 void pause(void);							/* press any key */

 #pragma page(1)

/****************************************************************************
 *
 *	binary display routine
 *
 *	This function prints an 8 bit ascii binary representation of it's
 *	argument to the serial port.
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void int_to_binary(int num,char *str)
 {
	int x;
	int mask;

	mask = 0x80;
	for(x = 0; x <=7; x++)
		{
		if(num & mask)
			*str++ = '1';
		else
			*str++ = '0';
		mask = mask >> 1;
		}

	*str = '\0';
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Binary display routine
 *
 *	This function prints an 12 bit ascii binary representation of it's
 *	argument to the serial port.
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void int_to_binary12(int num,char *str)
 {
	int x;
	int mask;

	mask = 0x800;
	for(x = 0; x <=11; x++)
		{
		if(num & mask)
			*str++ = '1';
		else
			*str++ = '0';
		mask = mask >> 1;
		}

	*str = '\0';
 }

 #pragma page(1)

/****************************************************************************
 *
 *	New Line routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void new_line(void)
 {
	com_puts("\r\n");
 }

 #pragma page(1)

/****************************************************************************
 *
 *	PAUSE routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void pause(void)
 {
	char c;

	com_puts("\r\npress any key");

	c = com_getc();	/* get a character from the serial port */

	new_line();
 }
