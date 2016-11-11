/* file: menu_rst.c */

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

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 7.00
 *	MASM Version 6.0
 *	Datalight C_Thru_ROM Version 2.0c
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
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
 *	Include definitions
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
 *	hardware reset functions
 *
 ***************************************************************************/

 void reset_menu(void);                 /* reset functions */

 #pragma page(1)

/****************************************************************************
 *
 *	reset functions
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void reset_menu(void)
 {
	int c;

	while(TRUE)
	{
		com_puts("\r\n\r\nRESET FUNCUTIONS\r\n");

		com_puts("\r\n0. **** COLD BOOT ****\r\n");
		com_puts("\r\n1. toggle INT 0\r\n");
		com_puts("\r\n2. toggle INT 1\r\n");
		com_puts("\r\n3. toggle INT 2\r\n");
		com_puts("\r\n4. toggle INT 3\r\n");
		com_puts("\r\n5. toggle INT 4\r\n");
		com_puts("\r\n6. toggle INT 5\r\n");
		com_puts("\r\n7. toggle INT 6\r\n");
		com_puts("\r\n8. toggle INT 7\r\n");
		com_puts("\r\n9. exit\r\n");
		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(c)
		{
			case '0':
					com_puts("\r\n RESETTING... \r\n");
					int_disable_all();	 /* disable interrutps */
					delay(32000);
					reset_boot();
					break;

			case '1':
					if(int_toggle(0) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '2':
					if(int_toggle(1) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '3':
					if(int_toggle(2) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '4':
					if(int_toggle(3) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '5':
					if(int_toggle(4) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '6':
					if(int_toggle(5) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '7':
					if(int_toggle(6) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '8':
					if(int_toggle(7) == ENABLE)
						{
						com_puts("\r\nenabled\r\n");
						pause();
						}
					else
						{
						com_puts("\r\ndisabled\r\n");
						pause();
						}
					break;

			case '9':
					return;

			default:
					break;
		}
	}
 }
