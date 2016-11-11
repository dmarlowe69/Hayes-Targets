// file: TLX_HK.c */

//////////////////////////////////////////////////////////////////////////////
//
//  	MAIN function
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include definitions
//

 #include   <dos.h>
 #include   <conio.h>
 #include	"config.h"
 #include   "icu.h"
 #include   "tcu.h"
 #include   "dspio.h"
 #include   "ground.h"

//
//  	Local Definitions
//

 #define TRUE 1
 #define FALSE 0

 void init_brain(void);
 void deinit_brain(void);
 void far brain(void);
 void init_systems(void);
 void deinit_systems(void);

 void far house_keep(void);

 void a_d_config(void);     /* configure A/D data structure */
 void d_a_config(void); 	/* configure D/A data structure */

//////////////////////////////////////////////////////////////////////////////
//
//      Main function
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void main(void)
 {
 unsigned Baud=9600;	/* 300,1200,2400,4800,9600,19200,38400,57600 */
 unsigned Port=1;       /* 0,1 */
 int c;
 int exit_flag=FALSE;

//
//  initialize serial control unit
//

    dspio_1_open(Baud);             /* open the serial port */

    dspio_2_open(Baud);         	/* open the serial port */

//
//  initialize serial function pointers
//

    init_gnd(Port);

//
//      Power Up Banner
//
        printf("TLX Height Keeping Control System\r\n");
        dspio_printf("TLX Height Keeping Control System\r\n");

//
//      initialize Interrupt Controller
//

    //init_icu();                                     /* initialize icu on LPM-SBC40 card */

    //outp(ICU_BASE+1,0xff);                  /* mask all interruputs (disable) */

 #ifdef _SLAVE_ICU

    //outp(ICU_SLAVE_BASE+1,0xff);    /* mask all interruputs (disable) */

 #endif

//
//  initialize Command/Telemetry interface
//

//
//  Start Autopilot
//
    printf("TLX AUTOPILOT\r\n");
    dspio_printf("TLX AUTOPILOT\r\n");

    init_brain();

//
//  main loop
//

	while(TRUE) {

        if(dspio_1_check()!=ERR) {          /* character waiting at port */
            if(dspio_1_getc()==0x1b) {      /* must be an escape character */
                printf("dspio_1_check = ESC\r\n");
                gnd_menu();                 /* invoke calibration menu */
                printf("Return From gnd_menu\r\n");
                }
            }

        if(dspio_2_check()!=ERR) {          /* character waiting at port */
            if(dspio_2_getc()==0x1b) {      /* must be an escape character */
                printf("dspio_2_check = ESC\r\n");
                exit_flag = gnd_menu();     /* invoke calibration menu */
                printf("Return From gnd_menu\r\n");
                if(exit_flag == TRUE)
                    break;
                }
            }

		} /* while */

//  DANGER ZONE - if program gets to here, bad news
//
//  shutdown
//

//
//  close serial ports
//
	dspio_1_close();
	dspio_2_close();

//
//  shut down Autopilot
//

    deinit_brain();

 end:
        printf("Exiting - TLX Height Keeping Control System\r\n");

 }                      /* end of main function */
//////////////////////////////////////////////////////////////////////////////
//
//      Initialize application function
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void init_brain(void)
 {
    init_systems();                         /* initialize hardware */

    //autopilot_init();                       /* initialize autopliot */
//
//  	initialize timer hardware
//
 #define SAMPLE_RATE 16             /* 16 HZ */

    init_timer(SAMPLE_RATE);        /* initialize timer */
    install_application(brain);     /* activate autopilot */

 }
 void deinit_brain(void)
 {
    deinit_timer();
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Hardware Sample Rate Function
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void far brain(void)
 {
    if(tcu_table.ticks > 16) {
        tcu_table.ticks = 0;
        com_putc('.');
        }

    //autopilot();
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Inintialize application specific hardware
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void init_systems(void)
 {
   printf("init_systems\r\n");
   dspio_printf("init_systems\r\n");

//
//  initialize analog I/O hardware
//

    a_d_config();       /* configure A/D */

    //init_A_D();         /* initialize A/D converter */

    d_a_config();       /* configure D/A */

    //D_A_init();         /* initialize D/A converter */

 }
 void deinit_systems(void)
 {

 }
//////////////////////////////////////////////////////////////////////////////
//
//      House Keep application specific hardware
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void far house_keep(void)
 {

 }
//////////////////////////////////////////////////////////////////////////////
//
//      A/D Configuration Routine
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void a_d_config(void)
 {
	printf("a_d_config\r\n");
    dspio_printf("a_d_config\r\n");

 }
//////////////////////////////////////////////////////////////////////////////
//
//      D/A Configuration Routine
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void d_a_config(void)
 {
   printf("d_a_config\r\n");
   dspio_printf("d_a_config\r\n");
 }
