/* file: reset.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Hardware Initialization functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      Hardware Initialization functions
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
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"

 #include	"icu.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	reset function prototypes
 *
 ***************************************************************************/

 void reset_boot(); 								/* cold boot */

 void (interrupt far *funptr_reset)(void);			/* pointer to function */

/****************************************************************************
 *
 *	end of reset definitions
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      COLD RESET
 *
 *      DELCARATION:    void reset_boot(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    CPU reset
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:   Memory Location ffff:0000
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         hardware.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the processor thru its powerup cold boot
 *      vector. This functions does not return as the processor, once
 *      reset, will return to the original cold start up code.
 *
 *      written by denton marlowe
 *		(c)1990-1994 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void reset_boot(void)
 {
	_disable();                     /* disable interrutps at CPU

	int_disable_all();              /* disable interrutps at ICU */

	funptr_reset = (void (interrupt far *)(void)) 0xffff0000;
									/* install pointer to application */

	(*funptr_reset)();              /* call application code */

 }
