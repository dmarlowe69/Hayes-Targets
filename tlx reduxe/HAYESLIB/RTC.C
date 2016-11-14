/* file: rtc.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Real Time Clock (RTC) functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      Real Time Clock functions
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

 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *      Include HAYES definitions
 *
 ***************************************************************************/

 #include	"config.h"
 #include	"icu.h"
 #include	"hardware.h"

 #ifdef REMOTE_I_O

 #include	"remoteio.h"

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	set definition of Real Time Clock used
 *
 ***************************************************************************/

 #if defined(V40)

 #define OKI6264

 #pragma message("OKI6264")

 #elif defined(V50)

 #define OKI6264
 #define NO_RTC_IRQ

 #pragma message("OKI6264")

 #elif defined(V20)

 #define NO_RTC
 #define NO_RTC_IRQ

 #elif defined(I486)

 #define MC146818A

 #pragma message("MC146818A")

 #endif

/****************************************************************************
 *
 *	set definition of Real Time Clock interrupt
 *
 ***************************************************************************/

 #if defined(REMOTE_I_O)

 #define NO_RTC_IRQ

 #endif

/****************************************************************************
 *
 *	check clock I/O address
 *
 ***************************************************************************/

 #if !defined(NO_RTC)

 #if !defined CLOCK_BASE

 #error Base address of RTC not defined

 #endif

 #endif

/****************************************************************************
 *
 *	set RTC definitions
 *
 ***************************************************************************/

 #if defined(V40)

 #define CLOCK_SEC_1  CLOCK_BASE
 #define CLOCK_SEC_10 CLOCK_BASE+1
 #define CLOCK_MIN_1  CLOCK_BASE+2
 #define CLOCK_MIN_10 CLOCK_BASE+3
 #define CLOCK_HR_1   CLOCK_BASE+4
 #define CLOCK_HR_10  CLOCK_BASE+5
 #define CLOCK_DAY_1  CLOCK_BASE+6
 #define CLOCK_DAY_10 CLOCK_BASE+7
 #define CLOCK_MON_1  CLOCK_BASE+8
 #define CLOCK_MON_10 CLOCK_BASE+9
 #define CLOCK_YR_1   CLOCK_BASE+10
 #define CLOCK_YR_10  CLOCK_BASE+11
 #define CLOCK_WEEK   CLOCK_BASE+12
 #define CLOCK_CD     CLOCK_BASE+13
 #define CLOCK_CE     CLOCK_BASE+14
 #define CLOCK_CF     CLOCK_BASE+15

 #elif defined(V50)

 #define CLOCK_SEC_1  CLOCK_BASE
 #define CLOCK_SEC_10 CLOCK_BASE+2
 #define CLOCK_MIN_1  CLOCK_BASE+4
 #define CLOCK_MIN_10 CLOCK_BASE+6
 #define CLOCK_HR_1   CLOCK_BASE+8
 #define CLOCK_HR_10  CLOCK_BASE+10
 #define CLOCK_DAY_1  CLOCK_BASE+12
 #define CLOCK_DAY_10 CLOCK_BASE+14
 #define CLOCK_MON_1  CLOCK_BASE+16
 #define CLOCK_MON_10 CLOCK_BASE+18
 #define CLOCK_YR_1   CLOCK_BASE+20
 #define CLOCK_YR_10  CLOCK_BASE+22
 #define CLOCK_WEEK   CLOCK_BASE+24
 #define CLOCK_CD     CLOCK_BASE+26
 #define CLOCK_CE     CLOCK_BASE+28
 #define CLOCK_CF     CLOCK_BASE+30

 #elif defined(MC146818A)

 #define CMOS_CNTRL		0x70
 #define CMOS_DATA		0x71

 #define SECONDS		0x00
 #define ALARM_SECONDS	0x01
 #define MINUTES		0x02
 #define ALARM_MINUTES	0x03
 #define HOURS			0x04
 #define ALARM_HOURS	0x05
 #define DAY_OF_WEEK	0x06
 #define DAY_OF_MONTH	0x07
 #define MONTH			0x08
 #define YEAR			0x09

 #define CMOS_MEMORY	0x0F
 #define LAST_ADDRESS	0xFF

 #endif

