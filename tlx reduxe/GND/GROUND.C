/* file: ground.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Ground Test Functions")
 #pragma subtitle("(c)1991 Hayes Targets")

/****************************************************************************
 *
 *      BIRD Ground test functions
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
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

 #include	<hayes.h>

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

 #ifdef I486_8250
 void (*com_open_ptr)(unsigned baud_rate, unsigned parms,unsigned port);
 #else
 void (*com_open_ptr)(unsigned baud_rate);	/* initialize UART */
 #endif
 void (*com_close_ptr)(void);			/* close out uart */
 int (*com_check_ptr)(void);			/* check for character */
 int (*com_getc_ptr)(void);			/* get charcter */
 int (*com_poll_ptr)(void);			/* wait for character */
 void (*com_flush_ptr)(void);			/* flush characters from receive buffer */
 #ifdef I486_8250
 void (*com_putc_ptr)(char ch);			/* output character to UART */
 #else
 void (*com_putc_ptr)(int ch);			/* output character to UART */
 #endif
 void (*com_gets_ptr)(char *str);		/* input string from receive buffer */
 void (*com_puts_ptr)(char *str);		/* output string to UART */

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

 #if defined(I486_8250)

 #pragma message("Using 8250 comm_port 0")

	com_open_ptr = comm_open;
	com_close_ptr = comm_close;
	com_check_ptr = comm_check;
	com_getc_ptr = comm_getc;
	com_poll_ptr = comm_poll;
	com_flush_ptr = comm_flush;
	com_putc_ptr = comm_putc;
	com_gets_ptr = comm_gets;
	com_puts_ptr = comm_puts;

 #elif defined(I486)

 #pragma message("Using DSPIO comm_ports 0 or 1")

	if(port == 0) {
		com_open_ptr = dspio_1_open;
		com_close_ptr = dspio_1_close;
		com_check_ptr = dspio_1_check;
		com_getc_ptr = dspio_1_getc;
		com_poll_ptr = dspio_1_poll;
		com_flush_ptr = dspio_1_flush;
		com_putc_ptr = dspio_1_putc;
		com_gets_ptr = dspio_1_gets;
		com_puts_ptr = dspio_1_puts;
		}
	else
		{
		com_open_ptr = dspio_2_open;
		com_close_ptr = dspio_2_close;
		com_check_ptr = dspio_2_check;
		com_getc_ptr = dspio_2_getc;
		com_poll_ptr = dspio_2_poll;
		com_flush_ptr = dspio_2_flush;
		com_putc_ptr = dspio_2_putc;
		com_gets_ptr = dspio_2_gets;
		com_puts_ptr = dspio_2_puts;
		}

 #elif defined(PC)

 #pragma message("Using DSPIO comm_ports 0 or 1")

	if(port == 0) {
		com_open_ptr = dspio_1_open;
		com_close_ptr = dspio_1_close;
		com_check_ptr = dspio_1_check;
		com_getc_ptr = dspio_1_getc;
		com_poll_ptr = dspio_1_poll;
		com_flush_ptr = dspio_1_flush;
		com_putc_ptr = dspio_1_putc;
		com_gets_ptr = dspio_1_gets;
		com_puts_ptr = dspio_1_puts;
		}
	else
		{
		com_open_ptr = dspio_2_open;
		com_close_ptr = dspio_2_close;
		com_check_ptr = dspio_2_check;
		com_getc_ptr = dspio_2_getc;
		com_poll_ptr = dspio_2_poll;
		com_flush_ptr = dspio_2_flush;
		com_putc_ptr = dspio_2_putc;
		com_gets_ptr = dspio_2_gets;
		com_puts_ptr = dspio_2_puts;
		}

 #endif

 }
