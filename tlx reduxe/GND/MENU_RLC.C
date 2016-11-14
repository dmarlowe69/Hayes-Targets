/* file: menu_rlc.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Ground Test Functions")
 #pragma subtitle("(c)1991 Hayes Targets")

/****************************************************************************
 *
 *      Ground test functions
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
 #include       <dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *      Include BIRD definitions
 *
 ***************************************************************************/

 #include	<hayes.h>

 #include	"ground.h"
 #include	"menu_utl.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	RLC serial interface function prototypes
 *
 ***************************************************************************/

 void rlc_menu(void);
 void rlc_status(void);
 void rlc_test(void);
 void rlc_report(int status);
 void rlc_config_display(void);
 void rlc_sequence_test(void);

 #pragma page(1)

/****************************************************************************
 *
 *	RLC functions
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void rlc_menu(void)
 {
	int c;

	init_rlc();

	rlc_config_display();

	while(TRUE)
	{
		com_puts("\r\n\r\nRLC FUNCUTIONS\r\n");

		com_puts("\r\n1. RLC status\r\n");
		com_puts("\r\n2. Test Relay\r\n");
		com_puts("\r\n3. Sequence Relays\r\n");
		com_puts("\r\n9. exit\r\n");
		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(c)
		{
			case '1':
					rlc_status();
					break;

			case '2':
					rlc_test();
					break;

			case '3':
					rlc_sequence_test();
					break;

			case '9':
					return;

			default:
					break;
		}
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	RLC functions
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void rlc_status(void)
 {
	char temp[80];
	char buffer[80];
	int status;

	status = read_relay();
	int_to_binary12(status,temp);
	sprintf(buffer,"RLC Status %s\r\n",temp);
	com_puts(buffer);
 }

 #pragma page(1)

/****************************************************************************
 *
 *	RLC functions
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void rlc_test(void)
 {
	char temp[80];
	int status;
	int relay;

	com_puts("\r\n\nSelect Relay Channel (0..7) ");
	com_gets(temp);        /* get a string from the serial port */
	relay = atoi(temp) & 0x07;

	reset_relay(relay);
	status = test_relay(relay);
	rlc_report(status);
	delay(32000);

	set_relay(relay);
	status = test_relay(relay);
	rlc_report(status);
	delay(32000);

	toggle_relay(relay);
	status = test_relay(relay);
	rlc_report(status);
 }

 #pragma page(1)

/****************************************************************************
 *
 *	RLC functions
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void rlc_report(int status)
 {
	switch(status)
	{
		case RELAY_RESET :	com_puts("\r\nrelay reset\r\n");
					break;
		case RELAY_SET :	com_puts("\r\nrelay set\r\n");
					break;
		case RELAY_ERROR :	com_puts("\r\nrelay error\r\n");
					break;
		default:		com_puts("\r\nUNKNOWN CODE\r\n");
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Display RLC Configuration Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void rlc_config_display(void)
 {
	char temp[80];
	int i;
	unsigned int rlc_addr,relay;

	sprintf(temp,"base address: 0x%4x\r\n",rlc.rlc_addr);
	com_puts(temp);

	pause();		/* wait for key press */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Relay Srquence Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void rlc_sequence_test(void)
 {
	char temp[80];
	int relay;
	int repeat;

	for(repeat=0;repeat<16;repeat++) {

	//multi_delay(512);
	delay(32000);

	for(relay=0;relay<8;relay++) {

			sprintf(temp,"relay %d\r\n",relay);
			com_puts(temp);

			//multi_delay(256);
			delay(32000);
			toggle_relay(relay);		/* toggle RLC relay */
			//multi_delay(256);
			delay(32000);

			//test_relay(relay);		/* return state of relay	*/
			//set_relay(relay);			/* set RLC relay */
			//reset_relay(relay);		/* reset RLC relay */

			//write_relay(relay);	/* update relays */

			//read_relay();			/* read relays */

			//multi_delay(256);
			delay(32000);
		}
	}

 }
