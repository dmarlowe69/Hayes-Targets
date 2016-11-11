/* file: watchdog.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("WatchDOG Timer Functions")
 #pragma subtitle("(c)1990-1994 Hayes Targets")

/*@H*************************************************************************
 *
 *      WatchDOG Timer functions
 *
 *      written by denton marlowe
 *		(c)1990-1994 Hayes Targets
 *
 ************************************************************************@H*/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/90	D.marlowe	Original release
 *	05/12/94	D.marlowe	Added I486 routines
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

 #include	<conio.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"

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
 *      WatchDOG Timer function prototypes
 *
 ***************************************************************************/

 void init_the_dog(void);       /* initialize watch dog timer */
 void kick_the_dog(void);       /* toggle watch dog timer */
 void deinit_the_dog(void);		/* deinitialize watch dog timer */

/****************************************************************************
 *
 *      end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Watch Dog Timer initialization routine ( BIRD DOG )
 *
 *      DELCARATION:    int init_the_dog(void)
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
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         watchdog.c
 *
 *      DESCRIPTION:
 *
 *		This function is the watchdog initialization routine
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_the_dog(void)
 {

 #if defined(V40)

	outp(WATCH_DOG,0);		/* strobe the watch dog timer port */

 #elif defined(I486)

	outp(WATCH_DOG_INIT,1);

 #endif

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Watch Dog Timer up date routine ( BIRD DOG )
 *
 *      DELCARATION:    int kick_the_dog(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    watch dog timer strobed
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         watchdog.c
 *
 *      DESCRIPTION:
 *
 *      This function strobes the on board watch dog timer. The timer, if
 *      enabled by a hardware jummper, must be strobed at least every 1.5
 *      seconds, or an hardware RESET is generated.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void kick_the_dog(void)
 {

 #if defined(V40)

	outp(WATCH_DOG,0);		/* strobe the watch dog timer port */

 #elif defined(I486)

	outp(WATCH_DOG_KICK,(inp(WATCH_DOG_KICK)^0x01));

 #endif

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *		Watch Dog Timer deinitialization routine ( BIRD DOG )
 *
 *		DELCARATION:	int deinit_the_dog(void)
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
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         watchdog.c
 *
 *      DESCRIPTION:
 *
 *		This function is the watchdog deinitialization routine
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void deinit_the_dog(void)
 {

 #if defined(I486)

	outp(WATCH_DOG_INIT,0);

 #endif

 }
