/* file: cmos.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Battery Backed Cmos RAM (CMOS) functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      BIRD CMOS RAM functions
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

 #include       "config.h"
 #include       "rtc.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      CMOS RAM functions prototypes
 *
 ***************************************************************************/

 void init_cmos(void);

 int battery_backup_check(void);
 void battery_backup_set(void);

/****************************************************************************
 *
 *      define data struture
 *
 ***************************************************************************/

 typedef struct {
	int checksum;
	int seconds;
	int minutes;
	int hours;
	int day;
	int month;
	int year;
	int stat0;
	int stat1;
	int stat2;
	int stat3;
	} cmos;

/****************************************************************************
 *
 *      define pointer to structure
 *      cmos ram structure is FAR DATA (not in our DATA SEGMENT)
 *
 ***************************************************************************/

 #if defined(V40)

	cmos far *cmosptr;              /* define pointer to far structure */

 #endif

/****************************************************************************
 *
 *      general definitions
 *
 ***************************************************************************/

 #define OK   0
 #define ERR -1

/****************************************************************************
 *
 *      end of cmos definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      CMOS RAM initialization
 *
 *      DELCARATION:    void init_cmos(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    pointer setup to cmos ram
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    typedef struct cmos
 *                      cmos far *cmosptr
 *
 *      INCLUDES:       config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         cmos.c
 *
 *      DESCRIPTION:
 *
 *      set pointer to CMOS RAM
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_cmos(void)
 {

 #if defined(V40)

	cmosptr = (void far *) CMOS_RAM;

	cmosptr->stat1 = 0x11;
	cmosptr->stat2 = 0x22;
	cmosptr->stat3 = 0x33;

 #endif

 }

 #pragma page(1)

 #if defined(V40)

/****************************************************************************
 *
 *      Battery Backup function
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 int battery_backup_check(void)
 {
	int far *farptr;
	int checksum;
	int checksum_read;
	int checksum_cal;
	int sec,min,hrs,dd,mm,yyyy;

/****************************************************************************
 *
 *      initialize pointer to battery backed ram
 *
 ***************************************************************************/

	cmosptr = (void far *) CMOS_RAM;

	farptr = (int far *) CMOS_RAM;

/****************************************************************************
 *
 *      read time and date information
 *
 ***************************************************************************/

	checksum_read = *(farptr)++;

	sec = *(farptr)++;
	min = *(farptr)++;
	hrs = *(farptr)++;
	dd = *(farptr)++;
	mm = *(farptr)++;
	yyyy = *(farptr)++;

	checksum_cal = sec|min|hrs|dd|mm|yyyy;

	if(checksum_cal == checksum_read)
		return(OK);
	else
		return(ERR);
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Battery Backup function
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void battery_backup_set(void)
 {
/****************************************************************************
 *
 *      initialize pointer to battery backed ram
 *
 ***************************************************************************/

	cmosptr = (void far *) CMOS_RAM;

/****************************************************************************
 *
 *      store time and date information
 *
 ***************************************************************************/

	read_clock_time(&rtc);                  /* read time */
	read_clock_date(&rtc);                  /* read date */

	cmosptr->checksum = rtc.seconds|rtc.minutes|rtc.hours|rtc.day|rtc.month|rtc.year;
                                            /* checksum */

	cmosptr->seconds = rtc.seconds;
	cmosptr->minutes = rtc.minutes;
	cmosptr->hours = rtc.hours;

	cmosptr->day = rtc.day;
	cmosptr->month = rtc.month;
	cmosptr->year= rtc.year;

	cmosptr->stat0 = 0x00;
	cmosptr->stat1 = 0x11;
	cmosptr->stat2 = 0x22;
	cmosptr->stat3 = 0x33;
 }

 #endif
