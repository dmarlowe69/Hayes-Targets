/* file: menu_gnd.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Ground Test Functions")
 #pragma subtitle("(c)1991-1993 Hayes Targets")

/****************************************************************************
 *
 *	BIRD Ground test functions
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
 #include	<ctype.h>

/****************************************************************************
 *
 *	Include BIRD definitions
 *
 ***************************************************************************/

 #include	<hayes.h>

 #include	"ground.h"
 #include	"menu_utl.h"

 #include	"MENU_RLC.H"
 #include	"MENU_ANA.H"
 #include	"MENU_SBX.H"
 #include	"TERMINAL.H"
 #include	"MENU_RTC.H"
 #include	"MENU_TM.H"
 #include	"MENU_STA.H"
 #include	"MENU_RST.H"
 #include   "MENU_PIO.H"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Ground function prototypes
 *
 ***************************************************************************/

 void gnd_menu(void);		/* UART command processor */

 #pragma page(1)

/****************************************************************************
 *
 *	Ground functions menu
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void gnd_menu(void)
 {
	int c;

	while(TRUE)
	{
		com_puts("\r\n\r\nGROUND FUNCTION MENU\r\n");

 #ifndef PC_CONSOLE
		com_puts("\r\n0. Terminal Loop Back\r\n");
 #endif

 #ifdef V45
		com_puts("\r\n1. Real Time Clock Menu\r\n");
 #endif
		com_puts("\r\n2. Analog Menu\r\n");
		com_puts("\r\n3. Relay Menu\r\n");
		com_puts("\r\n4. Status Menu\r\n");

 #ifdef V40
        com_puts("\r\n5. Toggle Status LED\r\n");
 #else
        com_puts("\r\n5. DSPIO Menu\r\n");
 #endif

 #ifndef PC
		com_puts("\r\n6. C_THRU_ROM\r\n");
 #endif

		com_puts("\r\n7. Remtron Menu\r\n");

 #ifndef PC
		com_puts("\r\n8. Reset Menu\r\n");
 #endif

		com_puts("\r\n9. Return\r\n");

		com_puts("\r\nA. LPM-SBX Menu\r\n");

		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(toupper(c))
		{

 #ifndef PC_CONSOLE
			case '0':
					term();
					break;
 #endif

 #ifdef V45
			case '1':
					rtc_menu();
					break;
 #endif

			case '2':
					analog_menu();
					break;

			case '3':
					rlc_menu();
					break;

			case '4':
					status_menu();
					break;

 #ifdef V40
			case '5':
					led_toggle();
                    break;
 #else
            case '5':
                    pio_menu();
                    break;
 #endif

 #ifndef PC
			case '6':
					C_THRU_ROM();
					break;
 #endif

			case '7':
					cmd_tm_menu();
					break;

 #ifndef PC
			case '8':
					reset_menu();
					break;
 #endif

			case '9':
					return;

			case 'A':
					lpm_sbx_menu();
					break;

			default:
					break;
		}
	}
 }
