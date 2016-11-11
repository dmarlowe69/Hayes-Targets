/* file: menu_sta.c */

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

 #include	"ground.h"
 #include	"menu_utl.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	STA serial interface function prototypes
 *
 ***************************************************************************/

 void status_menu(void);
 void status_status(void);
 void status_test(void);
 void led_report(int status);
 void status_config_display(void);
 void status_sequence_test(void);

 #pragma page(1)

/****************************************************************************
 *
 *	STATUS functions
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void status_menu(void)
 {
	int c;

	init_status();		/* initialize STATUS card */

	status_config_display();

	while(TRUE)
	{
		com_puts("\r\n\r\nSTATUS FUNCUTIONS\r\n");

		com_puts("\r\n1. STATUS status\r\n");
		com_puts("\r\n2. Test LED\r\n");
		com_puts("\r\n3. Sequence LEDs\r\n");
		com_puts("\r\n9. exit\r\n");
		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(c)
		{
			case '1':
					status_status();
					break;

			case '2':
					status_test();
					break;

			case '3':
					status_sequence_test();
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
 *	Status
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void status_status(void)
 {
	char temp[80];
	char buffer[80];
	int status;
	int bank;

	com_puts("\r\nLED Status\r\n");

	for(bank=0;bank<4;bank++) {
		status = read_LED(bank);
		int_to_binary(status,temp);
		sprintf(buffer,"STATUS Bank %d Status %s\r\n",bank,temp);
		com_puts(buffer);
		}

	com_puts("\r\nSW Status\r\n");

	for(bank=0;bank<4;bank++) {
		status = read_SW(bank);
		int_to_binary(status,temp);
		sprintf(buffer,"STATUS Bank %d Status %s\r\n",bank,temp);
		com_puts(buffer);
		}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Status
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void status_test(void)
 {
	char temp[80];
	int status;
	int LED;
	int bank;

	com_puts("\r\n\nSelect BANK Number (0..3) ");
	com_gets(temp);        /* get a string from the serial port */
	bank = atoi(temp) & 0x03;

	com_puts("\r\n\nSelect LED Number (0..7) ");
	com_gets(temp);        /* get a string from the serial port */
	LED = atoi(temp) & 0x07;

	reset_LED(bank, LED);
	status = test_LED(bank, LED);
	led_report(status);
	pause();

	set_LED(bank, LED);
	status = test_LED(bank, LED);
	led_report(status);
	pause();

	toggle_LED(bank, LED);
	status = test_LED(bank, LED);
	led_report(status);
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Status
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void led_report(int status)
 {
	switch(status)
	{
		case LED_RESET :	com_puts("\r\nLED reset\r\n");
					break;
		case LED_SET :		com_puts("\r\nLED set\r\n");
					break;
		case LED_ERROR :	com_puts("\r\nLED error\r\n");
					break;
		default:		com_puts("\r\nUNKNOWN CODE\r\n");
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Display Status Configuration Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void status_config_display(void)
 {
	char temp[80];
	int i;
	unsigned int led_addr,sw_addr,led,sw;

	sprintf(temp,"base address: 0x%4x\r\n",status_led_sw.status_addr);
	com_puts(temp);

	com_puts("\r\nled addr sw addr led sw\r\n\r\n");

	for(i=0;i<4;i++) {
		led_addr = status_led_sw.led_addr[i];
		sw_addr = status_led_sw.sw_addr[i];
		led = status_led_sw.led[i];
		sw = status_led_sw.sw[i];

		sprintf(temp,"%4X     %4X %4X %4X\r\n",led_addr,sw_addr,led,sw);
		com_puts(temp);
		}

	pause();		/* wait for key press */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Status Sequence	Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void status_sequence_test(void)
 {
	char temp[80];
	int bank;
	int LED;
	int repeat;

	for(repeat=0;repeat<16;repeat++) {

	for(bank=0;bank<4;bank++) {
		//delay(32000);
		delay(32000);
		for(LED=0;LED<8;LED++) {

			sprintf(temp,"bank %d LED %d\r\n",bank,LED);
			com_puts(temp);

			toggle_LED(bank, LED);		/* toggle STATUS LED */
			//test_LED(bank, LED);		/* return state of LED	*/
			//set_LED(bank, LED);			/* set STATUS LED */
			//reset_LED(bank, LED);		/* reset STATUS LED */

			//delay(32000);
			//delay(32000);

			//write_LED(bank, LED);	/* update bank of LEDs */
			//delay(32000);
			//delay(32000);
			//read_LED(bank);		/* read bank of LEDs */

			//delay(32000);
			delay(32000);
			}
		}
	}

 }
