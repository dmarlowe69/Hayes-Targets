/* file: menu_rtc.c */

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
 *      RTC serial interface function prototypes
 *
 ***************************************************************************/

 void rtc_menu(void);           /* RTC functions menu */
 void set_time(void);           /* set time */
 void set_date(void);           /* set data */
 void display_clock(void);      /* display clock */

 #pragma page(1)

 #if !defined(V20)

 #pragma page(1)

/****************************************************************************
 *
 *      RTC functions menu
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void rtc_menu(void)
 {
	int c;

	while(TRUE)
	{
		com_puts("\r\n\r\nHAYES TARGETS BIRD BRAIN\r\n");

		com_puts("\r\n1. Display Time/Date\r\n");
		com_puts("\r\n2. Set Clock Time\r\n");
		com_puts("\r\n3. Set Clock Date\r\n");
		com_puts("\r\n4. exit to main menu\r\n");

		com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(c)
		{
			case '1':       display_clock();
					break;

			case '2':       set_time();     /* get the time and set the clock*/
					break;

			case '3':       set_date();     /* get the date and set the clock */
					break;

			case '4':       return;

			default:
					break;
		}
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Real Time Clock initialization
 *
 *      Prompt the user to enter the time, then set the hardware clock
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void set_time(void)
 {
	char temp[80];
	char *str;
	int minutes,hours;

	sprintf(temp,"\r\n\nEnter time (HH:MM) ");
	com_puts(temp);
	com_gets(temp);        /* get a string from the serial port */

/****************************************************************************
 *
 *      sscanf is tied into the DOS system and rather than re-write it we
 *      will do a brute force parsing of the string entered. This technique
 *      assumes a valid string.
 *
 ***************************************************************************/

	str = temp;                     /* set text pointer to temp address */
	while(*str)                     /* check string till terminating null */
		{
		if(*str == ':')         /* check for delimiter */
			*str = '\0';    /* insert null character */
		str++;                  /* increment pointer */
		}
	hours = atoi(temp);
	minutes = atoi(temp + 3);

/****************************************************************************
 *
 *      Now set the hardware clock to the desired time
 *
 ***************************************************************************/

	set_clock_time(hours,minutes,0);

	sprintf(temp,"\r\n\r\nClock set to %02d:%02d:00\r\n",hours,minutes);
	com_puts(temp);
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Real Time Clock initialization
 *
 *      Prompt the user to enter the date and set the hardware clock
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void set_date(void)
 {
	char temp[80];
	char *str;
	int day,month,year;

	sprintf(temp,"\r\n\r\nEnter the date (MM/DD/YYYY) ");
	com_puts(temp);
	com_gets(temp);        /* get the string from the serial port */

/****************************************************************************
 *
 *      This code also uses the same "brute force" parsing technique to
 *      avoid using sscanf and all that it pulls in with it. Again, a valid
 *      string is assumed
 *
 ***************************************************************************/

	str = temp;
	while(*str)
		{
		if(*str == '/')
			*str = '\0';
		str++;
		}
	month = atoi(temp);
	day = atoi(temp + 3);
	year = atoi(temp + 6);

	sprintf(temp,"\r\n\r\nSetting date to %02d/%02d/%04d\r\n",month,day,year);
	com_puts(temp);

/****************************************************************************
 *
 *      Now set the hardware clock to the desired value
 *
 ***************************************************************************/

	set_clock_date(month,day,year);
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Real Time Clock display routine
 *
 *      outputs date and time from RTC to serial port
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void display_clock(void)
 {
	char temp[80];

	read_clock_time(&rtc);
	read_clock_date(&rtc);
	new_line();
	sprintf(temp,"%02d/%02d/%04d %02d:%02d:%02d",rtc.month,rtc.day,rtc.year,rtc.hours,rtc.minutes,rtc.seconds);
	com_puts(temp);
	new_line();
 }

 #endif
