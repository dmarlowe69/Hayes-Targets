/* file: uart_cmd.c */

/****************************************************************************
 *
 *	Uart CMD functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/90	D.marlowe	Original release
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

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	<hayes.h>
 #include	<gnd.h>

 #include	"cmd_tm.h"
 #include	"monloop.h"

/****************************************************************************
 *
 *	defintions
 *
 ***************************************************************************/

 #define ENABLE 	0
 #define DISABLE	1
 #define USE		2
 #define DISUSE 	3
 #define NODATA 	4

/****************************************************************************
 *
 *	Ground function prototypes
 *
 ***************************************************************************/

 void uart_cmd_initialize(void);
 void uart_cmd_de_initialize(void);
 int uart_cmd(void);
 int uart_cmd_port(void);

 void uart_port(void);
 void uart_menu(void);

 #pragma page(1)

/****************************************************************************
 *
 *	UART ground test function
 *
 ***************************************************************************/
 void uart_cmd_initialize(void)
 {
	int_disable(CMD_IRQ);	/* disable command decoder */

	cmd.frame_count = 8;	/* initialize frame counter */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	UART ground test function
 *
 ***************************************************************************/
 void uart_cmd_de_initialize(void)
 {
	uart_puts("\r\nUart CMD Port Disabled\r\n");

	uart_flush();

	cmd.frame_count = 0;	/* initialize frame counter */

	int_enable(CMD_IRQ);	/* renable RTD1 interrupt */

	cmd_data.uart_cmd = DISABLE;
 }

 #pragma page(1)

/****************************************************************************
 *
 *	UART ground test function
 *
 ***************************************************************************/
 int uart_cmd(void)
 {
 int status;
 int c;

	if(uart_check() != ERR) {
		status = monloop();
		if(status == ERR_EXIT) {
			uart_cmd_de_initialize();
			return(DISUSE);
			}
		return(OK);
		}

	return(NODATA);
 }

 #pragma page(1)

/***************************************************************************
 *
 *	terminal break in
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 int uart_cmd_port(void)
 {
	if(uart_check()!=ERR) {			/* character waiting at port */
		if(uart_getc()==0x1b) {		/* must be an escape character */
			uart_puts("\r\nUart CMD Port USEable\r\n");
			return USE;				/* call ground test menu */
		}
	}

	return DISABLE;					/* call ground test menu */
 }

 #pragma page(1)

/***************************************************************************
 *
 *	terminal break in
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void uart_port(void)
 {
	if(uart_check()!=ERR)			 /* character waiting at port */
		if(uart_getc()==0x1b)		/* must be an escape character */
			uart_menu();			/* call ground test menu */

 }

 #pragma page(1)

/***************************************************************************
 *
 *	terminal break in
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void uart_menu(void)
 {
	int c;

    uart_puts("\r\nTLX Bird Brain Version 2.00\r\n");

	uart_flush();			/* call ground test menu */

	while(TRUE) {

		uart_puts("\r\n\r\nUart FUNCUTIONS\r\n");

		uart_puts("\r\n0. **** COLD BOOT ****\r\n");
		uart_puts("\r\n1. **** WARM BOOT ****\r\n");
		uart_puts("\r\n2. **** KERNEL BOOT ****\r\n");
		uart_puts("\r\n9. exit\r\n");
		uart_puts("\r\n	  Make Selection ");

		c = uart_getc();		/* get a character from the serial port */

		switch(toupper(c)) {

			case '0':
					uart_puts("\r\n RESETTING... \r\n");
					int_disable(0);         /* disable interrutps */
					int_disable(1);
					int_disable(2);
					int_disable(3);
					int_disable(4);
					int_disable(5);
					int_disable(6);
					int_disable(7);
					delay(32000);
					reset_boot();
					break;

			case '1':
					uart_puts("\r\n RESETTING... \r\n");
					int_disable(0);         /* disable interrutps */
					int_disable(1);
					int_disable(2);
					int_disable(3);
					int_disable(4);
					int_disable(5);
					int_disable(6);
					int_disable(7);
					delay(32000);
					_astart();
					break;

			case '2':
					init_systems(); 		/* set hardware configuration */
					uart_close();			/* close out serial port */
					C_THRU_ROM();			/* call C THRU ROM kernel */
					return; 				/* never really returns */
					break;

			case '9':
					return;
					break;

			default:
					break;
		}
	}
 }
