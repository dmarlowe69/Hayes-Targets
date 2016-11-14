/* file: rtc.h */

/***************************************************************************
 *
 *      RTC header file
 *
 *      written by denton marlowe
 *      (c)1989,1990 Hayes Targets
 *
 **************************************************************************/

/****************************************************************************
 *
 *      RTC function prototypes
 *
 ***************************************************************************/

 void init_clock(void);         /* initialize RTC */
 void clock_init(void);         /* Initialize the 6264 clock chip */
 void clock_int_setup(void);    /* Set up the 1 second interrupt from the clock */
 void set_clock_time(int hrs,int mins,int secs);         /* set time */
 void set_clock_date(int month,int day,int year);        /* set date */
 void read_clock_time(volatile struct RTC *rtc);         /* read time */
 void read_clock_date(volatile struct RTC *rtc);         /* read date */

 void install_rtc_application(void (far *routine)());    /* install pointer to main timing loop application */
 void far dummy_rtc_application(void);                   /* dummy main timing loop application */

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
 *      define data which will be externel to other modules
 *
 ***************************************************************************/

 #if !defined(NO_RTC)

 extern volatile struct RTC rtc;				/* RTC data structure */

 #endif
