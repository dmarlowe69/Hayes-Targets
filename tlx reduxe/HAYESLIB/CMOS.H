/* file: cmos.h */

/***************************************************************************
 *
 *      BIRD CMOS RAM header file
 *
 *      written by denton marlowe
 *      (c)1989,1990 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      CMOS definitions
 *
 ***************************************************************************/

 #define OK   0
 #define ERR -1

/****************************************************************************
 *
 *      cmos ram structure
 *
 ***************************************************************************/

 typedef volatile struct {
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

 extern cmos far *cmosptr;           /* define pointer to far structure */

/****************************************************************************
 *
 *      CMOS RAM functions prototypes
 *
 ***************************************************************************/

 void init_cmos(void);

 int battery_backup_check(void);
 void battery_backup_set(void);
