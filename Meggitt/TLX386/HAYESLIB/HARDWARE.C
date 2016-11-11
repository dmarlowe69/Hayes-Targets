/* file: hardware.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Hardware Initialization functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      Hardware Initialization functions
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/90	D.marlowe	Original release
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

 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"

 #ifndef PC

 #include	"v40init.h"

 #endif

 #include	"ppi.h"

 #ifndef PC

 #include	"cmos.h"

 #endif

 #include	"icu.h"

 #ifndef PC

 #include	"tcu.h"
 #include	"rtc.h"
 #include	"uart.h"

 #endif

 #include	"remtron.h"
 #include	"analog.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Define Hardware configuration options
 *
 ***************************************************************************/

 #define RAM_ENV	0x00
 #define ROM_ENV	0x01
 #define PC_ENV 	0x02

/****************************************************************************
 *
 *	Hardware initialization function prototypes
 *
 ***************************************************************************/

 void init_hardware(unsigned int configuration);	/* initialize hardware */

 void de_init_hardware(unsigned int configuration);	/* deinitialize hardware */

 void reset_boot(); 								/* cold boot */

 void (interrupt far *funptr_reset)(void);			/* pointer to function */

/****************************************************************************
 *
 *	end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Hardware initialization
 *
 *      DELCARATION:    void init_hardware(unsigned int configuration)
 *
 *      PARAMETERS:     configuration - hardware configuration options
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    hardware initialized
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:   v40_init()
 *                     init_cmos()
 *                     init_icu()
 *                     init_exception()
 *                     init_scu()
 *                     int_enable(SCU_IRQ)
 *                     init_ppi()
 *                     init_clock()
 *                     battery_backup_check()
 *                     set_clock_time(0,0,0)
 *					   set_clock_date(1,1,1990-1993)
 *                     uart_open(9600)
 *                     init_timer(SAMPLE_RATE)
 *                     init_remton()
 *                     init_A_D()
 *                     D_A_init()
 *
 *
 *      HEADERS:        <dos.h>
 *                      <conio.h>
 *                      config.h
 *                      v40init.h
 *                      icu.h
 *                      tcu.h
 *                      rtc.h
 *                      uart.h
 *                      ppi.h
 *                      cmos.h
 *                      remtron.h
 *                      analog.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         hardware.c
 *
 *      DESCRIPTION:
 *
 *      initializes :
 *                      V40 specific internal registers
 *                      PPI  on LPM-SBC40 CPU board
 *                      PPI  on iSBX expansion board
 *                      ICU  on LPM-SBC40 CPU board
 *                      TCU  on LPM-SBC40 CPU board
 *                      RTC  on LPM-SBC40 CPU board
 *                      SCU  on LPM-SBC40 CPU board
 *                      UART on LMP-SBC40 CPU board
 *                      CMOS battery-back ram
 *                      Exception interrupt vectors
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_hardware(unsigned int configuration)
 {

 #ifdef PC

	init_icu(); 					/* initialize icu data structures for PC */

	init_REMTRON(); 				/* initialize CMD/TM */

	init_A_D(); 					/* initialize A/D converter */

	D_A_init(); 					/* initialize D/A converter */

 #else

/****************************************************************************
 *
 *	initialize V40 processor
 *
 ***************************************************************************/

 //#if !defined(V20)
 #if defined(V50)
 #if defined(V40)

	v40_init();             /* initialize V40 specific hardware */

 #endif
 #endif

/****************************************************************************
 *
 *	initialize pointers to battery backed ram
 *
 ***************************************************************************/

 #if !defined(V50)

        init_cmos();                    /* setup pointer to CMOS RAM */

 #endif

/****************************************************************************
 *
 *	initialize Interrupt Controller
 *
 ***************************************************************************/

	init_icu();                     /* initialize icu on LPM-SBC40 card */

/****************************************************************************
 *
 *	setup exception vectors
 *
 ***************************************************************************/

	if( (configuration & ROM_ENV) == ROM_ENV )
		init_exception();       /* initialize exception vectors */

/****************************************************************************
 *
 *	initialize serial control unit
 *
 ***************************************************************************/

 #ifndef I486

	if( (configuration & ROM_ENV) == ROM_ENV )
		scu_open(9600);         /* initialize SCU */
	else
		int_enable(SCU_IRQ);    /* kernel */

 #endif

/****************************************************************************
 *
 *	initialize Uart
 *
 ***************************************************************************/

 #if defined(V40)

	uart_open(9600);                /* initialize UART */

 #elif defined(V50)

	scu_open(9600);                 /* initialize SCU */

 #elif defined(V20)

	scu_open(9600);                 /* initialize SCU */

 #endif

/****************************************************************************
 *
 *	initialize PPI
 *
 ***************************************************************************/

 #ifndef I486

	init_ppi();                     /* initialize ppi on LPM-SBC40 card */

 #endif

/****************************************************************************
 *
 *	initialize Real Time Clock
 *
 ***************************************************************************/

 #if defined(V40)

	init_clock();							/* initialize RTC on LPM-SBC40 card */

	if((battery_backup_check()==ERR)) {
		set_clock_time(0,0,0);				/* initialize RTC time */
		set_clock_date(1,1,1990);			/* initialize RTC date */
		}

 #elif defined(V50)

	init_clock();                           /* initialize RTC on LPM-SBC40 card */

	set_clock_time(0,0,0);                  /* initialize RTC time */
	set_clock_date(1,1,1990);				/* initialize RTC date */

 #endif

/****************************************************************************
 *
 *	initialize timer/counter 0 for main timing loop
 *
 ***************************************************************************/

	init_timer(SAMPLE_RATE);        /* initialize timer control unit */

/****************************************************************************
 *
 *	initialize Command/Telemetry interface
 *
 ***************************************************************************/

	init_REMTRON();                 /* initialize CMD/TM */

/****************************************************************************
 *
 *	initialize Analog I/O
 *
 ***************************************************************************/

	init_A_D();                     /* initialize A/D converter */

	D_A_init();                     /* initialize D/A converter */

 #endif /* ifdef PC else */

 }

 #pragma page(1)

/****************************************************************************
 *
 *	deinitialize Hardware
 *
 ***************************************************************************/
 void de_init_hardware(unsigned int configuration)
 {

 #ifdef PC

	de_init_REMTRON();				/* initialize CMD/TM */

	de_init_A_D();					/* initialize A/D converter */

 #endif

 }


 #pragma page(1)

/****************************************************************************
 *
 *      COLD RESET
 *
 *      DELCARATION:    void reset_boot(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    CPU reset
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:   Memory Location ffff:0000
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         hardware.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the processor thru its powerup cold boot
 *      vector. This functions does not return as the processor, once
 *      reset, will return to the original cold start up code.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void reset_boot(void)
 {
	_disable();                     /* disable interrutps at CPU

	int_disable_all();              /* disable interrutps at ICU */

	funptr_reset = (void (interrupt far *)(void)) 0xffff0000;
					/* install pointer to application */

	(*funptr_reset)();              /* call application code */

 }
