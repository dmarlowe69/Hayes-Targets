/* file: ground.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Ground Test Functions")
 #pragma subtitle("(c)1991 Hayes Targets")

/****************************************************************************
 *
 *	BIRD Ground test functions
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
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
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *      Include BIRD definitions
 *
 ***************************************************************************/

 #include	"config.h"

 #include	"scu.h"
 #include	"uart.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	function pointers
 *
 ***************************************************************************/

 void (*com_open_ptr)(unsigned baud_rate);	/* initialize UART */
 void (*com_close_ptr)(void);				/* close out uart */
 int (*com_check_ptr)(void);				/* check for character */
 int (*com_getc_ptr)(void);					/* get charcter */
 int (*com_poll_ptr)(void);					/* wait for character */
 void (*com_flush_ptr)(void);				/* flush characters from receive buffer */
 void (*com_putc_ptr)(int ch);				/* output character to UART */
 void (*com_gets_ptr)(char *str);			/* input string from receive buffer */
 void (*com_puts_ptr)(char *str);			/* output string to UART */

/****************************************************************************
 *
 *      Ground function prototypes
 *
 ***************************************************************************/

 void init_gnd(int port);	/* setup required comm port */

/***************************************************************************
 *
 *      general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

 #pragma page(1)

/****************************************************************************
 *
 *	Ground Comm Port Initialization
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 void init_gnd(int port)
 {

 #if defined(V40)

 #pragma message("Using V40 comm_ports 0 or 1")

	if(port == 0) {
		com_open_ptr = scu_open;
		com_close_ptr = scu_close;
		com_check_ptr = scu_check;
		com_getc_ptr = scu_getc;
		com_poll_ptr = scu_poll;
		com_flush_ptr = scu_flush;
		com_putc_ptr = scu_putc;
		com_gets_ptr = scu_gets;
		com_puts_ptr = scu_puts;
		}
	else
		{
		com_open_ptr = uart_open;
		com_close_ptr = uart_close;
		com_check_ptr = uart_check;
		com_getc_ptr = uart_getc;
		com_poll_ptr = uart_poll;
		com_flush_ptr = uart_flush;
		com_putc_ptr = uart_putc;
		com_gets_ptr = uart_gets;
		com_puts_ptr = uart_puts;
		}
 #else

 #pragma message("Using SCU comm_port")

	com_open_ptr = scu_open;
	com_close_ptr = scu_close;
	com_check_ptr = scu_check;
	com_getc_ptr = scu_getc;
	com_poll_ptr = scu_poll;
	com_flush_ptr = scu_flush;
	com_putc_ptr = scu_putc;
	com_gets_ptr = scu_gets;
	com_puts_ptr = scu_puts;

 #endif

 }
