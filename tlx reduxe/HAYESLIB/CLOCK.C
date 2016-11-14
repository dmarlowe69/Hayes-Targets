/* file: clock.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("CLOCK Definitions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/****************************************************************************
 *
 *	CLOCK Defintions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

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

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Clock Rate Defintions
 *
 ***************************************************************************/

 #define CLOCK5 	2500000L
 #define CLOCK8 	4000000L
 #define CLOCK10	5000000L
 #define CLOCKXX	1193181L	/* standard PC rate */
 #define CLOCKX		1193181L	/* standard PC rate */

/****************************************************************************
 *
 *	define data structures for CLOCK
 *
 ***************************************************************************/

 struct CLOCK {
	unsigned long int PRESCALE;
	unsigned long int CLOCK_RATE;
	};

/****************************************************************************
 *
 *	initialize data structures for CLOCK
 *
 ***************************************************************************/

 struct CLOCK clock = {
	{2L};
	{5000000L};
	};
