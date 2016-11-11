/* file: TLX386.c */

/****************************************************************************
 *
 *	MAIN function
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma message("AIO A/D CONFIGURATION")

 #define LPM_AIO_DC             /* winsystems analog input-output board */

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

 #include	"platform.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Local Definitions
 *
 ***************************************************************************/

 #define TRUE 1

 #pragma page(1)

/****************************************************************************
 *
 *	main function
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void main(void)
 {
 unsigned Baud=9600;	/* 300,1200,2400,4800,9600,19200,38400,57600 */
 unsigned Port=0;		/* 0,1 */
 int c;

/****************************************************************************
 *
 *      initialize Interrupt Controller
 *
 ***************************************************************************/

	init_icu();                                     /* initialize icu on LPM-SBC40 card */

	outp(ICU_BASE+1,0xff);                  /* mask all interruputs (disable) */

 #ifdef _SLAVE_ICU

	outp(ICU_SLAVE_BASE+1,0xff);    /* mask all interruputs (disable) */

 #endif

/****************************************************************************
 *
 *      announce that we are a live
 *
 ***************************************************************************/

	sound_beeps(4);

/****************************************************************************
 *
 *	initialize analog I/O hardware
 *
 ***************************************************************************/

	a_d_config();		/* configure A/D */

	init_A_D();			/* initialize A/D converter */

	d_a_config();		/* configure D/A */

	D_A_init();			/* initialize D/A converter */

/****************************************************************************
 *
 *	initialize timer hardware
 *
 ***************************************************************************/
 #undef SAMPLE_RATE
 #define SAMPLE_RATE 16             /* 16 HZ */

	init_timer(SAMPLE_RATE);		/* initialize timer */
	install_application(brain);		/* activate autopilot */

/****************************************************************************
 *
 *	initialize serial control unit
 *
 ***************************************************************************/

    dspio_1_open(Baud);         	/* open the serial port */

    dspio_2_open(Baud);         	/* open the serial port */

/****************************************************************************
 *
 *	initialize serial function pointers
 *
 ***************************************************************************/

	init_gnd(0);

	printf_init(dspio_1_puts);

/****************************************************************************
 *
 *	initialize Command/Telemetry interface
 *
 ***************************************************************************/

    int_disable(CMD_IRQ);   /* disable command decoder */

	cmd.frame_count = 0;

/****************************************************************************
 *
 *	enable interrupts
 *
 ***************************************************************************/

	int_enable(CMD_IRQ);    /* enable command decoder */

    dspio_1_puts("TLX AUTOPILOT\r\n");

    init_brain();

/****************************************************************************
 *
 *	main loop
 *
 ***************************************************************************/

	while(TRUE) {

        if(dspio_1_check()!=ERR) {          /* character waiting at port */
			if(dspio_1_getc()==0x1b) {		/* must be an escape character */
                gnd_menu();                 /* invoke calibration menu */
                }
            }

		} /* while */

/****************************************************************************
 *
 *	shutdown
 *
 ***************************************************************************/

	dspio_1_close();
	dspio_2_close();

	int_disable(TCT0_IRQ);	/* disable specific interrupt */

	int_disable(CMD_IRQ);	/* disable command decoder */

/****************************************************************************
 *
 *	DANGER ZONE - if program gets to here, bad news
 *
 ***************************************************************************/

	_astart();			/* reset environment and restart code */

 }						/* end of main function */