/****************************************************************************
 *
 *      RTC definitions
 *
 ***************************************************************************/

 #ifdef  OKI6264

 #define int_1_64_sec 0x00
 #define int_1_sec    0x04
 #define int_1_min    0x80
 #define int_1_hour   0xc0

 #elif defined(MC146818A)

 #define REGISTER_A 	0x0A
 #define UIP			0x80
 #define DV2			0x40
 #define DV1			0x20
 #define DV0			0x10
 #define RS3			0x08
 #define RS2			0x04
 #define RS1			0x02
 #define RS0			0x01

 #define REGISTER_B 	0x0B
 #define SET			0x80
 #define PIE			0x40
 #define AIE			0x20
 #define UIE			0x10
 #define SQWE			0x08
 #define DM 			0x04
 #define HR_24			0x02
 #define DSE			0x01

 #define REGISTER_C 	0x0C
 #define IRQF			0x80
 #define PF 			0x40
 #define AF 			0x20
 #define UF 			0x10

 #define REGISTER_D 	0x0D
 #define VRT			0x80

 #endif

/****************************************************************************
 *
 *      RTC function prototypes
 *
 ***************************************************************************/

 void init_clock(void);         /* initialize RTC */
 void clock_init(void);         /* Initialize the 6264 clock chip */
 void clock_int_setup(void);    /* Set up the 1 second interrupt from the clock */
 void set_clock_time(int hours,int minutes,int seconds); /* set time */
 void set_clock_date(int month,int day,int year);        /* set date */
 void read_clock_time(volatile struct RTC *rtc);         /* read time */
 void read_clock_date(volatile struct RTC *rtc);         /* read date */

 void install_rtc_application(void (far *routine)(void));
														 /* install pointer to main timing loop application */
 void far dummy_rtc_application(void);                   /* dummy main timing loop application */

 int binary_to_bcd(int number);
 int bcd_to_binary(int number);

/****************************************************************************
 *
 *      RTC function prototype for interrupt service routine
 *
 ***************************************************************************/

 void interrupt far clock_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

/****************************************************************************
 *
 *      Structures
 *
 ***************************************************************************/

 volatile struct RTC {
		int ticks;
		int seconds;
		int minutes;
		int hours;
		int day;
		int month;
		int year;
		};

/****************************************************************************
 *
 *      RTC global variables
 *
 *      These global variables are updated once each second by the Real Time
 *      clock ISR.
 *
 ***************************************************************************/

 #if !defined(NO_RTC)

 volatile struct RTC rtc;                       /* RTC data structure */

 #endif

/****************************************************************************
 *
 *      RTC function pointer to application function
 *
 ***************************************************************************/

 #if !defined(NO_RTC)

 void (far *funptr_rtc_application)(void);      /* pointer to function */

 #endif

