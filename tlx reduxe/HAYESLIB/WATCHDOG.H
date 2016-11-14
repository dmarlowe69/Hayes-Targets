/* file: watchdog.h */

/****************************************************************************
 *
 *      WatchDog timer header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *		02/06/90		D.marlowe		Original release
 *		05/12/94		D.marlowe		Added I486 routines
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      WatchDog timer function prototypes
 *
 ***************************************************************************/

 void init_the_dog(void);       /* initialize watch dog timer */
 void kick_the_dog(void);       /* toggle watch dog timer */
 void deinit_the_dog(void);		/* deinitialize watch dog timer */
