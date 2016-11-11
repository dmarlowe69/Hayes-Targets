/* file: menu_tm.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Ground Test Functions")
 #pragma subtitle("(c)1991-1993 Hayes Targets")

/****************************************************************************
 *
 *      Ground test functions
 *
 *      written by denton marlowe
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
 *      CMD function pointer to command decoder function
 *
 ***************************************************************************/

 void (far *funptr_decoder_application)(void);	/* pointer to function */

/****************************************************************************
 *
 *      1553 serial interface function prototypes
 *
 ***************************************************************************/

 void cmd_tm_menu(void);        /* CMD/TM functions menu */
 void display_CMD(void);        /* display CMD data */
 void calibrate_CMD(void);      /* calibrate CMD data */
 void display_decode(void);     /* display and decode CMD data */
 void display_TM(void);         /* display TM data */
 void calibrate_TM(void);       /* calibrate TM data */

 void install_decoder_application(void (far *routine)(void));
						    /* install pointer to main timing loop application */
 void far dummy_decoder_application(void);
							/* dummy main timing loop application */

 #pragma page(1)

/****************************************************************************
 *
 *      CMD/TM functions menu
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void cmd_tm_menu(void)
 {
	int c;

	if(funptr_decoder_application == NULL)
		install_decoder_application(dummy_decoder_application);

	while(TRUE)
	{
		com_puts("\r\n\r\nANALOG FUNCTION MENU\r\n");

		com_puts("\r\n0. Display CMD data\r\n");
		com_puts("\r\n1. enable/disable CMD interrupts\r\n");
		com_puts("\r\n2. reset encoder/decoder\r\n");
		com_puts("\r\n3. manual entry of CMD data\r\n");
		com_puts("\r\n4. decode CMD data\r\n");
		com_puts("\r\n5. Display TM data\r\n");
		com_puts("\r\n6. enable/disable TM interrupts\r\n");
		com_puts("\r\n7. verify TM operation\r\n");
		com_puts("\r\n8. manual entry of TM data\r\n");
		com_puts("\r\n9. exit to main menu\r\n");

		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(c)
		{
			case '0':       display_CMD();
					break;

			case '1':       if(int_toggle(CMD_IRQ) == ENABLE)
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

			case '2':       cmd_tm_reset();
					break;

			case '3':       calibrate_CMD();
					break;

			case '4':       display_decode();
					break;

			case '5':       display_TM();
					break;

			case '6':       if(int_toggle(TM_IRQ) == ENABLE)
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

			case '8':       calibrate_TM();
					break;

			case '9':       return;

			default:
					break;
		}
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *      CMD/TM display routine
 *
 *      outputs CMD data to serial port
 *
 *      notes:
 *
 *      The telemetry data array elements are loaded into a temporary
 *      variable for processing as they are volatile data.
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void display_CMD(void)
 {
	char temp[80];
	char bits[80];
	unsigned char data;
	int i;

	com_puts("\r\n CMD DATA \r\n");

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		data = cmd.data[i];              /* temp storage */
		int_to_binary(data,bits);
		sprintf(temp,"CH%02d %03d %02X %s\r\n",i,data,data,bits);
		com_puts(temp);
		}

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *      calibrate CMD data
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void calibrate_CMD(void)
 {
	char temp[80];
	char bits[80];
	int addr;
	unsigned int data;
	double volts;

	com_puts("\r\n CMD Calibration \r\n");

	com_puts("\r\n\r\nSelect CMD Channel (0..15) ");
	com_gets(temp);        /* get a string from the serial port */

	addr = atoi(temp) & 0x0f;

	com_puts("\r\n\nEnter output Data (0..255) ");
	com_gets(temp);        /* get a string from the serial port */

	data = atoi(temp) & 0xff;

	int_to_binary(data,bits);

	volts = ( (double)(data) * (double)(5.0)/256.0);

	sprintf(temp,"\r\n\nCH%02d %03d %02X %s % 8.4f volts\r",addr,data,data,bits,volts);
	com_puts(temp);

	cmd.data[addr] = data;

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *      CMD/TM display routine
 *
 *      outputs and decodes CMD data to serial port
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void display_decode(void)
 {
	(*funptr_cmd_application)();		/* call command decoder code */

	(*funptr_decoder_application)();	/* call command decoder code */

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *      CMD/TM display routine
 *
 *      outputs TM data to serial port
 *
 *      notes:
 *
 *      The telemetry data array elements are loaded into a temporary
 *      variable for processing as they are volatile data.
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void display_TM(void)
 {
	char temp[80];
	char bits[80];
	unsigned char data;
	int i;

	com_puts("\r\n TM DATA \r\n");

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		data = tm.data[i];              /* temp storage */
		int_to_binary(data,bits);
		sprintf(temp,"CH%02d %03d %02X %s\r\n",i,data,data,bits);
		com_puts(temp);
		}

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Calibrate TM data
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void calibrate_TM(void)
 {
	char temp[80];
	char bits[80];
	int addr;
	unsigned int data;
	double volts;

	com_puts("\r\n TM Calibration \r\n");

	com_puts("\r\n\r\nSelect TM Channel (0..15) ");
	com_gets(temp);        /* get a string from the serial port */

	addr = atoi(temp) & 0x0f;

	com_puts("\r\n\r\nEnter output Data (0..255) ");
	com_gets(temp);        /* get a string from the serial port */

	data = atoi(temp) & 0xff;

	int_to_binary(data,bits);

	volts = ( (double)(data) * (double)(5.0)/256.0);

	sprintf(temp,"\r\n\r\nCH%02d %03d %02X %s % 8.4f volts\r",addr,data,data,bits,volts);
	com_puts(temp);

	tm.data[addr] = data;

	pause();
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Install User Decoder function
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void install_decoder_application(void (far *routine)(void))
 {
	funptr_decoder_application = routine;	/* install pointer to application */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Dummy Decoder Function
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void far dummy_decoder_application(void)
 {
	com_puts("\r\nInstall your own print function here\r\n");
 }
