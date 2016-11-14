/* file: tlx.c */

 //#define _WATCH_DOG

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Main function")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *      MAIN function
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      This Software was Designed using the following development tools:
 *
 *      MS-DOS Verison 3.3
 *      Microsoft C Version 5.1
 *      MASM Version 5.1
 *      Datalight C_Thru_ROM Version 5.1
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
 *      Include Microsoft definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include	<hayes.h>
 #include	<gnd.h>

 #include   "v_str.h"
 #include   "platform.h"

 #include   "pilot.h"
 #include   "utility.h"
 #include   "ground.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/***************************************************************************
 *
 *      general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

/****************************************************************************
 *
 *      Baud Rate
 *
 ***************************************************************************/

 #define BAUD_RATE      9600U

 #pragma page(1)

/****************************************************************************
 *
 *      main function
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void main(void)
 {
 char Application_Name[16];
 char *pAppName;
 int c;
 unsigned long int kernel_count;

/****************************************************************************
 *
 *      initialize Interrupt Controller
 *
 ***************************************************************************/

    init_icu();             	/* initialize icu on LPM-SBC40 card */

    outp(ICU_BASE+1,0xff);  	/* mask all interruputs (disable) */

/****************************************************************************
 *
 *      initialize status card
 *
 ***************************************************************************/

 #ifdef LED_DEBUG

	init_status();

 #endif

/****************************************************************************
 *
 *      initialize serial control unit
 *
 ***************************************************************************/

    scu_open(BAUD_RATE);    	/* open the serial port */

    init_gnd(0);            	/* setup required comm port */

/****************************************************************************
 *
 *      initialize serial interface
 *
 ***************************************************************************/

 #define _SERIAL_PROMPT
 #define _KERNEL_COUNT 100000U
 #ifdef  _SERIAL_PROMPT
 #pragma message("_SERIAL_PROMPT")

	kernel_count = 0L;

    while(TRUE) {

        if(com_check()!=ERR) {              	/* character waiting at port */
			c = com_getc();
            if(c==0x1b) {                   	/* must be an escape character */
            	com_puts("V40\r\n");
            	pAppName = strcpy(Application_Name,"TLX");
                com_puts(Application_Name);
				com_puts("\r\n");
				break;
				}
			if(c=='k') {
                com_puts("KERNEL\r\n");
                scu_close();
                _kernel_reset();
                break;
				}
			}

		kernel_count++;
        if(kernel_count>_KERNEL_COUNT)
			break;

		} /* while */

 #endif

/****************************************************************************
 *
 *      ground test menu
 *
 ***************************************************************************/

    printf_init(scu_puts);

/****************************************************************************
 *
 *      initialize Command/Telemetry interface
 *
 ***************************************************************************/

    init_cmd_tm();

/****************************************************************************
 *
 *      initialize Analog I/O
 *
 ***************************************************************************/

	init_A_D();                     /* initialize A/D converter */

	D_A_init();                     /* initialize D/A converter */

/****************************************************************************
 *
 *  initialize main loop
 *
 ***************************************************************************/

	init_brain_loop();

	init_brain();

/****************************************************************************
 *
 *      initialize command processor
 *
 ***************************************************************************/

    _PRINT_APPLICATION_NAME();
	_PRINT_AUTHOR_NAME();
	_PRINT_HAYES_COPYRIGHT();

    _PRINT_VERSION_STRING();

	init_cmd_mode();        /* initialize command mode */

    com_puts("out of init_cmd_mode\r\n");

/****************************************************************************
 *
 *  initialize watch dog - note jumper j12 must be installed on board
 *
 ***************************************************************************/

 #ifdef _WATCH_DOG
 #pragma message("!!! WATCH-DOG !!! INSTALL JUMPER J12 TO ENABLE !!!")

    init_the_dog();

 #endif

/****************************************************************************
 *
 *  strobe watch dog port
 *
 ***************************************************************************/

 #ifdef _WATCH_DOG
 #pragma message("!!! WATCH-DOG !!! INSTALL JUMPER J12 TO ENABLE !!!")

    kick_the_dog();

 #endif

/****************************************************************************
 *
 *      main loop
 *
 ***************************************************************************/

    while(TRUE) {

		brain_loop();

/****************************************************************************
 *
 *      Watch dog functions
 *
 ***************************************************************************/

    if(test_A_D(4,&wad) == ERR)     /* test for A/D IRQ active */
        test_A_D(1,&wad);           /* reset A/D interrupt cycle */

 #ifdef _WATCH_DOG

    kick_the_dog();                 /* watch dog timer */

 #endif

    } /* while */

/****************************************************************************
 *
 *  de-initialize watch dog - note jumper j12 must be installed on board
 *
 ***************************************************************************/

 #ifdef _WATCH_DOG
 #pragma message("!!! WATCH-DOG !!! INSTALL JUMPER J12 TO ENABLE !!!")

    deinit_the_dog();

 #endif

/****************************************************************************
 *
 *      DANGER ZONE - if program gets to here, bad news
 *
 ***************************************************************************/

	_astart();                      /* reset environment and restart code */

 }                                              /* end of main function */

 //////////////////////////////////////////////////////////////////////
 //
 //

 //
 // This procedure is used to add my own exit procedure
 //
 void my_exit(void)
 {
    _disable(); // disable all hardware irqs

    com_puts("my_exit\r\n");

    //
    // do clean up task here (TBD)
    //


    while(1);   // loop forever or restart code?
 }