/****************************************************************************
 *
 *      end of RTC defintions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock initialization
 *
 *      DELCARATION:    void init_clock(void)
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
 *       Referenced:    void clock_init(void)
 *                      void clock_int_setup(void)
 *                      struct RTC rtc
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      initialize Real Time Clock
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if !defined(NO_RTC)

 void init_clock(void)
 {

	clock_init();					/* initialize RTC */
	rtc.ticks = 0;                  /* initialize 1 second ticker */

 #ifndef NO_RTC_IRQ

	clock_int_setup();				/* setup RTC interrupt service */

 #endif

 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock initialization
 *
 *      DELCARATION:    void clock_init(void)
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
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      INCLUDES:       <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      This routine simply resets and then, places the clock chip into
 *      24 hour time keeping mode
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if !defined(NO_RTC)

 void clock_init(void)
 {
 #if defined(OKI6264)

	outp(CLOCK_CF,1);		/* Assert reset to clock */
	outp(CLOCK_CF,5);       /* Assert reset and 24 hour mode */
	outp(CLOCK_CF,4);		/* And finally only 24 hour mode */

 #elif defined(MC146818A)

 int temp;

	outp(CMOS_CNTRL,REGISTER_B);	// Set	"SET" bit to inhibit updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp | SET);

	temp = DV1 | RS2 | RS1; 		// 32.768KHZ and 1024KHZ SQW
	outp(CMOS_CNTRL,REGISTER_A);
	outp(CMOS_DATA,temp);

	temp = HR_24;					// 12 hour mode
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp);

	outp(CMOS_CNTRL,REGISTER_B);	// Clear "SET" bit to allow updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp & ~SET);

 #endif

 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock initialization
 *
 *      DELCARATION:    void set_clock_time(int hours, int minutes, int seconds)
 *
 *      PARAMETERS:     time to set hh:mm:ss
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
 *       Referenced:    int outp(unsigned portid, int value)
 *                      struct RTC rtc
 *
 *      INCLUDES:       <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      This function sets the Hardware clock to the argument values of
 *      hours,minutes and seconds
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if !defined(NO_RTC)

 void set_clock_time(int hours,int minutes,int seconds)
 {
 #if defined(OKI6264)

	outp(CLOCK_HR_10,hours / 10);	/* ten's digit of hour */
	outp(CLOCK_HR_1 ,hours % 10);   /* one's digit of hour */
	outp(CLOCK_MIN_10,minutes / 10); /* ten's digit of minute */
	outp(CLOCK_MIN_1 ,minutes % 10); /* one's digit of minute */
	outp(CLOCK_SEC_10,seconds / 10); /* ten's digit of second */
	outp(CLOCK_SEC_1 ,seconds % 10); /* one's digit of second */

 #elif defined(MC146818A)

 int temp;

	outp(CMOS_CNTRL,REGISTER_B);	// Set "SET" bit to inhibit updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp | SET);

	temp = binary_to_bcd(seconds);
	outp(CMOS_CNTRL,SECONDS);
	outp(CMOS_DATA,temp);

	temp = binary_to_bcd(minutes);
	outp(CMOS_CNTRL,MINUTES);
	outp(CMOS_DATA,temp);

	temp = binary_to_bcd(hours);
	outp(CMOS_CNTRL,HOURS);
	outp(CMOS_DATA,temp);

	outp(CMOS_CNTRL,REGISTER_B);	// Clear "SET" bit to allow updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp & ~SET);

 #endif

	rtc.seconds = seconds;
	rtc.minutes = minutes;
	rtc.hours = hours;

 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock initialization
 *
 *      DELCARATION:    void set_clock_date(int month, int day, int year)
 *
 *      PARAMETERS:     date to set mm/dd/yyyy
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
 *       Referenced:    int outp(unsigned portid, int value)
 *                      struct RTC rtc
 *
 *      INCLUDES:       <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      This function sets the Hardware clock to the argument values of
 *      month, day, year
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if !defined(NO_RTC)

 void set_clock_date(int month,int day,int year)
 {
	if(year > 99)                           /* check date > 1999 */
		year = year - 1900;             /* offset date form 1900 */

 #if defined(OKI6264)

	outp(CLOCK_YR_10,year / 10);		/* ten's digit of year */
	outp(CLOCK_YR_1 ,year % 10);        /* one's digit of year */
	outp(CLOCK_MON_10,month / 10);      /* ten's digit of month */
	outp(CLOCK_MON_1 ,month % 10);      /* one's digit of month */
	outp(CLOCK_DAY_10,day / 10);        /* ten's digit of day */
	outp(CLOCK_DAY_1 ,day % 10);        /* one's digit of day */

 #elif defined(MC146818A)

 int temp;

	outp(CMOS_CNTRL,REGISTER_B);	// Set "SET" bit to inhibit updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp | SET);

	temp = binary_to_bcd(month);
	outp(CMOS_CNTRL,MONTH);
	outp(CMOS_DATA,temp);

	temp = binary_to_bcd(day);
	outp(CMOS_CNTRL,DAY_OF_MONTH);
	outp(CMOS_DATA,temp);

	temp = binary_to_bcd(year);
	outp(CMOS_CNTRL,YEAR);
	outp(CMOS_DATA,temp);

	outp(CMOS_CNTRL,REGISTER_B);	// Clear "SET" bit to allow updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp & ~SET);

 #endif

	rtc.day = day;
	rtc.month = month;
	rtc.year = year;
 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock initialization
 *
 *      DELCARATION:    void clock_int_setup(void)
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
 *       Referenced:    void _disable(void)
 *                      void install_vector( interrupt far routine, int irq)
 *                      int outp(unsigned portid, int value)
 *                      void int_enable(int irq)
 *                      void _enable(void)
 *
 *      INCLUDES:       <dos.h.
 *                      <conio.h>
 *                      config.h
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      The function initializes the hardware clock to cause an interrupt
 *      every second. It also installs the interrupt service routine and
 *      enables the interrupts.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef NO_RTC_IRQ

 void clock_int_setup(void)
 {
 #if defined(MC146818A)
 int temp;
 #endif

	_disable();

/****************************************************************************
 *
 *      Install dummy rtc application service routine for clock interrupt
 *
 ***************************************************************************/

	install_rtc_application(dummy_rtc_application);

/****************************************************************************
 *
 *      Install interrupt service routine for clock interrupt
 *
 ***************************************************************************/

	install_vector(clock_isr,CLOCK_IRQ);

 #if defined(OKI6264)

/****************************************************************************
 *
 *      turn on 1/64 second interrupt at clock chip
 *
 ***************************************************************************/

	outp(CLOCK_CE,int_1_64_sec);    /* 1/64 second interrupt enabled */

/****************************************************************************
 *
 *		Make sure V40 interrupt 2 pin is enabled
 *
 ***************************************************************************/

 #ifdef V40

	outp(0xfffe,(inp(0xfffe) & 0xf3) | 0x04);

 #endif

 #elif defined(MC146818A)

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	outp(CMOS_CNTRL,REGISTER_B);	// Set "SET" bit to inhibit updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp | SET);

	outp(CMOS_CNTRL,REGISTER_B);
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp | PIE);

	outp(CMOS_CNTRL,REGISTER_B);	// Clear "SET" bit to allow updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp & ~SET);

 #endif

/****************************************************************************
 *
 *      unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(CLOCK_IRQ);

 #if defined(MC146818A)

	int_enable(SLAVE_IRQ);

 #endif

	_enable();

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
 *      SOURCE:         rtc.c
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

 #ifndef NO_RTC_IRQ

 void install_rtc_application(void (far *routine)(void))
 {
	int_disable(CLOCK_IRQ);           /* disable timer/counter interrupts */

	funptr_rtc_application = routine; /* install pointer to application */

	int_enable(CLOCK_IRQ);             /* enable timer/counter interrutps */
 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Dummy Application function
 *
 *      DELCARATION:    void far dummy_rtc_application(void)
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
 *      SOURCE:         rtc.c
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

 #ifndef NO_RTC_IRQ

 void far dummy_rtc_application(void)
 {

 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock interrupt service routine
 *
 *      DELCARATION:    void clock_int_setup(void)
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
 *       Referenced:    void _enable(void)
 *                      int outp(unsigned portid, int value)
 *                      void (far *funptr_rtc_application)(void)
 *                      struct RTC rtc
 *
 *      INCLUDES:       <dos.h.
 *                      <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      This is the interrupt invoked function of the hardware clock 1 second
 *      interrupt. As written, it simple reads the current time and date into
 *      global variables and flashes the on board Status LED with each
 *      interrupt.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 
 #pragma page(1)

 #ifndef NO_RTC_IRQ

 void interrupt far clock_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	_enable();                      /* enable interrupts */

	rtc.ticks++;                    /* increment 1/64 second ticker */

	(*funptr_rtc_application)();    /* call application code */

 #if defined(OKI6264)

	outp(ICU_BASE,CLOCK_EOI);		/* send specific EOI to controller */

 #elif defined(MC146818A)

	outp(ICU_BASE,MASTER_EOI);		/* send specific EOI to controller #1 */
	outp(ICU_SLAVE_BASE,CLOCK_EOI);	/* send specific EOI to controller #2 */

 #endif
 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock Read Time Function
 *
 *      DELCARATION:    void read_clock_time(struct RTC *rtc)
 *
 *      PARAMETERS:     Pointer to data structure
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         int temp
 *
 *      EXTERNALS
 *       Referenced:    int inp(unsigned portid)
 *                      struct RTC rtc
 *
 *      INCLUDES:       <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      This function returns (through the provided pointers) the current
 *      time in hours, minutes and seconds
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 
 #pragma page(1)

 #if !defined(NO_RTC)

 void read_clock_time(volatile struct RTC *rtc)
 {
	int temp;                              /* local variable */

 #if defined(OKI6264)

/****************************************************************************
 *
 *      Read the LSB and MSB of seconds and combine
 *
 ***************************************************************************/

	temp = inp(CLOCK_SEC_10) & 0x0f;      /* get 10's of seconds */
	temp = (temp * 10) + (inp(CLOCK_SEC_1) & 0x0f); /* 1's of seconds */
	rtc->seconds = temp;

/****************************************************************************
 *
 *      Read the LSB and MSB of minutes and combine
 *
 ***************************************************************************/

	temp = inp(CLOCK_MIN_10) & 0x0f;
	temp = (temp * 10) + (inp(CLOCK_MIN_1) & 0x0f);
	rtc->minutes = temp;

/****************************************************************************
 *
 *      Read the LSB and MSB of hours and combine
 *
 ***************************************************************************/

	temp = inp(CLOCK_HR_10) & 0x0f;
	temp = (temp * 10) + (inp(CLOCK_HR_1) & 0x0f);
	rtc->hours = temp;

 #elif defined(MC146818A)

	outp(CMOS_CNTRL,REGISTER_B);	// Set "SET" bit to inhibit updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp | SET);

	outp(CMOS_CNTRL,SECONDS);
	temp = (CMOS_DATA,temp);
	rtc->seconds = bcd_to_binary(temp);

	outp(CMOS_CNTRL,MINUTES);
	temp = inp(CMOS_DATA);
	rtc->minutes = bcd_to_binary(temp);

	outp(CMOS_CNTRL,HOURS);
	temp = inp(CMOS_DATA);
	rtc->hours = bcd_to_binary(temp);

	outp(CMOS_CNTRL,REGISTER_B);	// Clear "SET" bit to allow updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp & ~SET);

 #endif

 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Real Time Clock Read Date Function
 *
 *      DELCARATION:    void read_clock_date(struct RTC *rtc)
 *
 *      PARAMETERS:     Pointer to data structure
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         int temp
 *
 *      EXTERNALS
 *       Referenced:    int inp(unsigned portid)
 *                      struct RTC rtc
 *
 *      INCLUDES:       <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rtc.c
 *
 *      DESCRIPTION:
 *
 *      This function returns through the provided pointers, the current date
 *      in month, day, and year.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if !defined(NO_RTC)

 void read_clock_date(volatile struct RTC *rtc)
 {
	int temp;                              /* local variable */

 #if defined(OKI6264)

/****************************************************************************
 *
 *      Read the LSB and MSB of the months and combine
 *
 ***************************************************************************/

	temp = inp(CLOCK_MON_10) & 0x01;      /* get 10's of months */
	temp = (temp * 10) + (inp(CLOCK_MON_1) & 0x0f);
	rtc->month = temp;

/****************************************************************************
 *
 *      Read the LSB and MSB of the days and combine
 *
 ***************************************************************************/

	temp = inp(CLOCK_DAY_10) & 0x03;      /* get 10's of days */
	temp = (temp * 10) + (inp(CLOCK_DAY_1) & 0x0f);
	rtc->day = temp;

/****************************************************************************
 *
 *      Read the LSB and MSB of the years and combine
 *
 ***************************************************************************/

	temp = inp(CLOCK_YR_10) & 0x0f;
	temp = (temp * 10) + (inp(CLOCK_YR_1) & 0x0f);
	temp = temp + 1900;
	rtc->year = temp;

 #elif defined(MC146818A)

	outp(CMOS_CNTRL,REGISTER_B);	// Set "SET" bit to inhibit updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp | SET);

	outp(CMOS_CNTRL,MONTH);
	temp = (CMOS_DATA,temp);
	rtc->month = bcd_to_binary(temp);

	outp(CMOS_CNTRL,DAY_OF_MONTH);
	temp = inp(CMOS_DATA);
	rtc->day = bcd_to_binary(temp);

	outp(CMOS_CNTRL,YEAR);
	temp = inp(CMOS_DATA);
	rtc->year = bcd_to_binary(temp);

	outp(CMOS_CNTRL,REGISTER_B);	// Clear "SET" bit to allow updates
	temp = inp(CMOS_DATA);
	outp(CMOS_CNTRL,REGISTER_B);
	outp(CMOS_DATA,temp & ~SET);

 #endif

 }

 #endif

 int binary_to_bcd(int number)
 {
	int high_nibble;
	int low_nibble;
	int bcd;

	high_nibble = number / 10;	// ten's digit of number;
	low_nibble	= number % 10;	// one's digit of number;
	bcd = low_nibble | ( (high_nibble << 4) & 0xf0 );

	return(bcd);

 }

 int bcd_to_binary(int number)
 {
	int high_nibble;
	int low_nibble;
	int binary;

	high_nibble = number & 0xf0;	// ten's digit of number;
	low_nibble	= number & 0x0f;	// one's digit of number;
	binary = low_nibble + (high_nibble * 10);

	return(binary);

 }
