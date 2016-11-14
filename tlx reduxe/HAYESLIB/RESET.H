/* file: reset.h */

/****************************************************************************
 *
 *	reset header file
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_RESET

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
 *	reset function prototypes
 *
 ***************************************************************************/

 void reset_boot();								/* cold boot */

 void (interrupt far *funptr_reset)(void);		/* pointer to function */

 #define _INC_RESET
 #endif /* ifndef _INC_RESET */
