/* file: menu_sbx.c */

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

 #pragma page(1)

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include	<conio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
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
 *	function prototypes
 *
 ***************************************************************************/

 void lpm_sbx_menu(void);
 void loop_LPM_SBX(void);
 void lpm_sbx_config_display(void);

 #pragma page(1)

/****************************************************************************
 *
 *	LPM-SBX functions
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void lpm_sbx_menu(void)
 {
	int c;
	int port_0;
	int port_1;
	int port_2;
	int port_3;

	port_2 = PPI_MODE_0 | PORTA_OUT | PORTC_UPPER_OUT | PORTB_OUT | PORTC_LOWER_OUT;
	port_3 = PPI_MODE_0 | PORTA_IN | PORTC_UPPER_IN | PORTB_IN | PORTC_LOWER_IN;

	port_0 = port_2;
	port_1 = port_3;

	init_lpm_sbx(port_0, port_1, port_2, port_3);
									/* initialize ppi on LBX-PIO CARD */

	lpm_sbx_config_display();

	while(TRUE)
	{
		com_puts("\r\n\r\nLPM-SBX FUNCUTIONS\r\n");

		com_puts("\r\n1. loop test\r\n");
		com_puts("\r\n9. exit\r\n");
		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(c)
		{
			case '1':
					loop_LPM_SBX();
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
 *	LPM-SBX functions
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void loop_LPM_SBX(void)
 {
	char temp[80];
	int port_0;
	int port_1;
	int port_2;
	int port_3;
	int port;
	int byte;

	outp(0x14b,0x9b);	/* set port 1 to input */
	outp(0x143,0x80);	/* set port 0 to output */

	outp(0x140,0xaa);	/* port a output */
	outp(0x141,0x55);	/* port b output */
	outp(0x142,0xdd);	/* port c output */

	byte = inp(0x148);	/* port a input */
	sprintf(temp,"port 1a 0xaa %2x\r\n",byte);
	com_puts(temp);
	byte = inp(0x149);	/* port b input */
	sprintf(temp,"port 1b 0x55 %2x\r\n",byte);
	com_puts(temp);
	byte = inp(0x14a);	/* port c input */
	sprintf(temp,"port 1c 0xdd %2x\r\n",byte);
	com_puts(temp);

	outp(0x15b,0x9b);	/* set port 3 to input */
	outp(0x153,0x80);	/* set port 2 to output */

	outp(0x150,0xaa);	/* port a output */
	outp(0x151,0x55);	/* port b output */
	outp(0x152,0xdd);	/* port c output */

	byte = inp(0x158);	/* port a input */
	sprintf(temp,"port 3a 0xaa %2x\r\n",byte);
	com_puts(temp);
	byte = inp(0x159);	/* port b input */
	sprintf(temp,"port 3b 0x55 %2x\r\n",byte);
	com_puts(temp);
	byte = inp(0x15a);	/* port c input */
	sprintf(temp,"port 3c 0xdd %2x\r\n",byte);
	com_puts(temp);

	byte = com_getc();

	if(byte == 0x1b)
		return;

	port_2 = PPI_MODE_0 | PORTA_OUT | PORTC_UPPER_OUT | PORTB_OUT | PORTC_LOWER_OUT;
	port_3 = PPI_MODE_0 | PORTA_IN | PORTC_UPPER_IN | PORTB_IN | PORTC_LOWER_IN;

	port_0 = port_2;
	port_1 = port_3;

	init_lpm_sbx(port_0, port_1, port_2, port_3);
									/* initialize ppi on LBX-PIO CARD */

	sprintf(temp,"input hex byte to output: ");
	com_puts(temp);

	com_gets(temp);

 //#ifdef PC
 #ifdef PC_CONSOLE

	sscanf(temp,"%2x",&byte);

 #else

	byte = atoi(temp) & 0xff;

 #endif

	port = 0;
	write_porta_lpm_sbx(byte, port);	/* write byte to port A */
	byte++;
	write_portb_lpm_sbx(byte, port);	/* write byte to port B */
	byte++;
	write_portc_lpm_sbx(byte, port);	/* write byte to port C */

	port = 2;
	write_porta_lpm_sbx(byte, port);	/* write byte to port A */
	byte++;
	write_portb_lpm_sbx(byte, port);	/* write byte to port B */
	byte++;
	write_portc_lpm_sbx(byte, port);	/* write byte to port C */

	//multi_delay(1024);

	port = 1;
	byte = read_porta_lpm_sbx(port);		/* read byte from port A */
	sprintf(temp,"port 1a %2x\r\n",byte);
	com_puts(temp);

	byte = read_portb_lpm_sbx(port);		/* read byte from port B */
	sprintf(temp,"port 1b %2x\r\n",byte);
	com_puts(temp);

	byte = read_portc_lpm_sbx(port);		/* read byte from port C */
	sprintf(temp,"port 1c %2x\r\n",byte);
	com_puts(temp);

	port = 3;
	byte = read_porta_lpm_sbx(port);		/* read byte from port A */
	sprintf(temp,"port 3a %2x\r\n",byte);
	com_puts(temp);

	byte = read_portb_lpm_sbx(port);		/* read byte from port B */
	sprintf(temp,"port 3b %2x\r\n",byte);
	com_puts(temp);

	byte = read_portc_lpm_sbx(port);		/* read byte from port C */
	sprintf(temp,"port 3c %2x\r\n",byte);
	com_puts(temp);

	//int toggle_bit_lpm_sbx(int bit, int port);	/* toggle PPI port c bits */
	//int test_bit_lpm_sbx(int bit, int port);	/* test PPI port c bit */
	//int set_bit_lpm_sbx(int bit, int port);	/* set PPI port c bit */
	//int reset_bit_lpm_sbx(int bit, int port);	/* reset PPI port c bit */

 }

 #pragma page(1)

/****************************************************************************
 *
 *	Display LPM-SBX Configuration Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void lpm_sbx_config_display(void)
 {
	char temp[80];
	int port_0;
	int port_1;
	int port_2;
	int port_3;
	int port;

	port_2 = PPI_MODE_0 | PORTA_OUT | PORTC_UPPER_OUT | PORTB_OUT | PORTC_LOWER_OUT;
	port_3 = PPI_MODE_0 | PORTA_IN | PORTC_UPPER_IN | PORTB_IN | PORTC_LOWER_IN;

	port_0 = port_2;
	port_1 = port_3;

	for(port=0;port<4;port++) {
		sprintf(temp,"lpm_sbx[%1d].ppi_base_addr %4x\r\n",port,lpm_sbx[port].ppi_base_addr);
		com_puts(temp);
		sprintf(temp,"lpm_sbx[%1d].port_a_addr  %4x\r\n",port,lpm_sbx[port].port_a_addr);
		com_puts(temp);
		sprintf(temp,"lpm_sbx[%1d].port_b_addr  %4x\r\n",port,lpm_sbx[port].port_b_addr);
		com_puts(temp);
		sprintf(temp,"lpm_sbx[%1d].port_c_addr  %4x\r\n",port,lpm_sbx[port].port_c_addr);
		com_puts(temp);
		sprintf(temp,"lpm_sbx[%1d].cntrl_addr   %4x\r\n",port,lpm_sbx[port].cntrl_addr);
		com_puts(temp);
		}

	sprintf(temp,"port_0 %4x\r\n",port_0);
	com_puts(temp);
	sprintf(temp,"port_1 %4x\r\n",port_1);
	com_puts(temp);
	sprintf(temp,"port_2 %4x\r\n",port_2);
	com_puts(temp);
	sprintf(temp,"port_3 %4x\r\n",port_3);
	com_puts(temp);

	pause();		/* wait for key press */
 }
