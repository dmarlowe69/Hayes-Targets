/* file: hardware.h */

/****************************************************************************
 *
 *	hardware header file
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_HARDWARE

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
 *	Define Hardware configuration options
 *
 ***************************************************************************/

 #define RAM_ENV	0x00
 #define ROM_ENV	0x01
 #define PC_ENV 	0x02

/****************************************************************************
 *
 *      Hardware initialization function prototypes
 *
 ***************************************************************************/

 void init_hardware(unsigned int configuration);	/* initialize hardware */

 void de_init_hardware(unsigned int configuration);	/* deinitialize hardware */

 void reset_boot(); 								/* cold boot */

 void (interrupt far *funptr_reset)(void);			/* pointer to function */

 #define _INC_HARDWARE
 #endif /* ifndef _INC_HARDWARE */
