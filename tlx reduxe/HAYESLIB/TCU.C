/* file: tcu.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Timer/Counter Control Unit (TCU) functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      BIRD Timer/Counter Unit (TCU) functions
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
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include       <dos.h>
 #include       <conio.h>

/****************************************************************************
 *
 *      Include BIRD definitions
 *
 ***************************************************************************/

 #include       "clock.h"
 #include       "config.h"
 #include       "icu.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      I/O addresses of TCU control registers
 *
 *      The TIMER_BASE defines the address of the counter/timer (8254
 *      equivalent) present in the V40 and is controlled by the OPHA register
 *      (FFFCH) and the TULA register (FFF9H).
 *
 ***************************************************************************/

 #ifndef TIMER_BASE

 #error Base address of TCU not defined

 #endif

/****************************************************************************
 *
 *      TCU I/O port addresses
 *
 ***************************************************************************/

 #if !defined(V50)

 #define TCT0    TIMER_BASE     /* Lower address of timer unit 0 */
 #define TCT1    TIMER_BASE+1   /* Lower address of timer unit 1 */
 #define TCT2    TIMER_BASE+2   /* Lower address of timer unit 2 */
 #define TCTCMD  TIMER_BASE+3   /* Lower address of command register */

 #else

 #define TCT0    TIMER_BASE     /* Lower address of timer unit 0 */
 #define TCT1    TIMER_BASE+2   /* Lower address of timer unit 1 */
 #define TCT2    TIMER_BASE+4   /* Lower address of timer unit 2 */
 #define TCTCMD  TIMER_BASE+6   /* Lower address of command register */

 #endif

/****************************************************************************
 *
 *      TCU command words
 *
 ***************************************************************************/

 #define SC_0 0x00              /* select counter 0 */
 #define SC_1 0x40              /* select counter 1 */
 #define SC_2 0x80              /* select counter 2 */
 #define SC_READ_BACK 0xc0      /* read-back command */

 #define RW_LATCH 0x00          /* counter latch command */
 #define RW_LSB 0x10            /* read/write least significant byte only */
 #define RW_MSB 0x20            /* read/write most significant byte only */
 #define RW_LSB_MSB 0x30        /* read/write LSB first, then MSB */

 #define MODE_0 0x00            /* counter mode 0 */
 #define MODE_1 0x02            /* counter mode 1 */
 #define MODE_2 0x04            /* counter mode 2 */
 #define MODE_3 0x06            /* counter mode 3 */
 #define MODE_4 0x08            /* counter mode 4 */
 #define MODE_5 0x0a            /* counter mode 5 */

 #define BCD_BINARY 0x00        /* binary counter - 16 bits */
 #define BCD_BCD 0x01           /* binary coded decimal (4 decades) */

/****************************************************************************
 *
 *      TCU function prototypes
 *
 ***************************************************************************/

 void init_timer(unsigned int baud_rate);       /* setup main timing loop interrupt */
 void uart_baud(unsigned int baud_rate);        /* setup baud rate for UART */
 void scu_baud(unsigned int baud_rate);         /* setup baud rate for serial control unit (SCU) */
 void install_application(void (far *routine)(void));
						/* install pointer to main timing loop application */
 void far dummy_application(void);              /* dummy main timing loop application */

/****************************************************************************
 *
 *      TCU function prototype for interrupt service routine
 *
 ***************************************************************************/

 void interrupt far tcu0_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

/****************************************************************************
 *
 *      TCU data structure
 *
 ***************************************************************************/

 struct TCU_TABLE {
    unsigned int ticks;                 /* timing loop counter */
	};

 struct TCU_TABLE tcu_table;            /* define data structre */

/****************************************************************************
 *
 *      TCU function pointer to application function
 *
 ***************************************************************************/

 void (far *funptr_application)(void);  /* pointer to function */

/****************************************************************************
 *
 *      end of TCU definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      BIRD CMD/TM timer/counter initialization - 82c54 Compatiable
 *
 *      DELCARATION:    void init_tcu(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    timer 0 interrupt serivce routine installed and enabled
 *                      Timer 0 set to count down at given sample rate
 *                      dummy application timing loop function installed
 *
 *      LOCALS:         int baud
 *
 *      EXTERNALS
 *       Referenced:    void _disable(void)
 *                      void install_vector(void far *routine,int irq)
 *                      void install_application(void far *routine)
 *                      void far dummy_application(void)
 *                      int outp(unsigned portid, int byte_value)
 *                      void _enable(void)
 *                      struct TCU_TABLE tcu_table
 *
 *      INCLUDES:       <dos.h>
 *                      <conio.h>
 *                      config.h
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         tcu.c
 *
 *      DESCRIPTION:
 *
 *      counter number 0 is hardwared internaly as an interrupt source
 *      to the interrupt control unit and is used to generate the sample
 *      timming of the autopilot.
 *
 *      count = ( SYSTEM CLOCK MHZ / pre-scale) * sample time in seconds
 *
 *      example
 *              For an 10MHZ system with a prescale of 2 and a sample time
 *              of 0.01 seconds, the counter value is:
 *
 *      ( 10MHZ/ 2 ) * 0.01 SEC = 50000.
 *                              = 0XC350
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_timer(unsigned int baud_rate)
 {
	int baud;               /* timer frequency */

/****************************************************************************
 *
 *      calculate the necessary divisor given the desired baud rate and the
 *      input frequency to the counter/timer
 *
 ***************************************************************************/

	baud =(unsigned) (CLOCK_RATE / (long) baud_rate);

