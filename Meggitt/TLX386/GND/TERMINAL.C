/* file: terminal.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Ground Test Functions")
 #pragma subtitle("(c)1991-1993 Hayes Targets")

/****************************************************************************
 *
 *	BIRD Ground test functions
 *
 *	 written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *	Include BIRD definitions
 *
 ***************************************************************************/

 #include	<hayes.h>

 #include	"ground.h"
 #include	"menu_utl.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	COMM Port test function prototypes
 *
 ***************************************************************************/

 void term(void);				/* terminal loop-back test */
 void comm_port(void);			/* test comm ports for access */
 int com_break(void);			/* test comm ports for access */

 #pragma page(1)

/****************************************************************************
 *
 *	TERMINAL LOOP BACK TEST routine
 *
 *	Echo back anything typed, until escape is pressed
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

 void term(void)
 {
	int c;

	com_puts("\r\n\r\nTerminal mode selected\r\n\r\n");
	while(TRUE)
	{
		c = com_getc();         /* get character from the buffer */
		if(c == 0x1b)           /* check for escape key */
			break;
		com_putc(c);            /* Echo the character back out the serial port */

        if(c == '\r') {         /* check for enter key */
			com_putc('\n');

            }
	}
	com_puts("\r\nESCAPE key recognized Terminal mode aborted\r\n\r\n");
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Comm port break
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 int com_break(void)
 {
	if(com_check()!=ERR) {
		if(com_getc()==27)
			return(ERR);
		}

	return(OK);
 }
