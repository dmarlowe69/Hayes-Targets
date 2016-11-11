/* file: menu_ana.c */

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
 *	ANALOG serial interface function prototypes
 *
 ***************************************************************************/

 void analog_menu(void);        /* analog functions menu */
 void display_AD(void);         /* display A/D input data */
 void verify_AD(void);          /* verify A/D operation */
 void calibrate_AD(void);       /* calibrate A/D */
 void display_DA(void);         /* display D/A output data */
 void calibrate_DA(void);		/* calibrate D/A */

 void a_d_config_display(void); /* display A/D data structure */
 void d_a_config_display(void); /* display D/A data structure */

 #pragma page(1)

/****************************************************************************
 *
 *	ANALOG functions menu
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void analog_menu(void)
 {
	int c;

	test_A_D(0,&wad);       /* initialize variables */

	a_d_config_display();	/* display A/D data structure */

	d_a_config_display();	/* display D/A data structure */

	while(TRUE)
	{
		com_puts("\r\n\r\nANALOG FUNCTION MENU\r\n");

		com_puts("\r\n0. Display A/D input data\r\n");
		com_puts("\r\n1. Verify A/D converter Operational\r\n");
		com_puts("\r\n2. Step up/down Calibration Loop\r\n");
		com_puts("\r\n3. reset Calibration Loop\r\n");
		com_puts("\r\n4. Calibrate A/D converter\r\n");
		com_puts("\r\n5. Display D/A output data\r\n");
		com_puts("\r\n6. Calibrate D/A converter\r\n");
		com_puts("\r\n7. Disable A/D interrupts\r\n");
		com_puts("\r\n8. Enable A/D interrutps\r\n");
		com_puts("\r\n9. exit to main menu\r\n");

		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(c)
		{
			case '0':
					display_AD();
					break;

			case '1':
					verify_AD();
					break;

			case '2':
					test_A_D(6,&wad);		/* step up/down calibration value */
					break;

			case '3':
					test_A_D(0,&wad);		/* reset calibration loop alue */
					break;

			case '4':
					calibrate_AD();
					break;

			case '5':
					display_DA();
					break;

			case '6':
					calibrate_DA();
					break;

			case '7':
					int_disable(AD_IRQ);
					com_puts("\r\ndisabled\r\n");
					pause();
					break;

			case '8':
					init_A_D();
					com_puts("\r\nenabled\r\n");
					pause();
					break;

			case '9':      return;

			default:
					break;
		}
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Analog I/O display routine
 *
 *	outputs A/D data to serial port
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void display_AD(void)
 {
	char temp[80];
	char buffer[80];
	signed int input;
	int i;
	int max,min,zero,polarity;
	double volts;

	com_puts("\r\n A/D Input Data \r\n");

	max = adc.MAX;
	min = adc.MIN;
	zero = adc.ZERO;
	polarity = adc.POLARITY;

	for(i=0;i<16;i++) {
		input = adc.A_D[i];
		int_to_binary12(input,temp);
		volts = ( (double)(input) * (double)(max - min)/4096.0);
		sprintf(buffer,"CH%02d % 05d %04X %s % 8.4f volts\r\n",i,input,input,temp,volts);
		com_puts(buffer);
		}

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *	A/D Operational verification
 *
 *	outputs A/D data to serial port
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void verify_AD(void)
 {
	char temp[80];
	char buffer[80];
	signed int input;
	int max,min,zero,polarity;
	double volts;

/****************************************************************************
 *
 *	check for A/D converter time out (convert flag latch up)
 *
 ***************************************************************************/

	if(test_A_D(3,&wad) == ERR) {
		com_puts("\r\nERROR : time out on A/D conversion\r\n");
		}
	else
		{
		com_puts("\r\nOK: A/D passed time out check\r\n");
		}

	sprintf(buffer,"\r\nTime out = % 04X\r\n",wad.time_out);
	com_puts(buffer);

/****************************************************************************
 *
 *	check for A/D converter interrupt active
 *
 ***************************************************************************/

	test_A_D(1,&wad);       /* reset A/D interrupt cycle */

	if(test_A_D(4,&wad) == ERR) {
		com_puts("\r\nERROR : A/D interrupt not active\r\n");
		}
	else
		{
		com_puts("\r\nOK : A/D interrupt  is active\r\n");
		}

	sprintf(buffer,"\r\nTime out = % 04X\r\n",wad.time_out);
	com_puts(buffer);

/****************************************************************************
 *
 *	check for A/D converter calibration using D/A calibration loop
 *
 ***************************************************************************/

	if(test_A_D(5,&wad)==ERR) {
		com_puts("\r\nERROR: A/D out of calibration\r\n");
		}
	else
		{
		com_puts("\r\nOK: A/D in calibration\r\n");
		}

		int_to_binary12(wad.output,temp);
		max = dac[D_A_CAL_LOOP].MAX;
		min = dac[D_A_CAL_LOOP].MIN;
		volts = ( (double)(wad.output) * (double)(max - min)/4096.0) + min;
		sprintf(buffer,"Output = % 05d %04X %s % 8.4f volts\r\n",wad.output,wad.output,temp,volts);
		com_puts(buffer);

		input = wad.seed;
		max = adc.MAX;
		min = adc.MIN;
		int_to_binary12(input,temp);
		volts = ( (double)(input) * (double)(max - min)/4096.0);
		sprintf(buffer,"Seed = % 05d %04X %s % 8.4f volts\r\n",input,input,temp,volts);
		com_puts(buffer);

		input = wad.input;
		max = adc.MAX;
		min = adc.MIN;
		int_to_binary12(input,temp);
		volts = ( (double)(input) * (double)(max - min)/4096.0);
		sprintf(buffer,"Input = % 05d %04X %s % 8.4f volts\r\n",input,input,temp,volts);
		com_puts(buffer);

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Analog A/D calibration routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void calibrate_AD(void)
 {
	char temp[80];
	char buffer[80];
	signed int input;
	int i;
	int c;
	int max,min,zero,polarity;
	double volts;

	com_puts("\r\n A/D Calibration \r\n");

	com_puts("\r\n\nSelect A/D Channel (0..15) ");
	com_gets(temp);        /* get a string from the serial port */

	new_line();

	i = atoi(temp) & 0x0f;

	max = adc.MAX;
	min = adc.MIN;
	zero = adc.ZERO;
	polarity = adc.POLARITY;

	while(TRUE) {
		input = adc.A_D[i];
		int_to_binary12(input,temp);
		volts = ( (double)(input) * (double)(max - min)/4096.0);
		sprintf(buffer,"CH%02d % 05d %04X %s % 8.4f volts\r",i,input,input,temp,volts);
		com_puts(buffer);

 //#ifdef PC
 #ifdef PC_CONSOLE
		if(kbhit()) {
 #else
		if(com_check()!=ERR) {
 #endif
			c = com_getc();
			if(c == 27)
				break;
			}
		}

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Analog I/O display routine
 *
 *	outputs A/D data to serial port
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void display_DA(void)
 {
	char temp[80];
	char buffer[80];
	unsigned int output;
	int i;
	int max,min,zero,polarity;
	double volts;

	com_puts("\r\n D/A Output Data \r\n");

	for(i=0;i<8;i++) {
		max = dac[i].MAX;
		min = dac[i].MIN;
		zero = dac[i].ZERO;
		polarity = dac[i].POLARITY;
		output = dac[i].D_A;
		int_to_binary12(output,temp);
		volts = ( (double)(output) * (double)(max - min)/4096.0) + min;
		sprintf(buffer,"CH%02d %04d %03X %s % 8.4f volts\r\n",i,output,output,temp,volts);
		com_puts(buffer);
		}

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Analog D/A calibration routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void calibrate_DA(void)
 {
	char temp[80];
	char buffer[80];
	int d_a_channel;
	int data;
	int max,min,zero,polarity;
	double volts;

	com_puts("\r\n D/A Calibration \r\n");

	com_puts("\r\n\nSelect D/A Channel (0..7) ");
	com_gets(temp);        /* get a string from the serial port */

	d_a_channel = atoi(temp) & 0x07;

	max = dac[d_a_channel].MAX;
	min = dac[d_a_channel].MIN;
	zero = dac[d_a_channel].ZERO;
	polarity = dac[d_a_channel].POLARITY;

	com_puts("\r\n\nEnter output Data (0..4095) ");
	com_gets(temp);        /* get a string from the serial port */

	data = atoi(temp) & 0xfff;

	int_to_binary12(data,temp);

	volts = ( (double)(data) * (double)(max - min)/4096.0) + min;

	sprintf(buffer,"\r\n\nCH%02d %04d %03X %s % 8.4f volts\r",d_a_channel,data,data,temp,volts);
	com_puts(buffer);

	D_A_channel_out(d_a_channel,data);

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Display A/D Configuration Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void a_d_config_display(void)
 {
	char temp[80];
	int zero,max,min,polarity;

	com_puts("\r\nZERO MAX MIN POLARITY\r\n\r\n");

	zero = adc.ZERO;
	max = adc.MAX;
	min = adc.MIN;
	polarity = adc.POLARITY;

	sprintf(temp,"%4d %3d %3d %8d\r\n",zero,max,min,polarity);
	com_puts(temp);

	pause();		/* wait for key press */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Display D/A Configuration Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void d_a_config_display(void)
 {
	char temp[80];
	int i;
	int data,addr,zero,max,min,polarity;

	com_puts("\r\nCH DATA ADDR ZERO MAX MIN POLARITY\r\n\r\n");

	for(i=0;i<8;i++) {
	    data = dac[i].D_A;
	    addr = dac[i].ADDR;
	    zero = dac[i].ZERO;
	    max = dac[i].MAX;
	    min = dac[i].MIN;
	    polarity = dac[i].POLARITY;

	    sprintf(temp,"%2d %4d %4d %4d %3d %3d %8d\r\n",i,data,addr,zero,max,min,polarity);
	    com_puts(temp);
	    }

	pause();		/* wait for key press */
 }
