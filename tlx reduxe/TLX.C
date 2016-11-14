// file: TLX.C
 #define PC_CONSOLE
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
 #include   "tlxpilot.h"
 #include   "MDS.H"

//
//  	Local Definitions
//

 #define TRUE 1
 #define FALSE 0

//
//      Global Variables
//
 int autopilot_run_flag = 0;
 int autopilot_debug_flag = 1;

//
//      Function Prototypes
//
 void init_brain(void);
 void deinit_brain(void);
 void brain_loop(void);
 void far brain(void);
 void init_systems(void);
 void deinit_systems(void);
 void cmd_processor(void);
 void tm_processor(void);

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
 void main( int argc,       /* Number of strings in array argv          */
           char *argv[],    /* Array of command-line argument strings   */
           char **envp )    /* Array of environment variable strings    */
 {
 unsigned Baud=9600;	/* 300,1200,2400,4800,9600,19200,38400,57600 */
 unsigned Port=1;       /* 0,1 */
 int c;
 int exit_flag=FALSE;
 int cmd_line_args_count;

//
//  Process Command Line arguments
//

//
//  Process Environment variables
//

//
//  Initialize Autopilot Comnnands
//

	CMDPRESET = 50.0;
    PGAIN = KP_DEFAULT;
    RGAIN = KV_DEFAULT;
    IGAIN = KI_DEFAULT;
    MGAIN = MR_DEFAULT;
    SGAIN = KS_DEFAULT;
    RCVY_FLY = RCVY_MODE;
    PROG_MODE = PROG_STD;

    fvalid = 5.0;
    falt = 50.0;
    faccel = 0.0;

//
//  initialize serial control unit
//

    //dspio_1_open(Baud);             /* open the serial port */

    //dspio_2_open(Baud);             /* open the serial port */

//
//  initialize serial function pointers
//

    //init_gnd(Port);

//
//      Power Up Banner
//
        printf("TLX Height Keeping Control System\n");

//
//      initialize Interrupt Controller
//

    //init_icu();           	/* initialize icu on LPM-SBC40 card */

    //outp(ICU_BASE+1,0xff);    /* mask all interruputs (disable) */

 #ifdef _SLAVE_ICU

    //outp(ICU_SLAVE_BASE+1,0xff);    /* mask all interruputs (disable) */

 #endif

//
//  initialize Command/Telemetry interface
//

//
//  Start Autopilot
//
    printf("TLX AUTOPILOT\n");

    init_brain();

//
//  main loop
//

//    while(TRUE) {
//
//        if(dspio_1_check()!=ERR) {          /* character waiting at port */
//            if(dspio_1_getc()==0x1b) {      /* must be an escape character */
//                printf("dspio_1_check = ESC\r\n");
//                gnd_menu();                 /* invoke calibration menu */
//                printf("Return From gnd_menu\r\n");
//                }
//            }

//        if(dspio_2_check()!=ERR) {          /* character waiting at port */
//            if(dspio_2_getc()==0x1b) {      /* must be an escape character */
//                printf("dspio_2_check = ESC\r\n");
//                exit_flag = gnd_menu();     /* invoke calibration menu */
//                printf("Return From gnd_menu\r\n");
//                if(exit_flag == TRUE)
//                    break;
//                }
//            }
//
//        } /* while */

    while(TRUE) {

            cmd_processor();            /* command processor */
            brain_loop();               /* call brain loop function */
            tm_processor();             /* telemetry processor */

            if(com_check()!=0) {
            	if(com_getc()==0x1b) {      /* must be an escape character */
                	exit_flag = gnd_menu();     /* invoke calibration menu */
                    printf("Return From gnd_menu\n");
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
    //dspio_1_close();
    //dspio_2_close();

//
//  shut down Autopilot
//

    deinit_brain();

 end:
        printf("Exiting - TLX Height Keeping Control System\n");

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
    init_systems();                 /* initialize hardware */

    autopilot_run_flag = 0;         /* reset Autopilot Run Flag */
    autopilot_debug_flag = 1;       /* Set Autopilot Debug Flag */

    cmd_processor();                /* command processor */
    autopilot_init();               /* initialize autopliot */

    if(autopilot_debug_flag == 1) {
        preset_dump();
        gain_dump();
        autopilot_dump_header();
        }

//
//  	initialize timer hardware
//
 #define SAMPLE_RATE 16             /* 16 HZ */

    init_timer(SAMPLE_RATE);        /* initialize timer */
    install_application(brain);     /* activate autopilot */

 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void deinit_brain(void)
 {
    deinit_timer();
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Brain Loop function
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void brain_loop(void)
 {
    if(autopilot_run_flag == 1) {
        autopilot_run_flag = 0;
        autopilot();
        if(autopilot_debug_flag == 1) {
            autopilot_dump();
            }
        }
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
        //com_putc('.');
        }

    //autopilot();
    autopilot_run_flag = 1;                 /* Set Autopilot Run Flag */
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
   printf("init_systems\n");

//
//  initialize analog I/O hardware
//

    a_d_config();       /* configure A/D */

    d_a_config();       /* configure D/A */

 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void deinit_systems(void)
 {

 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void cmd_processor(void)
 {
    //
    //  Preset Altitude
    //
    //CMDPRESET = 50.0;
    if(CMDPRESET < 20.0)
        CMDPRESET = 20.0;
    if(CMDPRESET > 500.0)
        CMDPRESET = 50.0;
    tlx_cmd_data.preset = (int)((CMDPRESET * 255.0)/100.0);

    //
    //  proportional gain
    //
    //PGAIN = KP_DEFAULT;
    tlx_cmd_data.proportional_gain = PGAIN & 0xff;

    //
    //  rate gain
    //
    //RGAIN = KV_DEFAULT;
    tlx_cmd_data.rate_gain = RGAIN & 0xff;

    //
    //  integral gain
    //
    //IGAIN = KI_DEFAULT;
    tlx_cmd_data.integral_gain = IGAIN & 0xff;

    //
    //  scale factor
    //
    //SGAIN = KS_DEFAULT;
    tlx_cmd_data.delta_scale = ( (SGAIN & 0xf0) >> 4 );

    //
    //  manual reset
    //
    //MGAIN = MR_DEFAULT;
    tlx_cmd_data.delta_ec_manual = MGAIN & 0xff;

    //
    //  command channel
    //
    //
    //  RCVY/FLY
    //
    //RCVY_FLY = FLY_MODE;
    tlx_cmd_data.rcvy_fly = RCVY_FLY;

    //
    //  PROG
    //
    //PROG_MODE = PROG_STD;
    tlx_cmd_data.prog_mode = PROG_MODE;

 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void tm_processor(void)
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
    printf("a_d_config\n");
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
   printf("d_a_config\n");
 }