/****************************************************************************
 *
 *      timer zero - sample rate timer
 *
 ***************************************************************************/

	outp(TCTCMD,SC_0 | RW_LSB_MSB | MODE_3 | BCD_BINARY);
				/* setup sample time counter */
				/* timer 0,lo/hi, mode 0,binary */

	outp(TCT0,(baud & 0xff));       /* low byte of counter */
	outp(TCT0,(baud >> 8) & 0xff);  /* high byte of counter */

/****************************************************************************
 *
 *      install interrupt service routine
 *
 ***************************************************************************/

	_disable();                     /* disable interrupts till installation complete */

	install_application(dummy_application);

	install_vector(tcu0_int_handler,TCT0_IRQ);

	int_enable(TCT0_IRQ);

	tcu_table.ticks = 0;            /* initialize ticker */

	_enable();

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU Baud Rate Generation
 *
 *      DELCARATION:    void scu_baud(unsigned int baud_rate)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    timer 1 set free running at 16X given baud rate
 *
 *      LOCALS:         unsigned int baud
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int byte_value)
 *
 *      INCLUDES:       <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         tcu.c
 *
 *      DESCRIPTION:
 *
 *      counter number 1 is used as a baud rate generator for the internal
 *      serial control unit
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if !defined(V20)
 #if !defined(PC)
 #if !defined(I486)

 void scu_baud(unsigned int baud_rate)
 {
	unsigned baud;

/****************************************************************************
 *
 *      calculate the necessary divisor given the desired baud rate and the
 *      input frequency to the counter/timer
 *
 ***************************************************************************/

	baud =(unsigned) (CLOCK_RATE / ((long)baud_rate * 16L));

/****************************************************************************
 *
 *      First set up the counter/timer for the desired baud rate
 *
 ***************************************************************************/

	outp(TCTCMD,SC_1 | RW_LSB_MSB | MODE_3 | BCD_BINARY);
                                    /* Write command word to counter/timer */
	outp(TCT1,(baud & 0xff));       /* write lsb of divisor */
	outp(TCT1,(baud >> 8) & 0xff);  /* write msb of divisor */
 }

 #endif
 #endif
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART Baud Rate Generation
 *
 *      DELCARATION:    void uart_baud(unsigned int baud_rate)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    timer 2 set free running at 16X given baud rate
 *
 *      LOCALS:         unsigned int baud
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int byte_value)
 *
 *      INCLUDES:       <conio.h.
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         tcu.c
 *
 *      DESCRIPTION:
 *
 *      counter number 2 is used as a baud rate generator for the 8251 UART
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if defined(V40)

 void uart_baud(unsigned int baud_rate)
 {
	unsigned baud;

/****************************************************************************
 *
 *      calculate the necessary divisor given the desired baud rate and the
 *      input frequency to the counter/timer
 *
 ***************************************************************************/

	baud =(unsigned) (CLOCK_RATE / ((long)baud_rate * 16L));

/****************************************************************************
 *
 *      First set up the counter/timer for the desired baud rate
 *
 ***************************************************************************/

	outp(TCTCMD,SC_2 | RW_LSB_MSB | MODE_3 | BCD_BINARY);
					/* Write command word to counter/timer */
	outp(TCT2,(baud & 0xff));       /* write lsb of divisor */
	outp(TCT2,(baud >> 8) & 0xff);  /* write msb of divisor */
 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Application function pointer Initialization
 *
 *      DELCARATION:    void install_application(void far *routine)
 *
 *      PARAMETERS:     void far routine - far pointer to function
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    application pointer set to given function
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void int_disable(int irq)
 *                      void int_enable(int irq)
 *
 *      INCLUDES:       config.h
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         tcu.c
 *
 *      DESCRIPTION:
 *
 *      This function set the timing loop function pointer to the application
 *      specific main timing loop function.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void install_application(void (far *routine)(void))
 {
	int_disable(TCT0_IRQ);          /* disable timer/counter interrupts */

	funptr_application = routine;   /* install pointer to application */

	int_enable(TCT0_IRQ);           /* enable timer/counter interrutps */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Dummy Application function
 *
 *      DELCARATION:    void far dummy_application(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    None
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         tcu.c
 *
 *      DESCRIPTION:
 *
 *      This empty function is used as the default value for the application
 *      function pointer
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void far dummy_application(void)
 {

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      BIRD counter/timer 0 interrupt service routine
 *
 *      DELCARATION:    void interrupt far tcu0_int_handler(
 *                                    unsigned int es, unsigned int ds,
 *                                    unsigned int di, unsigned int si,
 *                                    unsigned int bp, unsigned int sp,
 *                                    unsigned int bx, unsigned int dx,
 *                                    unsigned int cx, unsigned int ax,
 *                                    unsigned int ip, unsigned int cs,
 *                                    unsigned int flags)
 *
 *      PARAMETERS:     Processor's registers
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    struct TCU_TABLE tcu_table
 *                      void toggle(int bit)
 *                      outp(unsigned portid, int byte_value)
 *                      (*funptr_application)()
 *
 *      INCLUDES:       <dos.h>
 *                      <conio.h>
 *                      config.h
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         tcu.c
 *
 *      DESCRIPTION:
 *
 *      This function is invoked by the timer/counter 0 interrupt. A tick
 *      counter is maintained and then a function is called through a pointer.
 *      This allows the application to alter the function of this service
 *      at run time.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void interrupt far tcu0_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	_enable();                  /* enable interrupts */

	++tcu_table.ticks;          /* increment timer */

	(*funptr_application)();    /* call application code */

	outp(ICU_BASE,TCT0_EOI);    /* specific EOI command */

 }                                  /* return from interrupt */
